import asyncio
from datetime import datetime
import cv2
from aiokafka import AIOKafkaProducer
import json
from ultralytics import YOLO
import time
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from matplotlib.gridspec import GridSpec
import ipywidgets as widgets
from IPython.display import display
import asyncio
import cv2
import matplotlib.image as mpimg
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import time
from mpl_interactions import panhandler, zoom_factory

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
        finally:
            await producer.stop()



async def video_test():
    kafka_service = KafkaService()

    try:
        video_path = "test_video.mp4"
        processing_interval = 0.25 # Сканируем каждые x секунды
        
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
                        #print(f"Изображение автомобиля успешно отправлено")
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

async def image_test():
    kafka_service = KafkaService()
    
    try:
        images = []
        for i in range(1, 10):
            img_name = f'test_images/car{i}.jpg'
            try:
                img = mpimg.imread(img_name)
                images.append((img, img_name))
            except FileNotFoundError:
                print(f"Файл {img_name} не найден")
                images.append(None)
                continue

        # Функция для отправки изображения
        async def send_image(index):
            if 1 <= index <= 9:
                if images[index-1] is not None:
                    img_data, img_name = images[index-1]
                    _, img_encoded = cv2.imencode('.jpg', cv2.cvtColor(img_data, cv2.COLOR_RGB2BGR))
                    try:
                        for i in range(3):
                            
                                await kafka_service.send_photo(img_encoded.tobytes(), metadata={
                                    'image_name': img_name,
                                    'timestamp': time.time()
                                })
                            
                    except Exception as e:
                        print(f"Ошибка при отправке изображения {index}: {e}")
                    print(f"Изображение {index} ({img_name}) успешно отправлено в Kafka")
                else:
                    print(f"Изображение {index} не загружено (файл не найден)")
            else:
                print("Номер должен быть от 1 до 9")

        while True:
            print("\nДоступные изображения: 1-9")
            print("Введите номер изображения для отправки (или 'q' для выхода):")
            user_input = input().strip()
            
            if user_input.lower() == 'q':
                break
                
            try:
                index = int(user_input)
                await send_image(index)
            except ValueError:
                print("Введите число от 1 до 9 или 'q' для выхода")

    except Exception as e:
        print(f"Ошибка в image_test: {e}")
        raise

async def main():
    #await video_test()
    await image_test()


if __name__ == "__main__":
    asyncio.run(main())
