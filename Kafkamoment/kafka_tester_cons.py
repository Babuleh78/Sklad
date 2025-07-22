
import asyncio
import numpy as np
import cv2
from aiokafka import AIOKafkaConsumer
import json
import sys
from pathlib import Path
import time
import redis
import time

sys.path.append(str(Path(__file__).parent.parent))
from models.computer_vision_model.demo import cv_model
from models.recognition_model.model import rec_model
from models.demonstration import recognize_plate_text
from models.demonstration import rotate_to_horizontal

# Конфигурация
BOOTSTRAP_SERVERS = "localhost:9092"
TOPIC = "testtopic"

r = redis.Redis(
    host='localhost',
    port=6379,
    db=0,
    decode_responses=True  # Для автоматического декодирования строк
)

def print_all_redis_data():
    print("\nТекущее состояние парковки:")
    for key in r.keys('*'):
        entry_time = float(r.get(key))
        parked_seconds = time.time() - entry_time
        print(f"- {key}: на парковке {parked_seconds:.2f} сек")


async def recognize_plate_from_photo(photo_data: bytes, metadata: dict):

    global plate_confirmation, last_detection_time
    
    if 'plate_confirmation' not in globals():
        plate_confirmation = {}  # Словарь для хранения состояний номеров
        last_detection_time = time.time()  # Время последней детекции
    
    CONFIRMATION_COUNT = 3  # Необходимое количество подтверждений
    CONFIRMATION_TIMEOUT = 3.0  # Максимальное время между подтверждениями (сек)
    REDIS_MAX_ITEMS = 5 # Максимальное количество предметов в кэше
    REDIS_TTL = 120  # Максимальное время хранения предмета в кэше
    
    try: 
        current_time = time.time()
        nparr = np.frombuffer(photo_data, np.uint8)
        img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

        if img is None:
            #print("Не удалось декодировать изображение")
            return None
        
       
        detection_results = cv_model.predict(img, verbose=False)
        if not detection_results or len(detection_results[0].boxes) == 0:
            #print("Номера не обнаружены")
            return None

        license_plates = []
        for result in detection_results:
            boxes = result.boxes.xyxy.cpu().numpy()
            for box in boxes:
                x1, y1, x2, y2 = map(int, box[:4])
                license_plate = img[y1:y2, x1:x2]
                if license_plate.size > 0:
                    license_plates.append(license_plate)
        
        if not license_plates:
            #print("Не удалось извлечь области с номерами")
            return None
        
        corrected_plate = rotate_to_horizontal(license_plates[0])
        results = cv_model.predict(corrected_plate, verbose=False)
        if not results or len(results[0].boxes) == 0:
            #print("Не удалось уточнить позицию номера после поворота")
            return None
    
        x1, y1, x2, y2 = results[0].boxes.xyxy[0].cpu().numpy().astype(int)
        final_plate = corrected_plate[y1:y2, x1:x2]

        if final_plate.size == 0:
            #print("Пустая область после уточнения")
            return None
                    
        plate_text = recognize_plate_text(final_plate, rec_model)
        if not plate_text.strip():
            #print("Не удалось распознать текст номера")
            return None
        
        clean_plate_text = ''.join(c for c in plate_text if c.isalnum()).upper()
        
        current_state = plate_confirmation.get(clean_plate_text, {
            'count': 0,
            'last_seen': 0,
            'confirmed': False
        })
        
        if current_time - current_state['last_seen'] < CONFIRMATION_TIMEOUT:
            current_state['count'] += 1
        else:
            current_state['count'] = 1  
            
        current_state['last_seen'] = current_time
        
        if current_state['count'] >= CONFIRMATION_COUNT and not current_state['confirmed']:
            current_state['confirmed'] = True
            plate_confirmation[clean_plate_text] = current_state
            # Пошел redis
            # Номер есть - машина въезжает
            if r.exists(clean_plate_text):
                entry_time = float(r.get(clean_plate_text)) # Сейчас в Редисе пара Номер - время
                parking_time = time.time() - entry_time
                print(f"Машина с номером {clean_plate_text} простояла {parking_time:.2f} секунд и выехала")
                print_all_redis_data()
                
                r.delete(clean_plate_text) # Машина уехала
                return clean_plate_text
            # Если номера нет - это въезд
            else:
                # Если Redis переполнен, удаляем самый старый номер
                if r.dbsize() >= REDIS_MAX_ITEMS:
                    oldest_key = min(r.keys('*'), key=lambda k: float(r.get(k)))
                    r.delete(oldest_key)
                    print(f"Удален самый старый номер: {oldest_key}")
                
                # Добавляем номер с текущей временной меткой
                r.setex(clean_plate_text, REDIS_TTL, time.time())
                print(f"Машина с номером {clean_plate_text} заехала на парковку")
                print_all_redis_data()
                
                return clean_plate_text
        else:
            #print(f"Предварительное распознавание: {clean_plate_text} (подтверждений: {current_state['count']}/{CONFIRMATION_COUNT})")
            plate_confirmation[clean_plate_text] = current_state
            return None
            
    except Exception as e:
        print(f"Ошибка распознавания: {e}")
        return None

async def consume_and_process_photos():
    consumer = AIOKafkaConsumer(
        TOPIC,
        bootstrap_servers=BOOTSTRAP_SERVERS.split(","),
        group_id=None,
        auto_offset_reset="latest", 
        enable_auto_commit=False
    )
    
    await consumer.start()
    print("Подключено к Kafka. Ожидание новых сообщений...")
    
    try:
        async for msg in consumer:
            try:
                data = json.loads(msg.value.decode('utf-8'))
                photo_data = bytes.fromhex(data["photo"])
                metadata = data["metadata"]
                
                await recognize_plate_from_photo(photo_data, metadata)
                
            except Exception as e:
                print(f"Ошибка обработки сообщения: {e}")
                
    finally:
        await consumer.stop()
        cv2.destroyAllWindows()

async def main():
    try:
        await consume_and_process_photos()
    except KeyboardInterrupt:
        print("Остановлено пользователем")
    except Exception as e:
        print(f"Критическая ошибка: {e}")

if __name__ == "__main__":
    asyncio.run(main())
