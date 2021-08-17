# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2012-2017 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#
#   https://www.gpvdm.com
#   Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
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
# 

## @package gl_view_point
#  The gl_view_point class for the OpenGL display.
#

import sys
from math import fabs

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb
	from PyQt5.QtGui import QFont


except:
	pass

from PyQt5.QtCore import QTimer

from gl_scale import scale_get_start_x
from gl_scale import scale_get_start_z
from gl_base_object import gl_base_object
from triangle import vec
from gl_scale import scale_get_device_y

class gl_cords():
	def draw_cords(self):
		if self.false_color==True:
			return

		#thse issue is that you should be using QOpenGLWidget and do the text rendering by hand
		#font = QFont("Arial")
		#font.setPointSize(18)

		width=0.04
		leng=1.0
		start_x=scale_get_start_x()-2.0
		start_z=scale_get_start_z()-2.0
		start_y=1.0
		quad=gluNewQuadric()


		glPushMatrix()
		glTranslatef(start_x,start_y,start_z)
		self.render_text (leng+0.2,0.0,0.0, "x")
		glColor4f(0.7,0.7,0.7, 1.0)
		quad=gluNewQuadric()
		glRotatef(90, 0.0, 1.0, 0.0)
		gluCylinder(quad, width, width, leng, 10, 1)
		glTranslatef(0.0,0.0,leng)
		gluCylinder(quad, 0.1, 0.00, 0.2, 10, 1)
		#if self.view.zoom<20:
		glPopMatrix()


		glPushMatrix()
		glTranslatef(start_x,start_y,start_z)
		self.render_text (0.0,leng+0.4,0.0, "y")
		glColor4f(0.7,0.7,0.7, 1.0)
		quad=gluNewQuadric()
		#glTranslatef(0.0,0.0,0.0)
		glRotatef(270, 1.0, 0.0, 0.0)
		gluCylinder(quad, width, width, leng, 10, 1)
		glTranslatef(0.0,0.0,leng)
		gluCylinder(quad, 0.1, 0.00, 0.2, 10, 1)
		#if self.view.zoom<20:
		glPopMatrix()


		glPushMatrix()
		glTranslatef(start_x,start_y,start_z)
		self.render_text (-0.0,0.0,leng+0.2, "z")
		glColor4f(0.7,0.7,0.7, 1.0)
		quad=gluNewQuadric()
		glRotatef(0, 0.0, 1.0, 0.0)
		gluCylinder(quad, width, width, leng, 10, 1)
		glTranslatef(0.0,0.0,leng)
		gluCylinder(quad, 0.1, 0.00, 0.2, 10, 1)

		gluSphere(quad,0.08,32,32)
		#if self.view.zoom<20:
		glPopMatrix()

		#self.draw_numbers()

	def draw_numbers(self):

		#font = QFont("Arial")
		#font.setPointSize(18)


		self.render_text (0.0,0.0,0.0, "(0,0,0)")
		self.render_text (1.0,0.0,0.0, "(1,0,0)")
		self.render_text (0.0,2.0,0.0, "(0,1,0)")
		self.render_text (0.0,0.0,1.0, "(0,0,1)")

	def gl_objects_add_grid(self):
		o=gl_base_object()
		o.id=["grid"]
		o.type="solid_and_mesh"

		y_pos=scale_get_device_y()
		start_x=0
		stop_x=20.0+18.0
		start_z=0
		stop_z=20.0+18.0

		xyz=vec()
		xyz.x=-18.0
		xyz.z=-18.0
		xyz.y=y_pos
		o.xyz.append(xyz)

		n=int(stop_x-start_x)
		dx=1.0#(stop_x-start_x)/n
		pos=start_x

		for i in range(0,n+1):
			o.triangles.append([start_x, 0.0, pos])
			o.triangles.append([stop_x, 0.0, pos])
			pos=pos+dx


		dz=1.0
		pos=start_z
		for i in range(0,n+1):
			o.triangles.append([pos, 0.0, start_z])
			o.triangles.append([pos, 0.0, stop_z])
			pos=pos+dz

		self.gl_objects_add(o)
		self.objects[-1].compile("lines",[0.8,0.8,0.8,1.0],[self.objects[-1].r_false,self.objects[-1].g_false,self.objects[-1].b_false],line_width=1)



