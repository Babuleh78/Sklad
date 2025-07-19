import asyncio
from datetime import datetime
import numpy as np
import cv2
from aiokafka import AIOKafkaConsumer, AIOKafkaProducer
import json
import sys
from pathlib import Path
import os

import cv2
import numpy as np

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



async def main():
    kafka_service = KafkaService()
    
    img = np.zeros((480, 640, 3), dtype=np.uint8)
    cv2.putText(img, "TEST PLATE", (50, 50), 
               cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)
    _, img_encoded = cv2.imencode('.jpg', img)
    
    metadata = {
        "filename": "test_plate.jpg",
        "source": "test_script"
    }
    await kafka_service.send_photo(img_encoded.tobytes(), metadata)
  


if __name__ == "__main__":
    asyncio.run(main())
