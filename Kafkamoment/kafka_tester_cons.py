
import asyncio
import numpy as np
import cv2
from aiokafka import AIOKafkaConsumer
import json
import sys
from pathlib import Path

sys.path.append(str(Path(__file__).parent.parent))
from models.computer_vision_model.demo import cv_model
from models.recognition_model.model import rec_model
from models.demonstration import recognize_plate_text
from models.demonstration import rotate_to_horizontal

# Конфигурация
BOOTSTRAP_SERVERS = "localhost:9092"
TOPIC = "testtopic"

async def recognize_plate_from_photo(photo_data: bytes, metadata: dict):
    try:
        nparr = np.frombuffer(photo_data, np.uint8)
        img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        
        detection_results = cv_model.predict(img)[0]

        license_plates = []
        for result in detection_results:
            boxes = result.boxes.xyxy.cpu().numpy()
            for box in boxes:
                x1, y1, x2, y2 = map(int, box[:4])
                license_plate = img[y1:y2, x1:x2]
                license_plates.append(license_plate)
        
        corrected_plate = rotate_to_horizontal(license_plates[0])
        results = cv_model.predict(corrected_plate)
    
        x1, y1, x2, y2 = results[0].boxes.xyxy[0].cpu().numpy().astype(int)
        final_plate = corrected_plate[y1:y2, x1:x2]
                    
        final_text = recognize_plate_text(final_plate, rec_model)
        
              
        print(f"Распознанный номер: {final_text}")
        print(f"Метаданные: {metadata}")

        return final_text
    except Exception as e:
        print(f"Ошибка распознавания: {e}")
        raise

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
