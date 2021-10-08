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

## @package thumb
#  XWindows thumbnail generator
#

import os
import math

from util_zip import archive_add_file
from cal_path import get_sim_path
from win_lin import running_on_linux
if running_on_linux()==True:
	import cairo

def gen_icon(path,icon_size):
	if running_on_linux()==True:
		#surface = cairo.SVGSurface('example1.svg', icon_size, icon_size)
		surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, icon_size, icon_size)
		ctx = cairo.Context(surface)

		ctx.scale(icon_size, icon_size)  # Normalizing the canvas

		pat = cairo.LinearGradient(0.0, 0.0, 0.0, 1.0)
		pat.add_color_stop_rgba(1, 0.7, 0, 0, 0.5)  # First stop, 50% opacity
		pat.add_color_stop_rgba(0, 0.9, 0.7, 0.2, 1)  # Last stop, 100% opacity

		ctx.rectangle(0.25, 0.25, 0.5, 0.5)  # Rectangle(x0, y0, x1, y1)
		ctx.set_source(pat)
		ctx.fill()

		ctx.translate(0.1, 0.1)  # Changing the current transformation matrix

		surface.write_to_png(path)  # Output to PNG
		surface.finish()

def thumb_nail_gen():
	if running_on_linux()==True:
		thumb_dir=os.path.join(get_sim_path(),"thumb")
		if os.path.isdir(thumb_dir)==False:
			os.mkdir(thumb_dir)

		for i in [16,32,48,64,128]:
			icon_path=os.path.join(thumb_dir,str(i)+"x"+str(i)+".png")
			gen_icon(icon_path,i)
			archive_add_file(os.path.join(get_sim_path(),"sim.gpvdm"),icon_path,thumb_dir)

if __name__ == '__main__':
	gen_icon("hello.png",128)

