import tkinter as tk
from PIL import Image, ImageTk
from readdata import get_data

import sys
our_data = get_data()
print(len(our_data))
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

def choose_one(a, b, c):

    ab = abs(a - b)
    ac = abs(a - c)
    bc = abs(b - c)

    if ab <= ac and ab <= bc:
        return min(a, b)  
    elif ac <= ab and ac <= bc:
        return min(a, c)  
    else:
        return min(b, c)  
    


root = tk.Tk()
root.title("Трепещи перри утконос")
root.geometry("1400x1000")

images = []
for i in range(32):
    image = Image.open(f"albums/{i+1}.jpg")
    image = image.resize((32, 32))
    
    tk_image = ImageTk.PhotoImage(image) 

    images.append(tk_image)
     
  

user_image_path = 'user_photo.jpg'
user_image = Image.open(user_image_path) # то, что скинул пользователь
user_image = user_image.resize((1024, 1024))
width, height = user_image.size

WIDTH_CUT  = 32
HEIGHT_CUT = 32
SMALL_WIDTH = width // WIDTH_CUT
SMALL_HEIGHT = height // HEIGHT_CUT
#NUMPIXELS = len(user_image)//WIDTH_CUT//HEIGHT_CUT
small_images = []

# if(WIDTH_CUT*HEIGHT_CUT!=len(our_data)):
#     print("Ошибка с данным")
#     sys.exit(1)
# p = Pixel(our_data[0]['r'], our_data[0]['g'], our_data[0]['b'])
# print(p)

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


        pixels = list(small_image.getdata())
        red_sum = 0
        green_sum = 0
        blue_sum = 0
        for r, g, b in pixels:
            red_sum += r
            green_sum += g
            blue_sum += b
        
        sum = red_sum+blue_sum+green_sum
        
        closest_number_r = find_closest(red_sum)
        closest_number_b = find_closest(blue_sum)
        closest_number_g = find_closest(green_sum)
        #print(closest_number_r, red_sum)
        #our_elephant = choose_one(closest_number_r, closest_number_g, closest_number_b)
        
        closest_numbers.append(closest_number_r)


print(len(closest_numbers))
print("Изо", len(images))
for i in range(HEIGHT_CUT):
    for j in range(WIDTH_CUT):
        index = i * WIDTH_CUT + j  # Вычисляем индекс для closest_numbers
        if index < len(closest_numbers):  # Проверяем, что индекс не выходит за пределы
            print(index)
            tk_image_small = images[closest_numbers[index]]
            label = tk.Label(root, image=tk_image_small)  
            label.place(x=j * SMALL_WIDTH, y=i * SMALL_HEIGHT) 
root.mainloop()
