
import asyncio
import numpy as np
import cv2
from aiokafka import AIOKafkaConsumer
import json
from datetime import datetime

import sys
from pathlib import Path

import os
import matplotlib.pyplot as plt
import cv2
import torch
import torchvision.transforms as transforms
from PIL import Image
import time

sys.path.append(str(Path(__file__).parent.parent))
from models.computer_vision_model.demo import cv_model
from models.recognition_model.model import rec_model

char_to_idx = {'A': 0, 'B': 1, 'E': 2, 'K': 3, 'M': 4, 'H': 5, # Словарь для преобразования символов в признаки
                           'O': 6, 'P': 7, 'C': 8, 'T': 9, 'Y': 10, 'X': 11,
                           '0': 12, '1': 13, '2': 14, '3': 15, '4': 16,
                           '5': 17, '6': 18, '7': 19, '8': 20, '9': 21, '':22} 
idx_to_char = {v: k for k, v in char_to_idx.items()} # Словарь, обратный предыдущему

# Конфигурация
BOOTSTRAP_SERVERS = "localhost:9092"
TOPIC = "testtopic"

def preprocess_plate_image(plate_img):
 
    plate_image = Image.fromarray(plate_img).convert('L')
    
    transform = transforms.Compose([
        transforms.Resize((64, 320)),  
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.5], std=[0.5])  
    ])
   
    image = transform(plate_image).unsqueeze(0)
    
    return image

def decode_predictions(preds, idx_to_char): # Преобразование предсказаний модели в понятный результат
    preds = preds.permute(1, 0, 2)  # [B, T, C] → [T, B, C]
    probs = torch.softmax(preds, dim=2)
    decoded = []
    
    for batch in probs:
        chars = []
        prev_char = None
        for t in range(batch.shape[0]):
            char_idx = torch.argmax(batch[t]).item()
            if char_idx == 22:  # blank
                prev_char = None
                continue
            if char_idx != prev_char:
                chars.append(idx_to_char[char_idx])
            prev_char = char_idx
        decoded.append("".join(chars))
    
    return decoded

def recognize_plate_text(plate_img, rec_model):
    global idx_to_char
    input_tensor = preprocess_plate_image(plate_img)
    
    with torch.no_grad():
        outputs = rec_model(input_tensor)
    
    plate_text = decode_predictions(outputs, idx_to_char)[0]
    
    return plate_text

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
        print(len(license_plates))

        plate_text = recognize_plate_text(license_plates[0], rec_model)
        
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
