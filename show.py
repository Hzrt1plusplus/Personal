#!/usr/bin/env python3

from PIL import Image
import os 
import numpy as np
import colorama
from io import BytesIO
import requests


colors = {
    "red" : colorama.Fore.RED, 
    "blue" : colorama.Fore.BLUE, 
    "yellow" : colorama.Fore.YELLOW, 
    "green" : colorama.Fore.GREEN,
    "cyan" : colorama.Fore.CYAN
}


#alphabet = [" ", "-", "_", "~", "^","*","T", "%", "#", "@"]
alphabet = [" ", ".","-", "_", "~", "^","*","/","[","]","T", "%", "#", "@"]
SIZE = len(alphabet)
COLORIZE = False

def to_char(r, g, b): 
    l = (r + g + b) / 765
    l *= (SIZE - 1)
    return alphabet[int(l)]

def to_Char(l): 
     l = (l / 255)*(SIZE - 1)
     return alphabet[int(l)]


def normalize(image, stride, w, h): 
   
    buffer = np.array(image, dtype = np.float64)
    buffer = buffer.mean(axis = 2)

    
    if w%stride != 0: 
        tmp  = []
        pad = np.array([0 for i in range(stride - (w%stride))])
        for i in range(h): 
            tmp.append(np.append(buffer[i], pad))
        buffer = np.array(tmp)

    h -= h%stride
    n = h // stride
    m = w // stride
    
    res = []
    for  i in range(n): 
        t = []
        offset1 = stride*i
        for j in range(m): 
            b = 0.
            offset2 = stride*j
            for z in range(stride):
                b += np.mean(buffer[offset1 + z][offset2:offset2 + stride])
            t.append(b/stride)
        res.append(t)

    return res

RATIO = 3
PROGRAM_HEADER = """ 

------------------------------------------------------------------
        Welcome to PWYW(print whatever you want)!
------------------------------------------------------------------

Commands: 

    color <colornames>      available colors: red, blue, yellow, green, cyan
    filename                file at the current folder
    link                    valid image link from internet 
    exit                    for quitting

------------------------------------------------------------------

"""

print(PROGRAM_HEADER)
currentColor = colorama.Fore.WHITE
while True: 

    userInput = input("Please enter a command or a filename(or link of image): ")
    userInput = [i.lower() for i in userInput.split()]
    size = len(userInput)

    if size == 0: continue
    elif size == 2 and userInput[0] == "color":
        if userInput[1] in colors: 
            currentColor = colors[userInput[1]]
            print(currentColor)
        else: 
            print(colorama.Style.BRIGHT,colorama.Fore.RED,"Error! No such color!",colorama.Fore.RESET,sep="")

    
    elif size == 1: 

        filename = userInput[0]
        if filename == "": continue
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

                if STRIDE > 1:
                    img = normalize(img, STRIDE, w, h)
                    for row in img: 
                        t = ""
                        for val in row: 
                            t += RATIO*to_Char(val)
                        print(t)
                else: 
                    for i in range(h): 
                        t = ""
                        for j in range(w): 
                            c = to_char(*img.getpixel((j,i)))
                            t += RATIO*c
                        print(t)

        except Exception as e: 

            print(colorama.Style.BRIGHT,colorama.Fore.RED,"Error! No such image in current directory or link is not valid image!",colorama.Fore.RESET,sep="")
    
    else: 
        print(colorama.Style.BRIGHT,colorama.Fore.RED,"Error! No such command!",colorama.Fore.RESET,sep="")
