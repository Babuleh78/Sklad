
import asyncio
import numpy as np
import cv2
from aiokafka import AIOKafkaConsumer
import json
from datetime import datetime

# Конфигурация
BOOTSTRAP_SERVERS = "localhost:9092"
TOPIC = "testtopic"

# Моки моделей
class MockCVModel:
    async def predict(self, img):
        return [type('obj', (object,), {
            'boxes': type('obj', (object,), {
                'xyxy': lambda: type('obj', (object,), {
                    'cpu': lambda: type('obj', (object,), {
                        'numpy': lambda: np.array([[10, 10, 100, 100]])
                    })()
                })()
            })()
        })]

class MockRecModel:
    async def recognize(self, img):
        return "A123TEST"

cv_model = MockCVModel()
rec_model = MockRecModel()

async def recognize_plate_from_photo(photo_data: bytes, metadata: dict):
    try:
        nparr = np.frombuffer(photo_data, np.uint8)
        img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        
        detection_results = await cv_model.predict(img)
        plate_text = await rec_model.recognize(img)
        
        print(f"Распознанный номер: {plate_text}")
        print(f"Метаданные: {metadata}")
        
        cv2.imshow("Processed Image", img)
        cv2.waitKey(1) 
        
        return plate_text
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
