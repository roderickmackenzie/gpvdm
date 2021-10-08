#!/usr/bin/env python3
# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2008-2022 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#   
#   https://www.gpvdm.com
#   
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License v2.0, as published by
#   the Free Software Foundation.
#   
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#   

## @package old-thumb
#  Old thumbnail generation for xwindows - delete?
#


import os
import sys
import math
import cairo
import shutil

def gen_icon(path,icon_size):


	#surface = cairo.SVGSurface('example1.svg', icon_size, icon_size)
	surface = cairo.ImageSurface(cairo.FORMAT_RGB24, icon_size, icon_size)
	ctx = cairo.Context(surface)

	ctx.scale(icon_size, icon_size)  # Normalizing the canvas

	#pat = cairo.LinearGradient(0.0, 0.0, 0.0, 1.0)

	#pat.add_color_stop_rgb(1, 0.7, 0, 0)  # First stop, 50% opacity
	#pat.add_color_stop_rgb(0, 0.9, 0.7, 0.2)  # Last stop, 100% opacity

	ctx.rectangle(0.25, 0.25, 0.5, 0.5)  # Rectangle(x0, y0, x1, y1)
	ctx.set_source_rgb (1.0, 0, 0);
	ctx.fill()

	ctx.translate(0.1, 0.1)  # Changing the current transformation matrix

	surface.write_to_png(path)  # Output to PNG
	surface.write_to_png("/home/rod/out.png")

	surface.finish()
	#os.system("convert -resize "+str(icon_size)+"x"+str(icon_size)+" "+path)
	#os.system("cp /home/rod/Desktop/one.png "+path)
	#shutil.copyfile("/home/rod/Desktop/a.png", path)

if __name__ == '__main__':

	f=open('/home/rod/data.txt', 'a')
	f.write("rod2 "+" ".join(sys.argv)+"\n")
	f.close()

	if len(sys.argv)>1:
		image_size=sys.argv[1]
		input_file=sys.argv[2]
		output_file=sys.argv[3]


		#shutil.copyfile("/home/rod/Desktop/a.png", output_file)
		gen_icon(output_file,int(image_size))

		sys.exit(0)
	sys.exit(0)




