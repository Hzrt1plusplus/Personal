#!/usr/bin/env python3

from PIL import Image
import os 
import numpy as np
import colorama
from io import BytesIO
import requests

alphabets = {
    "dark" : [" ", " ", " ", " ", "_", "^", "*","~",":","!","/", "[", "T", "&", "#", "$", "@"], 
    "normal":  [" ", ".", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "T", "&","$","#","@", "@", "@", "@"], 
    "light" : [".", "-", "_", "0", "1", "2", "3", "T", "J", "@", "@", "@", "#", "#", "#", "#", "#", "&", "&", "&"]
}

alphabet = alphabets["normal"]
SIZE = len(alphabet)
coloring = True

def toColor(r , g, b): 
    return f"\x1b[38;2;{r};{g};{b}m"

def to_char(r, g, b): 
    l = (r + g + b) / 765
    l *= (SIZE - 1)
    return alphabet[int(l)]

def to_Char(l): 
     l = (l / 255)*(SIZE - 1)
     return alphabet[int(l)]


def normalize(image, stride, w, h): 

    w -= w % stride
    h -= h % stride

    horizontal = w // stride
    vertical = h // stride
    buffer = np.array(image, dtype = np.float64)
    colors = []
    img = []
    
    # extract colors and brightness of pixels
    for i in range(vertical): 
        offset1 = i * stride 
        color_buffer = []
        image_buffer = [] 
        for j in range(horizontal): 
            offset2 = j * stride
            b = np.array([0., 0., 0.])
            for z in range(stride): 
                b += np.sum(buffer[offset1 + z][offset2:offset2 + stride], axis = 0)
            b /= (stride * stride)
            image_buffer.append(int(b.mean()))
            b = b.astype(int)
            color_buffer.append(toColor(*b))

        img.append(image_buffer)
        colors.append(color_buffer)

    return img, colors

RATIO = 3
PROGRAM_HEADER = """ 

------------------------------------------------------------------
        Welcome to PWYW(print whatever you want)!
------------------------------------------------------------------

Commands: 

    color <on/off>          turn on/off coloring
    mode <modename>         available modes: normal, light, dark
    filename                file at the current folder
    link                    valid image link from internet 
    exit                    for quitting
    help                    for info

------------------------------------------------------------------

"""

print(colorama.Style.BRIGHT)
print(PROGRAM_HEADER)
while True: 

    print(colorama.Fore.RESET)
    userInput = input("Please enter a command or a filename(or link of image): ")
    userInput = [i.lower() for i in userInput.split()]
    size = len(userInput)

    if size == 0: continue
    elif size == 2 and userInput[0] == "color":
        if userInput[1] == "on": 
            coloring = True
        elif userInput[1] == "off": 
            coloring = False
        else: 
            print(colorama.Style.BRIGHT,colorama.Fore.RED,"Error! No such option!",colorama.Fore.RESET,sep="")

    elif size == 2 and userInput[0] == "mode": 
        if userInput[1] in alphabets: 
            alphabet = alphabets[userInput[1]]
            SIZE = len(alphabet)
        else: 
            print(colorama.Style.BRIGHT, colorama.Fore.RED, "Error! No such mode!", colorama.Fore.RESET, sep = "")
    
    elif size == 1: 

        #breakpoint()

        filename = userInput[0]
        if filename == "": continue
        if filename == "help": 
            print(PROGRAM_HEADER)
            continue
        elif filename == "exit": break 

        try: 

            if filename.find("https://") != -1: 
                urlData = requests.get(filename)
                filename = BytesIO(urlData.content)

            with Image.open(filename) as img: 

                terminal_w = os.get_terminal_size().columns 
                terminal_w -= terminal_w%RATIO
                terminal_w //= RATIO

                img = img.convert("RGB")
                w, h = img.size 

                STRIDE = (w // terminal_w) + bool(w % terminal_w)
               
                img, colors = normalize(img, STRIDE, w, h)
                vert = (h - h%STRIDE) // STRIDE
                horz = (w - w%STRIDE) // STRIDE
                for i in range(vert): 
                    t = ""
                    if coloring:
                        for j in range(horz): 
                            t += colors[i][j]
                            t += RATIO*to_Char(img[i][j])    
                    else: 
                        for j in range(horz): 
                            t += RATIO*to_Char(img[i][j])
                    print(t)

        except Exception as e: 

            print(colorama.Style.BRIGHT,colorama.Fore.RED,"Error! No such image in current directory or link is not valid image!",colorama.Fore.RESET,sep="")
    
    else: 
        print(colorama.Style.BRIGHT,colorama.Fore.RED,"Error! No such command!",colorama.Fore.RESET,sep="")


print(colorama.Fore.RESET)
