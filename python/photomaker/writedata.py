from PIL import Image
import json

class Pixel:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def to_dict(self):
        return {'r': self.r, 'g': self.g, 'b': self.b}

all_data = []
image = Image.open("main_photo.jpg")

width, height = image.size

WIDTH_CUT  = 4
HEIGHT_CUT = 4
small_width = width // WIDTH_CUT
small_height = height // HEIGHT_CUT

red_sum = 0
green_sum = 0
blue_sum = 0



for i in range(HEIGHT_CUT): 
    for j in range(WIDTH_CUT):  
        left = j * small_width
        upper = i * small_height
        right = left + small_width
        lower = upper + small_height
        
        small_image = image.crop((left, upper, right, lower))
        
        red_sum = 0
        green_sum = 0
        blue_sum = 0

        pixels = list(small_image.getdata())

        for r, g, b in pixels:
            red_sum += r
            green_sum += g
            blue_sum += b

        num_pixels = len(pixels)
        average_red = red_sum // num_pixels
        average_green = green_sum // num_pixels
        average_blue = blue_sum // num_pixels

        p = Pixel(average_red, average_green, average_blue)
        all_data.append(p.to_dict())  


with open('data.json', 'w') as f:
    json.dump(all_data, f)

print("Конец")
