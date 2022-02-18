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

## @package gl_input
#  The mouse and keyboard input to the OpenGL display.
#

import sys
open_gl_ok=False

import sys
from math import fabs

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb

except:
	pass

import os
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import Qt
from PyQt5 import QtGui
import time
from cal_path import get_fonts_path
from PIL import Image as Image
import numpy
from PIL import ImageDraw, ImageFont

class gl_image_item():

	def __init__(self,image_path):
		self.image_path=image_path
		self.texture=None
		self.x=None
		self.y=None

	def render(self):

		#img = Image.new('RGBA', (100, 100), (100, 0, 0, 0))

		self.x=100
		self.y=100

		#draw = ImageDraw.Draw(img)
		#canvas.save("unicode-text.png", "PNG")
		#canvas.show()
		#img.save('test.png', 'PNG')
		img = Image.open(self.image_path)
		img_data = numpy.array(list(img.getdata()), numpy.int8)
		textID = glGenTextures(1)
		glBindTexture(GL_TEXTURE_2D, textID)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.size[0], img.size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data)
		self.texture=textID

class gl_image():

	def __init__(self):
		self.image_lib=[]

	def clear_lib(self):
		self.image_lib=[]

	def get_image(self,image_path):
		for img in self.image_lib:
			if img.image_path==image_path:
				return img

		img=gl_image_item(image_path)
		img.render()
		self.image_lib.append(img)
		return img

	def image(self,obj):
		x=obj.xyz[0].x
		y=obj.xyz[0].y
		z=obj.xyz[0].z

		dx=obj.dxyz.x
		dy=obj.dxyz.y
		dz=obj.dxyz.z

		glColor4f(0.0, 0.0, 0.0, 1.0)

		glEnable(GL_BLEND)
		glEnable(GL_TEXTURE_2D)
		#glDepthMask(GL_TRUE)
		obj_img=self.get_image(obj.image_path)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
		glBindTexture(GL_TEXTURE_2D, obj_img.texture)


		size=0.4

		glBegin(GL_QUADS)
		glTexCoord(0,0) 
		if dy==0.0:
			glVertex3f(x, y, z)

			glTexCoord(1,0) 
			glVertex3f(x+dx, y, z)

			glTexCoord(1,1)
			glVertex3f(x+dx, y, z+dz)

			glTexCoord(0,1)

			glVertex3f(x, y, z+dz)

		elif dz==0.0:
			glVertex3f(x, y, z)

			glTexCoord(1,0) 
			glVertex3f(x+dx, y, z)

			glTexCoord(1,1)
			glVertex3f(x+dx, y+dy, z)

			glTexCoord(0,1)

			glVertex3f(x, y+dy, z)
		elif dx==0.0:
			glVertex3f(x, y, z)

			glTexCoord(1,0) 
			glVertex3f(x, y, z+dz)

			glTexCoord(1,1)
			glVertex3f(x, y+dy, z+dz)

			glTexCoord(0,1)

			glVertex3f(x, y+dy, z)
		glEnd()

		#gluDeleteQuadric(qobj)
		glDisable(GL_TEXTURE_2D)
		#glDepthMask(GL_TRUE)
		glDisable(GL_BLEND)

	def gl_render_image(self,o):
		self.image(o)


