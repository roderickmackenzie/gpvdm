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

## @package gl_render_obj
#  This does the rendering of objects
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
from triangle import vec

class gl_render_obj:

	def __init__(self):
		self.stars=[]

	def draw_stars(self):

		if len(self.stars)==0:
			
			for i in range(0,1000):
				phi = random.uniform(0,2*pi)
				costheta = random.uniform(-1,1)
				theta = acos( costheta )
				r=70+random.uniform(0,300)
				x = r * sin( theta) * cos( phi )
				y = r * sin( theta) * sin( phi )
				z = r * cos( theta )
				color=random.uniform(0,1.0)
				r=color
				g=color
				b=color
				s=random.uniform(1,3)	
				self.stars.append([x,y,z,r,g,b,s])
		
			self.stars.append([x,4,z,0.5,0.0,0.0,5])


		for i in range(0,len(self.stars)):
			glPointSize(self.stars[i][6])

			#glMaterialfv(GL_FRONT, GL_DIFFUSE, col)
			glColor4f(self.stars[i][3],self.stars[i][4],self.stars[i][5], 1.0)

			glBegin(GL_POINTS)
			glVertex3f(self.stars[i][0],self.stars[i][1],self.stars[i][2])
			glEnd()
			#glVertex3f(-1.0,-1.0,0.0)
		

	def raw_ray(self,o):
		glLineWidth(5)
		self.set_color(o)
		glBegin(GL_LINES)
		glVertex3f(o.x, o.y, o.z)
		glVertex3f(o.x+o.dx, o.y+o.dy, o.z+o.dz)
		glEnd()

	def paint_ball(self,o):
		for xyz in o.xyz:
			glPushMatrix()
			quad=gluNewQuadric()
			glTranslatef(xyz.x,xyz.y,xyz.z)
			self.set_color(o)
			gluSphere(quad,o.dxyz.x,32,32)
			glPopMatrix()

	def paint_marker(self,o):
		for xyz in o.xyz:
			glPushMatrix()
			quad=gluNewQuadric()
			glTranslatef(xyz.x+o.dxyz.x,xyz.y,xyz.z)
			self.set_color(o)
			gluSphere(quad,0.1,10,10)
			glPopMatrix()

	def paint_marker_small(self,o):
		for xyz in o.xyz:
			glPushMatrix()
			quad=gluNewQuadric()
			glTranslatef(xyz.x+o.dxyz.x,xyz.y,xyz.z)
			self.set_color(o)
			gluSphere(quad,0.025,10,10)
			glPopMatrix()

	def paint_arrow(self,o):
		width=0.04
		leng=1.0
		self.set_color(o)
		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)
			quad=gluNewQuadric()
			glRotatef(o.rotate_y, 0.0, 1.0, 0)
			glRotatef(o.rotate_x, 1.0, 0.0, 0)
			gluCylinder(quad, width, width, leng, 10, 1)
			glTranslatef(0.0,0.0,leng)
			gluCylinder(quad, 0.1, 0.00, 0.2, 10, 1)
			glPopMatrix()

	def paint_line(self,o):
		glLineWidth(1)
		self.set_color(o)
		for xyz in o.xyz:
			glBegin(GL_LINES)
			glVertex3f(xyz.x, xyz.y, xyz.z)
			glVertex3f(xyz.x+o.dxyz.x, xyz.y+o.dxyz.y, xyz.z+o.dxyz.z)
			glEnd()

	def paint_resistor(self,o):
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE)
		glMaterialfv(GL_FRONT,GL_SHININESS,50.0)
		glColor4f(o.r,o.g,o.b,0.5)
		glEnable(GL_BLEND)
		glEnableClientState(GL_COLOR_ARRAY)

		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)

			glLineWidth(2)
			#self.set_color(o)
			glBegin(GL_LINES)
			glVertex3f(0.0, 0.0, 0.0)
			glVertex3f(o.dxyz.x, o.dxyz.y, o.dxyz.z)
			glEnd()


			glLineWidth(5)
			glBegin(GL_LINES)
			glVertex3f(o.dxyz.x*0.3, o.dxyz.y*0.3, o.dxyz.z*0.3)
			glVertex3f(o.dxyz.x*0.7, o.dxyz.y*0.7, o.dxyz.z*0.7)
			glEnd()

			glPopMatrix()

	def paint_diode(self,o):
		diode_min=0.7
		diode_max=0.3
		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)
			glLineWidth(2)
			self.set_color(o)
			glBegin(GL_LINES)
			glVertex3f(0.0, 0.0, 0.0)
			glVertex3f(o.dxyz.x, o.dxyz.y, o.dxyz.z)
			glEnd()


			glLineWidth(2)
			glBegin(GL_LINES)
			#arrow btm
			glVertex3f(-0.1, o.dxyz.y*diode_min, 0.0)
			glVertex3f(0.1, o.dxyz.y*diode_min, 0.0)
			#bar top
			glVertex3f(-0.1, o.dxyz.y*diode_max, 0.0)
			glVertex3f(0.1, o.dxyz.y*diode_max, 0.0)

			#arrow left
			glVertex3f(-0.1, o.dxyz.y*diode_min, 0.0)
			glVertex3f(0.0, o.dxyz.y*diode_max, 0.0)

			#arrow right
			glVertex3f(+0.1, o.dxyz.y*diode_min, 0.0)
			glVertex3f(0.0, o.dxyz.y*diode_max, 0.0)

			glEnd()
			glPopMatrix()




	def gl_render_box_lines(self,o):
		self.set_color(o)

		glLineWidth(10)

		w=o.dxyz.x
		h=o.dxyz.y
		d=o.dxyz.z
		#print("l",o.xyz[0].y,o.dxyz.y)
		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)
			glBegin(GL_LINES)

			#btm

			glVertex3f(0.0,0.0,0.0)
			glVertex3f(w,0.0,0.0)

			glVertex3f(w,0.0,0.0)
			glVertex3f(w,0.0,d)

			glVertex3f(w,0.0,d)
			glVertex3f( 0.0, 0.0, d) 


			#
			glVertex3f(0.0,h,0.0)
			glVertex3f(w,h,0.0)


			glVertex3f(w,h,0.0)
			glVertex3f(w,h,d)
			
			glVertex3f(w,h,d)	
			glVertex3f( 0.0, h,d) 

			#right

			glVertex3f(w,0.0,0.0)
			glVertex3f(w,h,0.0)

			glVertex3f(w,h,0.0)
			glVertex3f(w,h,d)

			glVertex3f(w,h,d)	
			glVertex3f(w, 0.0,d) 

			#left

			glVertex3f(0.0,0.0,0.0)
			glVertex3f(0.0,h,0.0)

			glVertex3f(0.0,h,0.0)
			glVertex3f(0.0,h,d)
			
			glVertex3f(0.0,h,d)
			glVertex3f(0.0, 0.0,d) 


		#
			glVertex3f(0.0,0.0,d)
			glVertex3f(w,0.0,d)

			glVertex3f(w,0.0,d)
			glVertex3f(w,h,d)

			glVertex3f(w,h,d)	
			glVertex3f(0.0, h,d) 


			#top
			glVertex3f(0.0,h,0.0)
			glVertex3f(w,h,0.0)

			glVertex3f(w,h,0.0)
			glVertex3f(w,h,d)
			
			glVertex3f(w,h,d)
			glVertex3f(0.0, h,d) 

			glEnd()
			glPopMatrix()




