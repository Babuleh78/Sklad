import tkinter as tk
from PIL import Image, ImageTk


root = tk.Tk()
root.title("Трепещи перри утконос")

image_path = 'main_photo.jpg' 
image = Image.open(image_path)


width, height = image.size

WIDTH_CUT  = 4
HEIGHT_CUT = 4
small_width = width // WIDTH_CUT
small_height = height // HEIGHT_CUT
small_images = []


for i in range(WIDTH_CUT):  
    for j in range(HEIGHT_CUT): 
        left = j * small_width
        upper = i * small_height
        right = left + small_width
        lower = upper + small_height
        
        small_image = image.crop((left, upper, right, lower))
        small_images.append(small_image)


tk_small_images = []

for s_i in small_images:
    tk_image_small = ImageTk.PhotoImage(s_i)  
    tk_small_images.append(tk_image_small)  
    label = tk.Label(root, image=tk_image_small)  
    label.pack()

root.mainloop()