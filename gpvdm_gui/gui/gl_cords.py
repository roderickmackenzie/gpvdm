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

## @package gl_cords
#  The gl_cords class for the OpenGL display.
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

from gl_base_object import gl_base_object
from triangle import vec
from gpvdm_json import gpvdm_data

class gl_cords():

	def draw_cords(self):
		if self.false_color==True:
			return

		if self.scale.world_max==None:
			return

		width=0.04
		leng=1.0
		
		start_x=self.scale.project_m2screen_x(0.0)-2.0
		start_z=self.scale.project_m2screen_z(0.0)-2.0
		start_y=self.scale.project_m2screen_y(self.scale.world_max.y)-1.0#  self.scale.project_m2screen_y(0.0)+1.0
		#quad=gluNewQuadric()


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

	def gl_objects_add_grid(self,x0,x1,y0,y1,z0,z1,color=[0.8,0.8,0.8,1.0],dx=1.0,dz=1.0,dy=1.0,direction="zx"):
		o=gl_base_object()
		o.id=["grid"]
		o.type="solid_and_mesh"

		start_x=0
		start_y=0
		start_z=0

		if x1!=None:
			stop_x=x1-x0
			nx=int((stop_x-start_x)/dx)

		if y1!=None:
			stop_y=y1-y0
			ny=int((stop_y-start_y)/dy)

		if z1!=None:
			stop_z=z1-z0
			nz=int((stop_z-start_z)/dz)

		xyz=vec()
		xyz.x=x0
		xyz.z=z0
		xyz.y=y0

		o.xyz.append(xyz)


		if direction=="zx":
			pos=start_z
			while pos<=stop_z:
				o.triangles.append([start_x, 0.0, pos])
				o.triangles.append([stop_x, 0.0, pos])
				pos=pos+dz

			pos=start_x
			while pos<=stop_x:
				o.triangles.append([pos, 0.0, start_z])
				o.triangles.append([pos, 0.0, stop_z])
				pos=pos+dx

		elif direction=="zy":
			pos=start_z
			while pos<=stop_z:
				o.triangles.append([0.0, start_y, pos])
				o.triangles.append([0.0, stop_y, pos])
				pos=pos+dz

			pos=start_y
			while pos<=stop_y:
				o.triangles.append([0.0, pos, start_z])
				o.triangles.append([0.0, pos, stop_z])
				pos=pos+dy

		elif direction=="xy":
			pos=start_x
			while pos<=stop_x:
				o.triangles.append([pos, start_y, 0.0])
				o.triangles.append([pos, stop_y, 0.0])
				pos=pos+dx

			pos=start_y
			while pos<=stop_y:
				o.triangles.append([start_x, pos, 0.0])
				o.triangles.append([stop_x, pos, 0.0])
				pos=pos+dy

		self.gl_objects_add(o)
		self.objects[-1].compile("lines",color,line_width=1)
		return self.objects[-1]



	def world_box(self):
		self.gl_objects_remove_regex("world_box")
		a=gl_base_object()
		a.type="solid_and_mesh"
		a.id=["world_box"]

		xyz=vec()
		xyz.x=self.scale.project_m2screen_x(self.scale.world_min.x)
		xyz.y=self.scale.project_m2screen_y(self.scale.world_min.y)
		xyz.z=self.scale.project_m2screen_z(self.scale.world_min.z)
		a.xyz.append(xyz)

		a.dxyz.x=fabs(self.scale.world_max.x-self.scale.world_min.x)*self.scale.x_mul
		a.dxyz.y=fabs(self.scale.world_max.y-self.scale.world_min.y)*self.scale.y_mul
		a.dxyz.z=fabs(self.scale.world_max.z-self.scale.world_min.z)*self.scale.z_mul

		a.r=1.0
		a.g=0.0
		a.b=0.0

		a.alpha=1.0

		a.triangles.extend(self.default_shapes.box.data)

		self.gl_objects_add(a)
		self.objects[-1].compile("triangles_open",[1.0,0.0,0.0,1.0],line_width=5)

