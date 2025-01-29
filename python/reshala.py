import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2' # во избежание предупреждений
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
print(tf.__version__)
from tensorflow import keras
from tensorflow.keras.datasets import mnist
from tensorflow.keras.layers import Dense, Flatten
from PIL import Image, ImageDraw
import tkinter as tk

class DrawingApp:
    def __init__(self, master, model):  
        self.master = master
        self.model = model  ь
        master.title("Рисовалка")

        master.geometry("400x400")

        self.canvas_size = 140
        self.canvas = tk.Canvas(master, bg="white", width=self.canvas_size, height=self.canvas_size)
        self.canvas.pack(padx=10, pady=10)

        self.image = Image.new("L", (self.canvas_size, self.canvas_size), 255)  # Создаем белое изображение
        self.draw = ImageDraw.Draw(self.image)

        self.last_x, self.last_y = None, None
        self.canvas.bind("<Button-1>", self.start_draw)
        self.canvas.bind("<B1-Motion>", self.draw_line)
        self.canvas.bind("<ButtonRelease-1>", self.reset)

        self.clean_button = tk.Button(master, text="Очистить", command=self.clean_canvas)
        self.clean_button.pack(pady = 5)
        self.convert_button = tk.Button(master, text="Преобразовать в матрицу", command=self.convert_to_matrix)
        self.convert_button.pack(pady=5)

    def clean_canvas(self):
        self.canvas.delete("all")
    def start_draw(self, event):
        self.last_x, self.last_y = event.x, event.y

    def draw_line(self, event):
        if self.last_x is not None and self.last_y is not None:
            
            self.canvas.create_line(self.last_x, self.last_y, event.x, event.y, fill="black", width=2)
            
            self.draw.line([self.last_x, self.last_y, event.x, event.y], fill=0, width=2)
        self.last_x, self.last_y = event.x, event.y

    def reset(self, event):
        self.last_x, self.last_y = None, None

    def convert_to_matrix(self):
        small_image = self.image.resize((28, 28), Image.LANCZOS)
        data = np.array(small_image)

  
        norm_data = 1 - (data / 255.0)
        norm_data = np.expand_dims(norm_data, axis=(0, -1))  

        # Предсказание
        res = self.model.predict(norm_data)
        predicted_class = np.argmax(res)

        print(f"Предсказанная цифра: {predicted_class}")

if __name__ == "__main__":
    root = tk.Tk()

    # Загрузка и подготовка данных
    (x_train, y_train), (x_test, y_test) = mnist.load_data()
    x_train = x_train / 255.0
    x_test = x_test / 255.0
    y_train_category = keras.utils.to_categorical(y_train, 10)
    y_test_category = keras.utils.to_categorical(y_test, 10)

    # Создание модели
    model = keras.Sequential([
        Flatten(input_shape=(28, 28, 1)),
        Dense(100, activation='relu'),
        Dense(10, activation='softmax')
    ])

    model.compile(optimizer='Adam', loss='categorical_crossentropy', metrics=['accuracy'])

    # Обучение модели
    model.fit(x_train, y_train_category, batch_size=32, epochs=3, validation_split=0.2)

    

    app = DrawingApp(root, model)  # Передаем модель в приложение
    root.mainloop()