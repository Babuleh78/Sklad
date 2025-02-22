from main import HEIGHT_CUT, WIDTH_CUT, SMALL_HEIGHT, SMALL_WIDTH
import tkinter as tk
import json
from PIL import Image, ImageTk

root = tk.Tk()
root.title("Трепещи перри утконос")
root.geometry("1024x1024")

images = []

for i in range(170):
    image = Image.open(f"albums/{i+1}.jpg")
    image = image.resize((SMALL_WIDTH, SMALL_HEIGHT))
    tk_image = ImageTk.PhotoImage(image) 
    images.append(tk_image)

with open('images.json', 'r') as f:
        closest_numbers = json.load(f)


for i in range(HEIGHT_CUT):
    for j in range(WIDTH_CUT):
        index = i * WIDTH_CUT + j  
        if index < len(closest_numbers):  
            tk_image_small = images[closest_numbers[index]]
            label = tk.Label(root, image=tk_image_small, bd=0, highlightthickness=0)  
            label.place(x=j * SMALL_WIDTH, y=i * SMALL_HEIGHT)



root.mainloop()
