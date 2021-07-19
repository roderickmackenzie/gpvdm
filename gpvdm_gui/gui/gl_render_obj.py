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

from gl_scale import scale_get_xmul
from gl_scale import scale_get_ymul
from gl_scale import scale_get_zmul

from gl_base_object import gl_base_object


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
		





	def draw_photon(self,x,y,z,up,r,g,b):

		length=0.9
		glColor4f(r, g, b, 1.0)

		glLineWidth(3)
		wx=np.arange(0, length , 0.025)
		wy=np.sin(wx*3.14159*8)*0.2
		
		start_y=y+length
		stop_y=y

		glBegin(GL_LINES)
		for i in range(1,len(wx)):
			glVertex3f(x, start_y-wx[i-1], z+wy[i-1])
			glVertex3f(x, start_y-wx[i], z+wy[i])

		glEnd()

		if up==False:
			glBegin(GL_TRIANGLES)

			glVertex3f(x-0.1, stop_y,z)
			glVertex3f(x+0.1, stop_y ,z)
			glVertex3f(x,stop_y-0.1 ,z)

			glEnd()
		else:
			glBegin(GL_TRIANGLES)

			glVertex3f(x-0.1, start_y,z)
			glVertex3f(x+0.1, start_y ,z)
			glVertex3f(x,start_y+0.1 ,z)

			glEnd()

	def raw_ray(self,o):
		glLineWidth(5)
		self.set_color(o)
		glBegin(GL_LINES)
		glVertex3f(o.x, o.y, o.z)
		glVertex3f(o.x+o.dx, o.y+o.dy, o.z+o.dz)
		glEnd()

	def paint_ball(self,o):
		glPushMatrix()
		quad=gluNewQuadric()
		glTranslatef(o.xyz.x,o.xyz.y,o.xyz.z)
		self.set_color(o)
		gluSphere(quad,o.dxyz.x,32,32)
		glPopMatrix()

	def paint_marker(self,o):
		glPushMatrix()
		quad=gluNewQuadric()
		glTranslatef(o.xyz.x+o.dxyz.x,o.xyz.y,o.xyz.z)
		self.set_color(o)
		gluSphere(quad,0.1,10,10)
		glPopMatrix()

	def paint_marker_small(self,o):
		glPushMatrix()
		quad=gluNewQuadric()
		glTranslatef(o.xyz.x+o.dxyz.x,o.xyz.y,o.xyz.z)
		self.set_color(o)
		gluSphere(quad,0.025,10,10)
		glPopMatrix()

	def paint_line(self,o):
		glLineWidth(1)
		self.set_color(o)
		glBegin(GL_LINES)
		glVertex3f(o.xyz.x, o.xyz.y, o.xyz.z)
		glVertex3f(o.xyz.x+o.dxyz.x, o.xyz.y+o.dxyz.y, o.xyz.z+o.dxyz.z)
		glEnd()

	def paint_resistor(self,o):
		#glPushMatrix()
		#glTranslatef(o.xyz.x,o.xyz.y,o.xyz.z)

		#glLineWidth(2)
		#self.set_color(o)
		if o.gl_array_done==False:
			c=self.cal_color(o)
			#print(c)
			#c=[1.0,0.0,0.0,1.0]
			self.gl_array_lines.append([o.xyz.x+0.0, 			o.xyz.y+0.0, 			o.xyz.z+0.0])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+o.dxyz.x, 		o.xyz.y+o.dxyz.y, 		o.xyz.z+o.dxyz.z])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+o.dxyz.x*0.3, 	o.xyz.y+o.dxyz.y*0.3, 	o.xyz.z+o.dxyz.z*0.3])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+o.dxyz.x*0.7, 	o.xyz.y+o.dxyz.y*0.7, 	o.xyz.z+o.dxyz.z*0.7])
			self.gl_array_colors.append(c)
			o.gl_array_done=True
			#o.gl_array2=np.array(o.gl_array, dtype='float32')
		#glVertexPointer(3, GL_FLOAT, 0, o.gl_array2)
		#glDrawArrays(GL_LINES, 0, 4)

		#glBegin(GL_LINES)
		#glVertex3f(0.0, 0.0, 0.0)
		#glVertex3f(o.dxyz.x, o.dxyz.y, o.dxyz.z)
		#glEnd()


		#glLineWidth(5)
		#glBegin(GL_LINES)
		#glVertex3f(o.dxyz.x*0.3, o.dxyz.y*0.3, o.dxyz.z*0.3)
		#glVertex3f(o.dxyz.x*0.7, o.dxyz.y*0.7, o.dxyz.z*0.7)
		#glEnd()

		#glPopMatrix()

	def paint_diode(self,o):
		diode_max=0.7
		#glPushMatrix()
		#glTranslatef(o.xyz.x,o.xyz.y,o.xyz.z)
		
		#self.set_color(o)
		#glBegin(GL_LINES)
		#glVertex3f(0.0, 0.0, 0.0)
		#glVertex3f(o.dxyz.x, o.dxyz.y, o.dxyz.z)
		#glEnd()

		if o.gl_array_done==False:
			c=self.cal_color(o)
			#c=[1.0,0.0,0.0,1.0]
			self.gl_array_lines.append([o.xyz.x,			o.xyz.y,					o.xyz.z])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+o.dxyz.x, 	o.xyz.y+o.dxyz.y, 			o.xyz.z+o.dxyz.z])
			self.gl_array_colors.append(c)

			self.gl_array_lines.append([o.xyz.x-0.1, 		o.xyz.y+o.dxyz.y*0.3, 		o.xyz.z+0.0])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+0.1, 		o.xyz.y+o.dxyz.y*0.3, 		o.xyz.z+0.0])
			self.gl_array_colors.append(c)

			#bar top
			self.gl_array_lines.append([o.xyz.x-0.1, 		o.xyz.y+o.dxyz.y*diode_max, o.xyz.z+0.0])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+0.1, 		o.xyz.y+o.dxyz.y*diode_max, o.xyz.z+0.0])
			self.gl_array_colors.append(c)

			#arrow left`
			self.gl_array_lines.append([o.xyz.x-0.1, 		o.xyz.y+o.dxyz.y*0.3, 		o.xyz.z+0.0])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+0.0, 		o.xyz.y+o.dxyz.y*diode_max, o.xyz.z+0.0])
			self.gl_array_colors.append(c)

			#arrow right
			self.gl_array_lines.append([o.xyz.x+0.1, 		o.xyz.y+o.dxyz.y*0.3, 		o.xyz.z+0.0])
			self.gl_array_colors.append(c)
			self.gl_array_lines.append([o.xyz.x+0.0, 		o.xyz.y+o.dxyz.y*diode_max, o.xyz.z+0.0])
			self.gl_array_colors.append(c)
			o.gl_array_done=True

		#glBegin(GL_LINES)
		#arrow btm
		#glVertex3f(-0.1, o.dxyz.y*0.3, 0.0)
		#glVertex3f(0.1, o.dxyz.y*0.3, 0.0)
		#bar top
		#glVertex3f(-0.1, o.dxyz.y*diode_max, 0.0)
		#glVertex3f(0.1, o.dxyz.y*diode_max, 0.0)

		#arrow left
		#glVertex3f(-0.1, o.dxyz.y*0.3, 0.0)
		#glVertex3f(0.0, o.dxyz.y*diode_max, 0.0)

		#arrow right
		#glVertex3f(+0.1, o.dxyz.y*0.3, 0.0)
		#glVertex3f(0.0, o.dxyz.y*diode_max, 0.0)

		#glEnd()
		#glPopMatrix()

	def gl_render_box_lines(self,o):
		self.set_color(o)

		glLineWidth(10)

		w=o.dxyz.x
		h=o.dxyz.y
		d=o.dxyz.z

		glPushMatrix()
		glTranslatef(o.xyz.x,o.xyz.y,o.xyz.z)
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


	def gl_render_grid(self,o):
		glLineWidth(1)
		self.set_color(o)
		start_x=-18.0
		stop_x=20.0
		n=int(stop_x-start_x)
		dx=1.0#(stop_x-start_x)/n
		pos=start_x
		glBegin(GL_LINES)
		for i in range(0,n+1):
			glVertex3f(start_x, 0.0, pos)
			glVertex3f(stop_x, 0.0, pos)
			pos=pos+dx


		start_z=-18.0
		stop_z=20.0
		dz=1.0
		pos=start_z
		for i in range(0,n+1):
			glVertex3f(pos, 0.0, start_z)
			glVertex3f(pos, 0.0, stop_z)
			pos=pos+dz

		glEnd()
