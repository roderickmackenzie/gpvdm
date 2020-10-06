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
from gl_scale import scale_trianges_m2screen
from triangle_io import triangles_flip

from gl_scale import gl_scale

from epitaxy import get_epi

from gl_scale import scale_get_device_x
from gl_scale import scale_get_device_z
from mesh import get_mesh

class shape_layer():
	def shape_to_screen(self,a,pos,s):
		if s.shape_enabled==True:
			a.type="solid_and_mesh"
			if self.draw_device_cut_through==True:	# and l!=len(epi.layers)-1
					a.type="box_cut_through"
		else:
			a.type="marker"

		a.id=[s.id]
		a.xyz.x=gl_scale.project_m2screen_x(pos.x)
		#a.xyz.y=gl_scale.project_m2screen_y(pos.y)
		if s.shape_flip_y==False:
			a.xyz.y=gl_scale.project_m2screen_y(pos.y)
		else:
			a.xyz.y=gl_scale.project_m2screen_y(pos.y+s.dy)

		a.xyz.z=gl_scale.project_m2screen_z(pos.z)
		#print(">>>>>>",project_m2screen_z(0))

		a.dxyz.x=s.dx*scale_get_xmul()
		a.dxyz.y=s.dy*scale_get_ymul()
		a.dxyz.z=s.dz*scale_get_zmul()
		if s.shape_flip_y==False:
			a.dxyz.y=a.dxyz.y*-1.0

		a.r=s.r
		a.g=s.g
		a.b=s.b

		a.alpha=1.0
		if len(s.shapes)>0:
			a.alpha=0.5

		a.allow_cut_view=True
		a.selectable=True
		v=vec()
		v.x=s.dx
		v.y=s.dy
		v.z=s.dz
		#resize the shape to the mesh
		if s.triangles!=None:
			a.triangles=triangles_mul_vec(s.triangles.data,v)

			if s.shape_flip_y==True:
				a.triangles=triangles_flip(a.triangles)

			a.triangles=scale_trianges_m2screen(a.triangles)

		self.gl_objects_add(a)


	def shape_layer(self,epi_layer,y_padding=0.0, name="name"):
		self.gl_objects_remove_regex(name)

		a=gl_base_object()

		pos=vec()
		pos.x=epi_layer.x0
		pos.y=epi_layer.y0
		pos.z=epi_layer.z0
		epi_layer.dx=get_mesh().get_xlen()
		epi_layer.dz=get_mesh().get_zlen()

		a.origonal_object=True
		self.shape_to_screen(a,pos,epi_layer)

		for s in epi_layer.shapes:
			n=0
			for pos in s.expand_xyz0(epi_layer):
				
				a=gl_base_object()
				if n==0:
					a.origonal_object=True
				n=n+1
				self.shape_to_screen(a,pos,s)



