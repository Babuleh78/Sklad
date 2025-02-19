from PIL import Image, ImageTk

class Pixel:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def get_color(self):
        return (self.r, self.g, self.b)

image = Image.open("main_photo.jpg")

width, height = image.size

WIDTH_CUT  = 4
HEIGHT_CUT = 4
small_width = width // WIDTH_CUT
small_height = height // HEIGHT_CUT
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
        print(red_sum, green_sum, blue_sum)


print("Конец")
