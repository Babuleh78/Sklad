from writeimages import HEIGHT_CUT, WIDTH_CUT, SMALL_HEIGHT, SMALL_WIDTH
import tkinter as tk
import json
from PIL import Image, ImageTk


root = tk.Tk()
root.title("Мозайка")
root.geometry("300x100")

images = []

for i in range(170):
    image = Image.open(f"albums/{i+1}.jpg")
    image = image.resize((SMALL_WIDTH, SMALL_HEIGHT))
    tk_image = ImageTk.PhotoImage(image) 
    images.append(tk_image)

with open('images.json', 'r') as f:
    closest_numbers = json.load(f)

def save_canvas():
    image = Image.new("RGB", (WIDTH_CUT * SMALL_WIDTH, HEIGHT_CUT * SMALL_HEIGHT))

    for i in range(HEIGHT_CUT):
        for j in range(WIDTH_CUT):
            index = i * WIDTH_CUT + j  
            if index < len(closest_numbers):  
                tk_image_small = images[closest_numbers[index]]
                img = ImageTk.getimage(tk_image_small)
                image.paste(img, (j * SMALL_WIDTH, i * SMALL_HEIGHT))


    image.save("mosaic.png")  
save_canvas()

root.destroy()
