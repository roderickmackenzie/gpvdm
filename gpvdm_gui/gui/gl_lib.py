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

## @package gl_lib
#  general backend for the OpenGL viewer.
#

import sys

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	open_gl_ok=True
except:
	print("opengl error from gl_lib",sys.exc_info()[0])

import random
import numpy as np
from math import pi,acos,sin,cos

from gl_base_object import gl_base_object

stars=[]

def gl_obj_id_starts_with(ids,val):

	found=False
	for id in ids:
		if id.startswith(val)==True:
			found=True
			break
	return found


def val_to_rgb(v,grey=False):
	if v>1.0:
		v=0.99

	if grey==True:
		return v,v,v

	mesh=[   0.0, 0.5, 1.0]
	colors = [(0, 0, 1.0), (0, 1.0, 0), (1.0, 0, 0)]
	i0=0
	while(1):
		if mesh[i0]<v:
			i0=i0+1
		else:
			i0=i0-1
			break

	i1 = i0+1
	dx=1.0/(len(colors)-1)
	#print(i0,v)

	f=(v-mesh[i0])/dx
	#print(i0)
	#print(i1)
	#print(f)
	#print(i_f)
	#print(v)
	#print(len(colors)-1)
	#print(i)
	#print(f)
	(r0, g0, b0) = colors[i0]
	(r1, g1, b1) = colors[i1]
	#print(i0,i1,f,v,mesh[i0])
	return r0 + f*(r1-r0), g0 + f*(g1-g0), b0 + f*(b1-b0)

