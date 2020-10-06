#!/usr/bin/env python3

## @package gpvdm-thumbnailer
#  Thumbnailer for xwindows
#

import os
import sys
import math
import cairo
import shutil
from PIL import Image
from io import BytesIO
from PIL import ImageDraw
import random

def gen_icon(path,icon_size):
	image = Image.new('RGBA',(icon_size,icon_size))
	draw = ImageDraw.Draw(image)

	for x in range(0,64):
		for y in range(0,64):

			r = random.randrange(255)
			g = random.randrange(255)
			b = random.randrange(255)
			a = random.randrange(255)

			draw.rectangle((x,y,x+1,y+1),fill=(r,g,b,a))

	image.save(path, "PNG")

if __name__ == '__main__':

	print("hello")
	f=open('/home/rod/data.txt', 'a')
	f.write("rod4 "+" ".join(sys.argv)+"\n")
	f.close()

	if len(sys.argv)>1:
		image_size=sys.argv[1]
		input_file=sys.argv[2]
		output_file=sys.argv[3]

		gen_icon(output_file,int(image_size))

	sys.exit(0)
