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

## @package gl_base_object
#  Save the OpenGL scene.
#

import sys
import glob
from triangle import vec
import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *

class gl_base_object():
	def __init__(self,x=0.0,y=0.0,z=0.0,dx=0.0,dy=0.0,dz=0.0,r=1.0,g=1.0,b=1.0):
		self.id=[]
		self.type=""
		self.xyz=[]

		self.dxyz=vec()
		self.dxyz.x=dx
		self.dxyz.y=dy
		self.dxyz.z=dz

		self.r=r
		self.g=g
		self.b=b

		self.r_false=None
		self.g_false=None
		self.b_false=None

		self.alpha=0.5
		self.selected=False
		self.selectable=False
		self.moveable=False
		self.allow_cut_view=True
		self.triangles=[]
		self.points=[]
		self.text=""
		self.texture=None

		self.gl_array_types=[]
		self.gl_array_float32=[]
		self.gl_array_colors_float32=[]
		self.gl_array_points=[]
		self.gl_array_false_colors_float32=[]

		self.rotate_y=0.0
		self.rotate_x=0.0

	def copy(self,obj):
		self.id=obj.id
		self.type=obj.type

		self.xyz=[]
		self.xyz.extend(obj.xyz)

		self.dxyz.cpy(obj.dxyz)

		self.r=obj.r
		self.g=obj.g
		self.b=obj.b

		self.r_false=obj.r_false
		self.g_false=obj.g_false
		self.b_false=obj.b_false

		self.alpha=obj.alpha
		self.selected=obj.selected
		self.selectable=obj.selectable
		self.moveable=obj.moveable
		self.allow_cut_view=obj.allow_cut_view
		self.triangles=obj.triangles
		self.points=obj.points

		self.rotate_y=obj.rotate_y
		self.rotate_x=obj.rotate_x

	def match_false_color(self,r,g,b):
		rf=int(self.r_false*255)
		gf=int(self.g_false*255)
		bf=int(self.b_false*255)

		ri=int(r*255)
		gi=int(g*255)
		bi=int(b*255)
		if ri==rf and gi==gf and bi==bf:
			return True

		return False

	def dump(self):
		print(self.id)
		print(self.type)
		for v in self.xyz:
			print("xyz: "+str(v))
		print("dx: "+str(self.xyz.dx))
		print("dy: "+str(self.xyz.dy))
		print("dz: "+str(self.xyz.dz))
		print(self.r)
		print(self.g)
		print(self.b)
		print(self.alpha)
		print(self.selected)
		print(self.selectable)
		print(self.moveable)
		print(self.triangles)

	def id_starts_with(self,val):

		for id in self.id:
			if id.startswith(val)==True:
				return id

		return False

	def __str__(self):
		ret=""
		ret=ret+self.type

		for v in self.xyz:
			ret=ret+";;"+str(v.x)
			ret=ret+";;"+str(v.y)
			ret=ret+";;"+str(v.z)

		ret=ret+";;"+str(self.dxyz.x)
		ret=ret+";;"+str(self.dxyz.y)
		ret=ret+";;"+str(self.dxyz.z)

		ret=ret+";;"+str(self.r)
		ret=ret+";;"+str(self.g)
		ret=ret+";;"+str(self.b)

		ret=ret+";;"+str(self.alpha)
		ret=ret+";;"+str(self.selected)
		ret=ret+";;"+str(self.selectable)
		ret=ret+";;"+str(self.moveable)
		ret=ret+";;"+str(self.allow_cut_view)

		ret=ret+";;"+str(self.text)


		return ret

	def compile(self,gl_render_type,color,false_color):
		self.gl_array_types.append(gl_render_type)
		points=[]
		colors=[]
		false_colors=[]
		points_per_tri=3
		if gl_render_type==GL_TRIANGLES:
			points_per_tri=3
			for t in self.triangles:
				points.append([t.xyz0.x,t.xyz0.y,t.xyz0.z])
				colors.append(color)
				false_colors.append(false_color)
				points.append([t.xyz1.x,t.xyz1.y,t.xyz1.z])
				colors.append(color)
				false_colors.append(false_color)
				points.append([t.xyz2.x,t.xyz2.y,t.xyz2.z])
				colors.append(color)
				false_colors.append(false_color)
		elif gl_render_type==GL_LINES:
			points_per_tri=6
			for t in self.triangles:
				points.append([t.xyz0.x,t.xyz0.y,t.xyz0.z])
				colors.append(color)
				false_colors.append(false_color)
				points.append([t.xyz1.x,t.xyz1.y,t.xyz1.z])
				colors.append(color)
				false_colors.append(false_color)

				points.append([t.xyz1.x,t.xyz1.y,t.xyz1.z])
				colors.append(color)
				false_colors.append(false_color)
				points.append([t.xyz2.x,t.xyz2.y,t.xyz2.z])
				colors.append(color)
				false_colors.append(false_color)

				points.append([t.xyz2.x,t.xyz2.y,t.xyz2.z])
				colors.append(color)
				false_colors.append(false_color)
				points.append([t.xyz0.x,t.xyz0.y,t.xyz0.z])
				colors.append(color)
				false_colors.append(false_color)


		self.gl_array_points.append(len(self.triangles)*points_per_tri)
		self.gl_array_float32.append(np.array(points, dtype='float32'))
		self.gl_array_colors_float32.append(np.array(colors, dtype='float32'))
		self.gl_array_false_colors_float32.append(np.array(false_colors, dtype='float32'))


