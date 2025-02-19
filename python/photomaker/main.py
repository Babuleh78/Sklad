import tkinter as tk
from PIL import Image, ImageTk
from readdata import get_data
from writedata import Pixel
import sys
our_data = get_data()

def find_closest(number):
    global our_data 
    closest = our_data[0]  
    closest_num = 0
    min_diff = abs(number - closest)  

    for i in range(len(our_data)):
        value = our_data[i]
        diff = abs(number - value)  
        if diff < min_diff:  
            min_diff = diff 
            closest_num = i

    return closest_num
root = tk.Tk()
root.title("Трепещи перри утконос")
root.geometry("1000x1000")


user_image_path = 'user_photo.jpg'
user_image = Image.open(user_image_path) # то, что скинул пользователь
width, height = user_image.size

WIDTH_CUT  = 64
HEIGHT_CUT = 64
SMALL_WIDTH = width // WIDTH_CUT
SMALL_HEIGHT = height // HEIGHT_CUT
#NUMPIXELS = len(user_image)//WIDTH_CUT//HEIGHT_CUT
small_images = []

if(WIDTH_CUT*HEIGHT_CUT!=len(our_data)):
    print("Ошибка с данным")
    sys.exit(1)
# p = Pixel(our_data[0]['r'], our_data[0]['g'], our_data[0]['b'])
# print(p)

# Список для хранения ссылок на изображения
photo_images = []
closest_numbers = []
for i in range(HEIGHT_CUT): 
    for j in range(WIDTH_CUT):  
        left = j * SMALL_WIDTH 
        upper = i * SMALL_HEIGHT 
        right = left + SMALL_WIDTH 
        lower = upper + SMALL_HEIGHT 
        
        small_image = user_image.crop((left, upper, right, lower))
        
        tk_image_small = ImageTk.PhotoImage(small_image) 

        photo_images.append(tk_image_small)

        pixels = list(small_image.getdata())
        red_sum = 0
        green_sum = 0
        blue_sum = 0
        for r, g, b in pixels:
            red_sum += r
            green_sum += g
            blue_sum += b
        
        sum = red_sum+blue_sum+green_sum
        closest_numbers.append(find_closest(red_sum))

print(len(photo_images))
for i in range(HEIGHT_CUT):
    for j in range(WIDTH_CUT):
        tk_image_small = photo_images[closest_numbers[i*WIDTH_CUT+j]]
        label = tk.Label(root, image=tk_image_small)  
        label.place(x=j * SMALL_WIDTH, y=i * SMALL_HEIGHT) 

root.mainloop()
