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

## @package gl_lib_ray
#  Library to draw ray
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
import os
from math import sqrt
from math import fabs
from lines import lines_read
from util import wavelength_to_rgb
from util import isnumber
from gl_scale import gl_scale
from gl_scale import scale_trianges_m2screen

from gl_scale import scale_get_device_y
from gl_scale import scale_get_device_x
from gl_scale import scale_get_device_z

from gl_base_object import gl_base_object

from dat_file import dat_file
from epitaxy import epitaxy_get_epi
from epitaxy import get_epi

from gl_scale import scale_get_xmul
from gl_scale import scale_get_ymul
from gl_scale import scale_get_zmul
from triangle_io import triangles_mul_vec

from triangle import vec

from mesh import get_mesh

from triangle_io import triangles_flip_in_box
from triangle_io import triangles_add_vec
from triangle_io import triangles_flip

class gl_contacts():

	def draw_contacts(self):
		#print("draw contacts")
		epi=get_epi()
		box=vec()
		pos=vec()
		y_mesh=get_mesh().y
		x_mesh=get_mesh().x
		z_mesh=get_mesh().z

		self.gl_objects_remove_regex("contact")
		top_contact_layer=epi.get_top_contact_layer()
		btm_contact_layer=epi.get_btm_contact_layer()

		for c in epi.contacts.segments:
			if c.shape_enabled==True:
				a=gl_base_object()
				a.id=[c.id]

				if c.position=="left":
					if x_mesh.get_points()>1:
						sticking_out_bit=0.2
						a.type="solid_and_mesh"
						a.xyz.x=gl_scale.project_m2screen_x(0)-sticking_out_bit
						a.xyz.y=gl_scale.project_m2screen_y(c.y0)
						a.xyz.z=gl_scale.project_m2screen_z(0)
						a.dxyz.x=1.0
						a.dxyz.y=scale_get_ymul()*c.dy
						a.dxyz.z=scale_get_device_z()

						a.r=c.color_r
						a.g=c.color_g
						a.b=c.color_b
						a.coloralpha=1.0
						my_vec=vec()
						my_vec.x=sticking_out_bit/scale_get_xmul()#+c.ingress
						my_vec.y=c.dy
						my_vec.z=get_mesh().z.get_len()

						if c.triangles!=None:
							a.triangles=triangles_flip_in_box(c.triangles.data)
							a.triangles=triangles_mul_vec(a.triangles,box)
							a.triangles=triangles_add_vec(a.triangles,pos)
							a.triangles=scale_trianges_m2screen(a.triangles)
							self.gl_objects_add(a)

				elif c.position=="top":
					if top_contact_layer!=-1:
						#if epi.layers[0].name!="air":
						box=vec()
						if x_mesh.get_points()==1 and z_mesh.get_points()==1:
							xstart=0
							box.x=get_mesh().x.get_len()
						else:
							xstart=c.x0
							box.x=c.dx

						box.y=epi.layers[0].dy#+c.ingress
						box.z=get_mesh().z.get_len()

						if self.draw_device_cut_through==False:
							a.type="solid_and_mesh"
						else:
							a.type="solid_and_mesh_cut_through"


						a.xyz.x=gl_scale.project_m2screen_x(xstart)
						if c.shape_flip_y==False:
							a.xyz.y=gl_scale.project_m2screen_y(epi.get_layer_start(0))
						else:
							a.xyz.y=gl_scale.project_m2screen_y(epi.get_layer_end(0))
						a.xyz.z=gl_scale.project_m2screen_z(0.0)

						a.dxyz.x=box.x*scale_get_xmul()
						a.dxyz.y=box.y*scale_get_ymul()
						a.dxyz.z=scale_get_device_z()
						if c.shape_flip_y==False:
							a.dxyz.y=a.dxyz.y*-1.0

						a.r=c.color_r
						a.g=c.color_g
						a.b=c.color_b
						a.color_alpha=1.0

						if c.triangles!=None:
							a.triangles=triangles_mul_vec(c.triangles.data,box)
							if c.shape_flip_y==True:
								a.triangles=triangles_flip(a.triangles)

							a.triangles=scale_trianges_m2screen(a.triangles)
							self.gl_objects_add(a)

				elif c.position=="bottom":
					if btm_contact_layer!=-1:
						if x_mesh.get_points()==1 and z_mesh.get_points()==1:

							xstart=0
							box.x=get_mesh().x.get_len()
						else:
							xstart=c.x0
							box.x=c.dx

						box.y=epi.layers[len(epi.layers)-1].dy#+c.ingress 
						box.z=get_mesh().z.get_len()

						a.type="solid_and_mesh"

						pos.x=xstart
						pos.y=epi.get_layer_start(len(epi.layers)-1)
						pos.z=0

						a.xyz.x=gl_scale.project_m2screen_x(pos.x)
						a.xyz.y=gl_scale.project_m2screen_y(pos.y)
						a.xyz.z=gl_scale.project_m2screen_z(pos.z)

						a.dxyz.x=box.x*scale_get_xmul()
						a.dxyz.y=-box.y*scale_get_ymul()
						a.dxyz.z=scale_get_device_z()

						a.r=epi.layers[len(epi.layers)-1].color_r
						a.g=epi.layers[len(epi.layers)-1].color_g
						a.b=epi.layers[len(epi.layers)-1].color_b

						a.alpha=1.0
						my_vec=vec()
						my_vec.x=c.dx
						my_vec.y=epi.layers[len(epi.layers)-1].dy
						my_vec.z=get_mesh().z.get_len()

						a.r=c.color_r
						a.g=c.color_g
						a.b=c.color_b
						a.color_alpha=1.0

						a.triangles=triangles_flip_in_box(c.triangles.data)
						a.triangles=triangles_mul_vec(a.triangles,box)
						#a.triangles=triangles_add_vec(a.triangles,pos)
						a.triangles=scale_trianges_m2screen(a.triangles)

						self.gl_objects_add(a)

