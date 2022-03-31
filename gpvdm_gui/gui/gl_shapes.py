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


from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtOpenGL
from PyQt5.QtOpenGL import QGLWidget
open_gl_ok=True


	
import random
import numpy as np
from math import pi,acos,sin,cos

from gl_lib import val_to_rgb
from triangle_io import triangles_get_min
from triangle_io import triangles_get_max
from triangle import triangle

class gl_shapes:
	def pyrmid(self,o):

		dx=o.dxyz.dx
		dy=o.dxyz.dy
		dz=o.dxyz.dz
		segs=40
		delta=180/segs
		theata=0
		theta_rad=(theata/360)*2*3.141592653
		self.set_color(o)

		#top

		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)

			glBegin(GL_TRIANGLES)
			glVertex3f(0.0,0.0,0.0)
			glVertex3f(dx,0.0,0.0)
			glVertex3f(dx/2,dy,dz/2)
			glEnd()

			glBegin(GL_TRIANGLES)
			glVertex3f(dx,0.0,0.0)
			glVertex3f(dx,y,dz)
			glVertex3f(dx/2,dy,dz/2)
			glEnd()

			glBegin(GL_TRIANGLES)
			glVertex3f(dx,0.0,dz)
			glVertex3f(0.0,0.0,dz)
			glVertex3f(dx/2,dy,dz/2)
			glEnd()

			glBegin(GL_TRIANGLES)
			glVertex3f(0.0,0.0,0.0)
			glVertex3f(0.0,0.0,dz)
			glVertex3f(dx/2,dy,dz/2)
			glEnd()
			glPopMatrix()


	def paint_from_array(self,o):
		glEnableClientState(GL_VERTEX_ARRAY)
		if self.false_color==True:
			glColor3f(o.r_false,o.g_false,o.b_false)
		else:
			glEnable(GL_BLEND)
			glEnableClientState(GL_COLOR_ARRAY)

		for xyz in o.xyz:
			glPushMatrix()
			
			#glScalef(1.0, 1.0, 1.0) 
			#print(o.id,xyz.x,xyz.y,xyz.z)
			glTranslatef(xyz.x,xyz.y,xyz.z)

			if o.dxyz.x!=-1:
				glScaled(o.dxyz.x,o.dxyz.y,o.dxyz.z)

			glTranslatef(1.0/2.0,1.0/2.0,1.0/2.0)
			glRotatef(o.rotate_y, 0.0, 1.0, 0)
			glRotatef(o.rotate_x, 1.0, 0.0, 0)
			glTranslatef(-1.0/2.0,-1.0/2.0,-1.0/2.0)

			n=0

			for b in o.blocks:
				if b.gl_line_width!=None:
					glLineWidth(b.gl_line_width)
				#print(o.gl_array_float32)
				glVertexPointer(3, GL_FLOAT, 0, b.gl_array_float32)

				#print(b.gl_array_float32[n])
				if self.false_color==False:
					glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE)
					glMaterialfv(GL_FRONT,GL_SHININESS,100.0)
					glColorPointer(4, GL_FLOAT, 0, b.gl_array_colors_float32)

				glDrawArrays(b.gl_array_type, 0, b.gl_array_points)
				n=n+1

			glPopMatrix()

		glDisableClientState(GL_VERTEX_ARRAY)

		if self.false_color==False:
			glDisableClientState(GL_COLOR_ARRAY)
			glDisable(GL_BLEND)


	def paint_open_triangles_from_array(self,o,false_color=True,line_width=5):
		

		dx=o.dxyz.x/2
		dy=o.dxyz.y/2
		dz=o.dxyz.z/2
		alpha=0.5
		beta=0.5
		glLineWidth(line_width)

		if o.r==None:
			o.r=1.0
			o.g=0.0
			o.b=0.0

		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)

			glBegin(GL_LINES)

			n=1.0
			for t in o.triangles:
				if false_color==True:
					self.set_color(o,r_override=o.r*alpha+o.r*beta*n, g_override=o.g*alpha+o.g*beta*n, b_override=o.b*alpha+o.b*beta*n)
				else:
					self.set_color(o)

				glVertex3f(t.xyz0.x,t.xyz0.y,t.xyz0.z)
				glVertex3f(t.xyz1.x,t.xyz1.y,t.xyz1.z)

				if t.points==3:

					glVertex3f(t.xyz1.x,t.xyz1.y,t.xyz1.z)
					glVertex3f(t.xyz2.x,t.xyz2.y,t.xyz2.z)

					glVertex3f(t.xyz2.x,t.xyz2.y,t.xyz2.z)
					glVertex3f(t.xyz0.x,t.xyz0.y,t.xyz0.z)

					n=n+0.01
					if n>1.0:
						n=0.0
			glEnd()
			glPopMatrix()

	def half_cyl(self,o,colored=False):
		
		r=dx/2
		x=x0+r
		y=y0
		z=z0
		alpha=0.2
		segs=40
		delta=180/segs
		theata=0
		theta_rad=(theata/360)*2*3.141592653

		self.set_color(o)

		#top
		while (theata<180):


			glBegin(GL_QUADS)
			dx=r*cos(theta_rad)
			dy=dy0*sin(theta_rad)

			glVertex3f(x+dx,y+dy,z+0.0)
			glVertex3f(x+dx,y+dy,z+dz)

			theata=theata+delta			
			theta_rad=(theata/360)*2*3.141592653

			dx=r*cos(theta_rad)
			dy=dy0*sin(theta_rad)

			glVertex3f(x+dx,y+dy,z+dz)
			glVertex3f(x+dx,y+dy,z)

			glEnd()

		theata=0
		theta_rad=(theata/360)*2*3.141592653

		self.set_color(o)

		while (theata<180):


			glBegin(GL_TRIANGLES)
			dx=r*cos(theta_rad)
			dy=dy0*sin(theta_rad)

			glVertex3f(x+dx,y+dy,z)

			theata=theata+delta			
			theta_rad=(theata/360)*2*3.141592653

			dx=r*cos(theta_rad)
			dy=dy0*sin(theta_rad)

			glVertex3f(x+dx,y+dy,z)

			glVertex3f(x,y,z)

			glEnd()

		theata=0
		theta_rad=(theata/360)*2*3.141592653

		while (theata<180):


			glBegin(GL_TRIANGLES)
			dx=r*cos(theta_rad)
			dy=dy0*sin(theta_rad)

			glVertex3f(x+dx,y+dy,z+dz)

			theata=theata+delta			
			theta_rad=(theata/360)*2*3.141592653

			dx=r*cos(theta_rad)
			dy=dy0*sin(theta_rad)

			glVertex3f(x+dx,y+dy,z+dz)

			glVertex3f(x,y,z+dz)

			glEnd()


	def box(self,o,cut_through=False):

		#btm
		dx=o.dxyz.x
		dy=o.dxyz.y
		dz=o.dxyz.z

		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)

			self.set_color(o)

			glBegin(GL_QUADS)
			if cut_through==False:
				glVertex3f(0.0,	0.0,	0.0)
				glVertex3f(dx,	 0.0,	0.0)
				glVertex3f(dx,	 0.0,	dz)
				glVertex3f(0.0,	0.0,	 dz) 
			
			#top
			if cut_through==False:
				glVertex3f(0.0,	 dy,		 0.0)
				glVertex3f(dx, dy,	 0.0)
				glVertex3f(dx,	 dy,	 dz)
				glVertex3f( 0.0,	 dy,	 dz) 

			#right

			glVertex3f(dx,	0.0,			0.0)
			glVertex3f(dx,	dy,	0.0)
			glVertex3f(dx,	dy,	dz)
			glVertex3f(dx,	0.0,			dz) 

			#left
			if cut_through==False:
				glVertex3f(0.0,0.0,			0.0)
				glVertex3f(0.0, dy,	0.0)
				glVertex3f(0.0, dy,	dz)
				glVertex3f(0.0, 0.0,		dz) 
			
			#back
			glVertex3f(0.0,0.0,dz)
			glVertex3f(dx,0.0,dz)
			glVertex3f(dx,dy,dz)
			glVertex3f(0.0, dy,dz) 

			#front
			if cut_through==False:
				glVertex3f(0.0,0.0,0.0)
				glVertex3f(0.0, dy,0.0)
				glVertex3f(dx,dy,0.0)
				glVertex3f(dx, 0.0,0.0)

			glEnd()
			glPopMatrix()


	def plane(self,o):
		dx=o.dxyz.x
		dy=o.dxyz.y
		dz=o.dxyz.z

		self.set_color(o)

		for xyz in o.xyz:
			glPushMatrix()
			glTranslatef(xyz.x,xyz.y,xyz.z)

			glBegin(GL_QUADS)

			glVertex3f(0.0, 0.0, 0.0)
			glVertex3f(dx, 0.0, 0.0)

			glVertex3f(dx, dy, 0.0)
			glVertex3f(0.0, dy, 0.0)

			glEnd()
			glPopMatrix()

