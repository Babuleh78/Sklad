from PIL import Image, ImageTk
import json

class Pixel:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def to_dict(self):
        return {'r': self.r, 'g': self.g, 'b': self.b}
    def __str__(self):
        return f"Pixel({self.r}, {self.g}, {self.b})"

all_data = []

image = Image.open("main_photo.jpg")

width, height = image.size

for i in range(170):
    image = Image.open(f"albums/{i+1}.jpg")
    image = image.resize((32, 32))
    red_sum = 0
    green_sum = 0
    blue_sum = 0

    pixels = list(image.getdata())

    for r, g, b in pixels:
            red_sum += r
            green_sum += g
            blue_sum += b
    denum = len(pixels)

    all_data.append((red_sum//denum, green_sum//denum, blue_sum//denum))
  

print("Конец")
