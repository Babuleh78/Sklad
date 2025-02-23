from writeimages import HEIGHT_CUT, WIDTH_CUT, SMALL_HEIGHT, SMALL_WIDTH
import tkinter as tk
import json
from PIL import Image, ImageTk, ImageDraw
from tkinter import messagebox

root = tk.Tk()
root.title("Мозайка")
root.geometry("1024x1024")

images = []

for i in range(170):
    image = Image.open(f"albums/{i+1}.jpg")
    image = image.resize((SMALL_WIDTH, SMALL_HEIGHT))
    tk_image = ImageTk.PhotoImage(image) 
    images.append(tk_image)

with open('images.json', 'r') as f:
    closest_numbers = json.load(f)

canvas = tk.Canvas(root, width=WIDTH_CUT * SMALL_WIDTH, height=HEIGHT_CUT * SMALL_HEIGHT)
canvas.pack()

for i in range(HEIGHT_CUT):
    for j in range(WIDTH_CUT):
        index = i * WIDTH_CUT + j  
        if index < len(closest_numbers):  
            tk_image_small = images[closest_numbers[index]]
            canvas.create_image(j * SMALL_WIDTH, i * SMALL_HEIGHT, anchor=tk.NW, image=tk_image_small)

def save_canvas():
    image = Image.new("RGB", (WIDTH_CUT * SMALL_WIDTH, HEIGHT_CUT * SMALL_HEIGHT))

    for i in range(HEIGHT_CUT):
        for j in range(WIDTH_CUT):
            index = i * WIDTH_CUT + j  
            if index < len(closest_numbers):  
                tk_image_small = images[closest_numbers[index]]
                # Получаем изображение из tk_image_small
                img = ImageTk.getimage(tk_image_small)
                # Вставляем изображение в новое изображение
                image.paste(img, (j * SMALL_WIDTH, i * SMALL_HEIGHT))

    image.save("mosaic.png")  

save_button = tk.Button(root, text="Сохранить как PNG", command=save_canvas)
save_button.pack()

def on_closing():
    try:
        save_canvas()
    except Exception:
        print(Exception)
    

root.protocol("WM_DELETE_WINDOW", on_closing)
root.mainloop()
