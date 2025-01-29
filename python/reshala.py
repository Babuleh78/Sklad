import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
print(tf.__version__)
from tensorflow import keras
from tensorflow.keras.datasets import mnist
from tensorflow.keras.layers import Dense, Flatten

# Загружаем данные MNIST
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# Нормализация изображений
x_train = x_train / 255.0
x_test = x_test / 255.0

# Измените форму входных данных с (60000, 28, 28) на (60000, 28, 28, 1)
x_train = x_train.reshape(-1, 28, 28, 1)
x_test = x_test.reshape(-1, 28, 28, 1)

# Преобразование меток в категориальный формат
y_train_category = keras.utils.to_categorical(y_train, 10)
y_test_category = keras.utils.to_categorical(y_test, 10)

# Определение модели
model = keras.Sequential([
    Flatten(input_shape=(28, 28, 1)),
    Dense(128, activation='relu'),
    Dense(10, activation='softmax')
])

# Компиляция модели
model.compile(optimizer='Adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Обучение модели
model.fit(x_train, y_train_category, batch_size=32, epochs=5, validation_split=0.2)

# Оценка модели
test_loss, test_accuracy = model.evaluate(x_test, y_test_category)
print(f"Тестовая точность: {test_accuracy}")
print("Завершили")
