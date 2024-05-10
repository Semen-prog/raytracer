#!/usr/bin/python3

from PIL import Image
import sys

path = sys.argv[1]
img = Image.open(path)
w, h = img.size
arr = img.load()

f = open("src.ppm", "w")
f.write(f"P3\n{w} {h}\n255\n")

for i in range(h):
    for j in range(w):
        r, g, b = arr[j, i]
        f.write(f"{r} {g} {b}\n")