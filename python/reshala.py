import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2' # во избежание предупреждений
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
print(tf.__version__)
from tensorflow import keras
from tensorflow.keras.datasets import mnist
from tensorflow.keras.layers import Dense, Flatten


(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train = x_train/255
y_train = y_train/255
#контроль векторов
y_train_category = keras.utils.to_categorical(y_train, 10)
y_test_category = keras.utils.to_categorical(y_test, 10)
# plt.figure(figsize=(10,5))
# for i in range(25):
#     plt.subplot(5, 5, i+1)
#     plt.xticks([])
#     plt.yticks([])
#     plt.imshow(x_train[i], cmap = plt.cm.binary)

# plt.show()


model = keras.Sequential([
    Flatten(input_shape=(28, 28, 1)),
    Dense(100, activation = 'relu'),
    Dense(10, activation = 'softmax')
])

model.compile(optimizer = 'Adam', loss = 'categorical_crossentropy', metrics = ['accuracy'])

model.fit(x_train, y_train_category, batch_size = 25, epochs = 3, validation_split = 0.2)

model.evaluate(x_test, y_test_category)
print("Завершили")