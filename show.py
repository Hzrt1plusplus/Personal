from PIL import Image

filename = input("Please enter the filename: ")
alphabet = [" ", "-", "_", "~", "^","*","T", "%", "#", "@"]
SIZE = len(alphabet)

def to_char(r, g, b): 
    l = (r + g + b) / 765
    l *= (SIZE - 1)
    return alphabet[int(l)]

with Image.open(filename) as img: 

    img = img.convert("RGB")
    w, h = img.size 

    for i in range(h): 
        t = ""
        for j in range(w): 
            c = to_char(*img.getpixel((j,i)))
            t += 3*c
        print(t)
