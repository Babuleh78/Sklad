import tkinter as tk
from PIL import Image, ImageTk

root = tk.Tk()
root.title("Трепещи перри утконос")
root.geometry("1000x1000")

image_path = 'main_photo.jpg' 
image = Image.open(image_path)

user_image_path = 'user_photo.jpg'
user_image = Image.open(user_image_path)
width, height = image.size

WIDTH_CUT  = 4
HEIGHT_CUT = 4
small_width = width // WIDTH_CUT
small_height = height // HEIGHT_CUT
small_images = []

pixels = list(image.getdata())
red_sum = 0
green_sum = 0
blue_sum = 0

# Подсчитываем суммы значений для каждого канала
for r, g, b in pixels:
    red_sum += r
    green_sum += g
    blue_sum += b
print(red_sum, green_sum, blue_sum)
# Список для хранения ссылок на изображения
photo_images = []

for i in range(HEIGHT_CUT): 
    for j in range(WIDTH_CUT):  
        left = j * small_width
        upper = i * small_height
        right = left + small_width
        lower = upper + small_height
        
        small_image = image.crop((left, upper, right, lower))
        
        tk_image_small = ImageTk.PhotoImage(small_image) 

        photo_images.append(tk_image_small)
        
        label = tk.Label(root, image=tk_image_small)  
        label.place(x=j * small_width, y=i * small_height) 

root.mainloop()