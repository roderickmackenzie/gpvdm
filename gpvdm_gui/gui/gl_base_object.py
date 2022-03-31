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

## @package gl_base_object
#  Save the OpenGL scene.
#

import sys
import glob
from triangle import vec
import numpy as np
import copy

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
except:
	pass
from json_base import json_base

class gl_base_object(json_base):
	def __init__(self,x=0.0,y=0.0,z=0.0,dx=0.0,dy=0.0,dz=0.0,r=1.0,g=1.0,b=1.0):
		json_base.__init__(self,"gl_base_object",segment_class=True,segment_example=vec())
		self.segments_name=["xyz","sub_objects"]
		self.segment_name=["xyz","sub_object"]
		self.id=[]
		self.var_list.append(["type",""])
		self.var_list.append(["r",r])
		self.var_list.append(["g",g])
		self.var_list.append(["b",b])
		self.var_list.append(["r_false",None])
		self.var_list.append(["g_false",None])
		self.var_list.append(["b_false",None])
		self.var_list.append(["alpha",0.5])
		self.var_list.append(["selected",False])
		self.var_list.append(["selectable",False])
		self.var_list.append(["moveable",False])
		self.var_list.append(["allow_cut_view",False])
		self.var_list.append(["text",""])
		self.var_list.append(["rotate_y",0.0])
		self.var_list.append(["rotate_x",0.0])
		self.var_list.append(["dxyz",vec(name="dxyz")])
		self.var_list_build()

		self.dxyz.x=-1.0
		self.dxyz.y=-1.0
		self.dxyz.z=-1.0

		self.sub_objects=[]
		self.triangles=[]
		self.points=[]
		self.texture=None

		self.gl_array_float32=[]
		self.gl_array_points=[]
		#self.gl_array_false_colors_float32=[]
		self.gl_array_line_width=[]
		self.blocks=[]

		#remove later maybe used for circuit
		self.gl_array_done=False
		a=copy.deepcopy(self)
		self.segment_examples=[vec(),a]

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
		print("\n".join(self.gen_json()))
		print(self.triangles)

	def id_starts_with(self,val):

		for id in self.id:
			if id.startswith(val)==True:
				return id

		return False

	def __str__(self):
		ret=""
		ret="\n".join(self.gen_json())
		return ret

	class code_block():
		def __init__(self):
			self.gl_array_type=None
			self.gl_array_points=0
			self.gl_array_colors_float32=[]
			self.gl_array_float32=[]
			self.gl_line_width=[]
			#self.gl_array_false_colors_float32=[]

	def compile(self,gl_render_type,color,line_width=3,slice_plane_x=-1e6,slice_plane_y=-1e6):
		self.points=[]
		colors=[]
		false_colors=[]
		false_color=[self.r_false,self.g_false,self.b_false]
		points_per_tri=3
		array_colors=False
		if len(color)>4:
			array_colors=True

		b=self.code_block()
		if gl_render_type=="triangles_solid":
			b.gl_array_type=GL_TRIANGLES
			points_per_tri=3
			for t in self.triangles:
				if t.xyz0.x>slice_plane_x and t.xyz0.y>slice_plane_y:
					self.points.append([t.xyz0.x,t.xyz0.y,t.xyz0.z])
					colors.append(color)
					false_colors.append(false_color)
					self.points.append([t.xyz1.x,t.xyz1.y,t.xyz1.z])
					colors.append(color)
					false_colors.append(false_color)
					self.points.append([t.xyz2.x,t.xyz2.y,t.xyz2.z])
					colors.append(color)
					false_colors.append(false_color)
		elif gl_render_type=="triangles_open":
			b.gl_array_type=GL_LINES
			points_per_tri=6
			for t in self.triangles:
				if t.xyz0.x>slice_plane_x and t.xyz0.y>slice_plane_y:
					self.points.append([t.xyz0.x,t.xyz0.y,t.xyz0.z])
					colors.append(color)
					false_colors.append(false_color)
					self.points.append([t.xyz1.x,t.xyz1.y,t.xyz1.z])
					colors.append(color)
					false_colors.append(false_color)

					self.points.append([t.xyz1.x,t.xyz1.y,t.xyz1.z])
					colors.append(color)
					false_colors.append(false_color)
					self.points.append([t.xyz2.x,t.xyz2.y,t.xyz2.z])
					colors.append(color)
					false_colors.append(false_color)

					self.points.append([t.xyz2.x,t.xyz2.y,t.xyz2.z])
					colors.append(color)
					false_colors.append(false_color)
					self.points.append([t.xyz0.x,t.xyz0.y,t.xyz0.z])
					colors.append(color)
					false_colors.append(false_color)
		elif gl_render_type=="lines":
			b.gl_array_type=GL_LINES
			points_per_tri=1
			pos=0
			for t in self.triangles:
				self.points.append(t)
				if array_colors==True:
					colors.append(color[pos])
				else:
					colors.append(color)
				false_colors.append(false_color)
				pos=pos+1

		b.gl_line_width=line_width
		b.gl_array_points=len(self.points)#len(self.triangles)*points_per_tri

		b.gl_array_colors_float32=np.array(colors, dtype='float32')
		b.gl_array_float32=np.array(self.points, dtype='float32')
		self.blocks.append(b)

	def build_master_objects(self):
		if self.type=="":
			self.triangles=[]
			colors=[]
			for o in self.sub_objects:
				self.triangles.append([o.xyz[0].x,o.xyz[0].y,o.xyz[0].z])
				colors.append([o.r,o.g,o.b,0.8])
				self.triangles.append([o.xyz[0].x+o.dxyz.x,o.xyz[0].y+o.dxyz.y,o.xyz[0].z+o.dxyz.z])
				colors.append([o.r,o.g,o.b,0.8])

			self.type="from_array"
			xyz=vec()
			xyz.x=0.0
			xyz.y=0.0
			xyz.z=0.0
			self.rotate_y=0.0
			self.rotate_x=0.0
			self.dxyz.x=-1.0
			self.dxyz.y=-1.0
			self.dxyz.z=-1.0

			self.xyz=[xyz]
			self.compile("lines",colors)

