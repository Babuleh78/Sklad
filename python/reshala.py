import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2' # во избежание предупреждений
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
print(tf.__version__)
from tensorflow import keras
from tensorflow.keras.datasets import mnist
from tensorflow.keras.layers import Dense, Flatten

import tkinter as tk

class DrawingApp:
    def __init__(self, master):
        self.master = master
        master.title("Рисовалко")
        self.frame = tk.Frame(master)
        self.frame.pack(padx=10, pady=10)

        self.canvas = tk.Canvas(self.frame, bg="white", width=150, height=150)
        self.canvas.pack()

        self.last_x, self.last_y = None, None

        self.canvas.bind("<Button-1>", self.start_draw)
        self.canvas.bind("<B1-Motion>", self.draw)
        self.canvas.bind("<ButtonRelease-1>", self.reset)
        self.clear_button = tk.Button(master, text="Очистить", command=self.clear_canvas)
        self.clear_button.pack(pady=5)

    def start_draw(self, event):
        self.last_x, self.last_y = event.x, event.y

    def draw(self, event):
        if self.last_x is not None and self.last_y is not None:
            self.canvas.create_line(self.last_x, self.last_y, event.x, event.y, fill="black", width=2)
    
        self.last_x, self.last_y = event.x, event.y

    def reset(self, event):
        self.last_x, self.last_y = None, None

    def clear_canvas(self):
        self.canvas.delete("all")

if __name__ == "__main__":
    root = tk.Tk()
    app = DrawingApp(root)
    root.mainloop()
#(x_train, y_train), (x_test, y_test) = mnist.load_data()

# x_train = x_train / 255.0
# x_test = x_test / 255.0


# y_train_category = keras.utils.to_categorical(y_train, 10)
# y_test_category = keras.utils.to_categorical(y_test, 10)


# model = keras.Sequential([
#     Flatten(input_shape=(28, 28, 1)),
#     Dense(100, activation = 'relu'),
#     Dense(10, activation = 'softmax')
# ])

# model.compile(optimizer = 'Adam', loss = 'categorical_crossentropy', metrics = ['accuracy'])

# model.fit(x_train, y_train_category, batch_size = 32, epochs = 3, validation_split = 0.2)

# model.evaluate(x_test, y_test_category)

# n = 0

# x = np.expand_dims(x_test[n], 0)
# res = model.predict(x)
# print(res)
# print(np.argmax(res))

# plt.imshow(x_test[n], cmap= plt.cm.binary)
# plt.show()
# print("Завершили")