
from PIL import Image, ImageTk
from readdata import get_data
import random
import json


our_data = get_data()

def find_closest(r, g, b):
    global our_data 
    global images
    min_diff = abs(our_data[0][0] - r)+abs(our_data[0][1] - g)+abs(our_data[0][2] - b)
    closest_num = 0
    for i in range(len(our_data)):
        value = our_data[i]
        diff = abs(value[0] - r)+abs(value[1] - g)+abs(value[2] - b)
        if diff < min_diff:  
            min_diff = diff 
            closest_num = i
    make_some_fun(closest_num)
    return closest_num

def make_some_fun(num):
    global our_data
    for i in range(2):
        rand = random.randint(0, 1)
        if rand == 1:
            our_data[num][i] += 3
        else:
            our_data[num][i] -= 3




user_image_path = 'user_photo.jpg'
user_image = Image.open(user_image_path) # то, что скинул пользователь
user_image = user_image.resize((1024, 1024))
width, height = user_image.size

WIDTH_CUT  = 64
HEIGHT_CUT = 64
SMALL_WIDTH = width // WIDTH_CUT
SMALL_HEIGHT = height // HEIGHT_CUT

images = []

for i in range(170):
    image = Image.open(f"albums/{i+1}.jpg")
    image = image.resize((SMALL_WIDTH, SMALL_HEIGHT))

    images.append(image)
     

photo_images = []
closest_numbers = []
for i in range(HEIGHT_CUT): 
    for j in range(WIDTH_CUT):  
        left = j * SMALL_WIDTH 
        upper = i * SMALL_HEIGHT 
        right = left + SMALL_WIDTH 
        lower = upper + SMALL_HEIGHT 
        
        small_image = user_image.crop((left, upper, right, lower))
       

        pixels = list(small_image.getdata())
        red_sum = 0
        green_sum = 0
        blue_sum = 0
        for r, g, b in pixels:
            red_sum += r
            green_sum += g
            blue_sum += b
        
        sum = red_sum+blue_sum+green_sum
        
     

        closest = find_closest(r, g, b)
        

        closest_numbers.append(closest)

with open('images.json', 'w') as f:
    json.dump(closest_numbers, f)


