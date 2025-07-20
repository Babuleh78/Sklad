import asyncio
from datetime import datetime
import cv2
from aiokafka import AIOKafkaProducer
import json
from pathlib import Path

from ultralytics import YOLO
import time

BOOTSTRAP_SERVERS = "localhost:9092"
TOPIC = "testtopic"



class KafkaService:
    def __init__(self):
        self.bootstrap_servers = BOOTSTRAP_SERVERS

    async def send_photo(self, photo_data: bytes, metadata: dict):
        producer = AIOKafkaProducer(
            bootstrap_servers=self.bootstrap_servers,
            value_serializer=lambda v: json.dumps(v).encode('utf-8')
        )
        await producer.start()
        try:
            message = {
                "photo": photo_data.hex(),
                "metadata": metadata,
                "timestamp": datetime.now().isoformat()
            }
            await producer.send(TOPIC, message)
            print("Изображение отправлено в Kafka")
        finally:
            await producer.stop()



async def send_real_image():
    kafka_service = KafkaService()

    try:
        video_path = "test_video.mp4"
        processing_interval = 0.5 # Сканируем каждые x секунды
        
        car_detection_model = YOLO('yolov8n.pt') # Consumer будет получать вырезанную машину (в реальности вряд ли пригодится)
        
        cap = cv2.VideoCapture(video_path)
        if not cap.isOpened():
            print("Ошибка открытия видеофайла")
            return
        
        frame_count = 0
        last_processing_time = time.time()
        
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                break
            
            frame_count += 1
            current_time = time.time()

            if current_time - last_processing_time < processing_interval:
                continue
                
            last_processing_time = current_time
            frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            
        
            car_results = car_detection_model(frame_rgb, classes=[2, 5, 7], verbose=False)
            
            for car_result in car_results: # Передаем каждую машину
                car_boxes = car_result.boxes.xyxy.cpu().numpy()
                for car_box in car_boxes:
                    x1, y1, x2, y2 = map(int, car_box[:4])
                    
                    car_img = frame[y1:y2, x1:x2]
                    
                    if car_img.size == 0:
                        continue
                    
                    _, img_encoded = cv2.imencode('.jpg', car_img)
                    
                    try:
                        await kafka_service.send_photo(img_encoded.tobytes(), metadata={
                            'timestamp': current_time
                        })
                        print(f"Изображение автомобиля успешно отправлено")
                    except Exception as e:
                        print(f"Ошибка при отправке в Kafka: {e}")
            
            cv2.imshow('Shashura', frame)
            
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        
        cap.release()
        cv2.destroyAllWindows()
        
    except Exception as e:
        print(f"Ошибка при обработке видео: {e}")
        raise

async def main():
    await send_real_image()


if __name__ == "__main__":
    asyncio.run(main())
