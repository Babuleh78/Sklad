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

WIDTH_CUT  = 32
HEIGHT_CUT = 32
for i in range(32):
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

    num_pixels = len(pixels)
    average_red = red_sum 
    average_green = green_sum 
    average_blue = blue_sum 
    sum = average_red+average_blue+average_green
        
    all_data.append(red_sum)
  
with open('data.json', 'w') as f:
    json.dump(all_data, f)

print("Конец")
