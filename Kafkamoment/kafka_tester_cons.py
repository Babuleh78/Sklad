
import asyncio
import numpy as np
import cv2
from aiokafka import AIOKafkaConsumer
import json
import sys
from pathlib import Path
import time
import datetime
import time

sys.path.append(str(Path(__file__).parent.parent))
from models.computer_vision_model.demo import cv_model
from models.recognition_model.model import rec_model
from models.demonstration import recognize_plate_text
from models.demonstration import rotate_to_horizontal

from database.database import Database
from database.redis import RedisCache

# Конфигурация
BOOTSTRAP_SERVERS = "localhost:9092"
TOPIC = "testtopic"

class ParkingSystem:
    def __init__(self, redis_cache, db):
        self.redis = redis_cache.redis
        self.db = db
        self.PARKING_PREFIX = "parking:"
        self.TIMESTAMPS_ZSET = "parking_timestamps"
        self.MAX_ITEMS = 5
        self.TTL_SECONDS = 120

    async def print_all_redis_data(self): # Вывод состояния парковки (не использую, тк больше мешает)
        print("\nТекущее состояние парковки:")
        current_time = time.time()
        all_cars = self.redis.zrange(self.TIMESTAMPS_ZSET, 0, -1, withscores=True)
        
        for plate_number, entry_time in all_cars:
            parked_seconds = current_time - entry_time
            car_data = self.redis.hgetall(f"{self.PARKING_PREFIX}{plate_number}")
            owner_info = f"{car_data.get('name', '')} {car_data.get('surname', '')}".strip()
            print(f"- {plate_number}: {owner_info or 'не указано'}, на парковке {parked_seconds:.2f} сек")

    async def manage_car_plate(self, plate_number: str): # Обработка пришедшего номера
        current_time = time.time()
        await self.cleanup_expired_cars(current_time) # Очищаем просроченные данные 
        
        if self.redis.zscore(self.TIMESTAMPS_ZSET, plate_number) is not None: # Проверяем наличие машины в парковке
            return await self.process_car_exit(plate_number, current_time) # Если кэш есть, значит машина точно выезжает (простояла недолго)
        else:
            return await self.process_car_entry_or_long_exit(plate_number, current_time)

    async def cleanup_expired_cars(self, current_time: float): # Очистка "старых" машин из кеша
     
        expired = self.redis.zrangebyscore(
            self.TIMESTAMPS_ZSET,
            min=0,
            max=current_time - self.TTL_SECONDS
        )
        
        if expired:
            # Обновляем статус в PostgreSQL
            async with self.db.pool.acquire() as conn:
                await conn.execute(
                    f"UPDATE parking_cars SET is_on_parking = FALSE "
                    f"WHERE plate_number = ANY($1::text[])",
                    expired
                )
            
            # Удаляем из Redis
            pipeline = self.redis.pipeline()
            for plate in expired:
                pipeline.delete(f"{self.PARKING_PREFIX}{plate}")
                pipeline.zrem(self.TIMESTAMPS_ZSET, plate)
            pipeline.execute()
            
            print(f"Автоматически выгружены: {', '.join(expired)}")

    async def process_car_exit(self, plate_number: str, current_time: float): # Обработка выезда
       
        car_data = self.redis.hgetall(f"{self.PARKING_PREFIX}{plate_number}")
        entry_time = float(self.redis.zscore(self.TIMESTAMPS_ZSET, plate_number))
        parking_time = current_time - entry_time
        
        print("\nПолная информация о машине (из кэша):")
        (f"Номер: {plate_number}")
        (f"Владелец: {car_data.get('name', 'не указано')} {car_data.get('surname', 'не указано')}")
        (f"Время стоянки: {parking_time:.2f} сек")
        
        # Удаляем из Redis
        pipeline = self.redis.pipeline()
        pipeline.delete(f"{self.PARKING_PREFIX}{plate_number}")
        pipeline.zrem(self.TIMESTAMPS_ZSET, plate_number)
        pipeline.execute()
        
        return parking_time

    async def process_car_entry_or_long_exit(self, plate_number: str, current_time: float) -> None: # Обработка въезда или выезда спустя много времени (подумай над названием, а)
       
        async with self.db.pool.acquire() as conn:
            car_info = await conn.fetchrow(
                "SELECT * FROM parking_cars WHERE plate_number = $1",
                plate_number
            )
            
            if not car_info: # Потом какя-нибудь адекватная обработка будет
                print(f"\nМашина {plate_number} не найдена в базе данных")
                return None
                
            if car_info['is_on_parking']:  # Если машина уже отмечена как на парковке - обновляем статус выезда
               
                await conn.execute(
                    "UPDATE parking_cars SET is_on_parking = FALSE "
                    "WHERE plate_number = $1",
                    plate_number
                )
                print(f"\nМашина {plate_number} отмечена как в БД выехавшая")
                entry_time = datetime.datetime.strptime(car_info['entry_time'], "%Y-%m-%d %H:%M:%S").timestamp()
                return current_time - entry_time
            else:
                # Если места нет - удаляем самую старую машину
                if self.redis.zcard(self.TIMESTAMPS_ZSET) >= self.MAX_ITEMS:
                    oldest = self.redis.zrange(self.TIMESTAMPS_ZSET, 0, 0, withscores=True)
                    if oldest:
                        oldest_plate = oldest[0][0]
                        
                        await conn.execute(
                            "UPDATE parking_cars SET is_on_parking = TRUE "
                            "WHERE plate_number = $1",
                            oldest_plate
                        )
                        print(f"\nМашина {oldest_plate} отмечена в БД как заехавшая (в кеше кончилось месте)")

                        pipeline = self.redis.pipeline()
                        pipeline.delete(f"{self.PARKING_PREFIX}{oldest_plate}")
                        pipeline.zrem(self.TIMESTAMPS_ZSET, oldest_plate)
                        pipeline.execute()
                    
                
                # Добавляем новую машину    
                pipeline = self.redis.pipeline()
                pipeline.hset(
                    f"{self.PARKING_PREFIX}{plate_number}",
                    mapping={
                        'name': car_info.get('name', ''),
                        'surname': car_info.get('surname', '')
                    }
                )
                pipeline.zadd(self.TIMESTAMPS_ZSET, {plate_number: current_time})
                pipeline.execute()
                
                print(f"\nМашина {plate_number} добавлена в кэш")
        
        return None


