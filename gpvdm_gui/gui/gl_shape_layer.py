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

## @package gl_lib
#  general backend for the OpenGL viewer.
#

import sys

from OpenGL.GL import *
from OpenGL.GLU import *

import random
import numpy as np
from math import pi,acos,sin,cos

from gl_scale import scale_get_xmul
from gl_scale import scale_get_ymul
from gl_scale import scale_get_zmul

from gl_base_object import gl_base_object
from triangle import vec
from triangle_io import triangles_mul_vec
from triangle_io import triangles_print
from triangle_io import triangles_add_vec
from triangle_io import triangles_sub_vec
from triangle_io import triangles_get_min

from epitaxy import get_epi

from gl_scale import scale_get_device_x
from gl_scale import scale_get_device_z
from mesh import get_mesh

class shape_layer():

	def shape_to_screen(self,shape0,base_obj=None):
		a=gl_base_object()
		a.moveable=shape0.moveable
		if shape0.shape_enabled==True:
			a.type="solid_and_mesh"
			if self.draw_device_cut_through==True:
					a.type="box_cut_through"
		else:
			a.type="marker"

		a.id=[shape0.id]
		for pos in shape0.expand_xyz0(shape0):
			xyz=vec()
			if base_obj!=None:
				vec_base=vec()
				vec_base.x=base_obj.x0
				vec_base.y=base_obj.y0
				vec_base.z=base_obj.z0
				pos=pos+vec_base
			xyz.x=self.scale.project_m2screen_x(pos.x)
			xyz.y=self.scale.project_m2screen_y(pos.y)
			xyz.z=self.scale.project_m2screen_z(pos.z)
			a.xyz.append(xyz)

		a.dxyz.x=shape0.dx*scale_get_xmul()
		a.dxyz.y=shape0.dy*scale_get_ymul()
		a.dxyz.z=shape0.dz*scale_get_zmul()

		a.r=shape0.color_r
		a.g=shape0.color_g
		a.b=shape0.color_b

		a.alpha=1.0
		if len(shape0.shapes)>0:
			a.alpha=0.5

		a.allow_cut_view=True
		a.selectable=True

		a.rotate_x=shape0.rotate_x
		a.rotate_y=shape0.rotate_y

		#resize the shape to the mesh
		a.triangles=[]
		#print(shape0.triangles)
		if shape0.triangles!=None:
			a.triangles.extend(shape0.triangles.data)
			#if shape0.shape_name=="dome":
			#	for t in shape0.triangles.data:
			#		print(t)
		self.gl_objects_add(a)
		#shape0.color_alpha
		self.objects[-1].compile("triangles_solid",[shape0.color_r,shape0.color_g,shape0.color_b,0.5],[self.objects[-1].r_false,self.objects[-1].g_false,self.objects[-1].b_false])
		self.objects[-1].compile("triangles_open",[shape0.color_r*0.9,shape0.color_g*0.9,shape0.color_b*0.9,shape0.color_alpha],[self.objects[-1].r_false,self.objects[-1].g_false,self.objects[-1].b_false],line_width=5)
		#print(self.objects[-1].r_false,self.objects[-1].g_false,self.objects[-1].b_false)

		
		#now itterate over other shapes in this shape
		for s in shape0.shapes:
			self.shape_to_screen(s,base_obj=shape0)







