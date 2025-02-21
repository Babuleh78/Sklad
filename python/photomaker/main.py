import tkinter as tk
from PIL import Image, ImageTk
from readdata import get_data

import sys
our_data = get_data()

def find_closest(r, g, b):
    global our_data 
    global images
    min_diff = abs(our_data[0][0] - r)+abs(our_data[0][1] - g)+abs(our_data[0][2] - b)
    for i in range(len(our_data)):
        value = our_data[i]
        diff = abs(value[0] - r)+abs(value[1] - g)+abs(value[2] - b)
        if diff < min_diff:  
            min_diff = diff 
            closest_num = i
    return closest_num

def choose_one(a, b, c):#r ,g , b
    global our_data
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
root.geometry("1100x1100")

user_image_path = 'user_photo.jpg'
user_image = Image.open(user_image_path) # то, что скинул пользователь
user_image = user_image.resize((1024, 1024))
width, height = user_image.size

WIDTH_CUT  = 32
HEIGHT_CUT = 32
SMALL_WIDTH = width // WIDTH_CUT
SMALL_HEIGHT = height // HEIGHT_CUT

images = []

for i in range(128):
    image = Image.open(f"albums/{i+1}.jpg")
    image = image.resize((SMALL_WIDTH, SMALL_HEIGHT))
    
    tk_image = ImageTk.PhotoImage(image) 

    images.append(tk_image)
     
  


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
        
     

        closest = find_closest(r, g, b)
        

        closest_numbers.append(closest)

for i in range(HEIGHT_CUT):
    for j in range(WIDTH_CUT):
        index = i * WIDTH_CUT + j  
        if index < len(closest_numbers):  
            tk_image_small = images[closest_numbers[index]]
            label = tk.Label(root, image=tk_image_small, bd=0, highlightthickness=0)  
            label.place(x=j * SMALL_WIDTH, y=i * SMALL_HEIGHT)
root.mainloop()