async def recognize_plate_from_photo(photo_data: bytes, metadata: dict, ps):

        global plate_confirmation, last_detection_time
        
        if 'plate_confirmation' not in globals():
            plate_confirmation = {}  # Словарь для хранения состояний номеров
            last_detection_time = time.time()  # Время последней детекции
        
        CONFIRMATION_COUNT = 3  # Необходимое количество подтверждений
        CONFIRMATION_TIMEOUT = 3.0  # Максимальное время между подтверждениями (сек
        
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
            })
            
            if current_time - current_state['last_seen'] < CONFIRMATION_TIMEOUT:
                current_state['count'] += 1
            else:
                current_state['count'] = 1  
                
            current_state['last_seen'] = current_time
            
            if current_state['count'] >= CONFIRMATION_COUNT:
                
                plate_confirmation[clean_plate_text] = current_state
                # Пошел redis
                parking_time = await ps.manage_car_plate(clean_plate_text)
            
                if parking_time is not None:
                    print(f"Машина с номером {clean_plate_text} простояла {parking_time:.2f} секунд и выехала")
                else:

                    print(f"Машина с номером {clean_plate_text} заехала на парковку")
                
                #await ps.print_all_redis_data()
                return clean_plate_text
            else:
                #print(f"Предварительное распознавание: {clean_plate_text} (подтверждений: {current_state['count']}/{CONFIRMATION_COUNT})")
                plate_confirmation[clean_plate_text] = current_state
                return None
                
        except Exception as e:
            print(f"Ошибка распознавания: {e}")
            return None

async def consume_and_process_photos(ps):
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
                    
                    await recognize_plate_from_photo(photo_data, metadata, ps)
                    
                except Exception as e:
                    print(f"Ошибка обработки сообщения: {e}")
                    
        finally:
            await consumer.stop()
            cv2.destroyAllWindows()

async def main():
        db = Database()
        redis_cache = RedisCache(
                host="localhost",
                port=6379,
                db=0
        )
        

        try:
            redis_cache.redis.ping()
            print("Успешно подключено к Redis")

            await db.connect(
                user='postgres',
                password='1234',
                host='localhost',
                port=5432,
                database='recognize_plate'
            )

            print("Успешно подключено к Postgres")

            parking_system = ParkingSystem(redis_cache, db)
            await consume_and_process_photos(parking_system)
        except Exception as e:
            print(f"Критическая ошибка: {e}")

if __name__ == "__main__":
        asyncio.run(main())
