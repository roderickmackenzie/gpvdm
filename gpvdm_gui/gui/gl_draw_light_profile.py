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

## @package gl_draw_light_profile
#  Draw the 3D light profile
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
	
import os
from util import wavelength_to_rgb

from gl_base_object import gl_base_object

from dat_file import dat_file
from epitaxy import get_epi


from triangle_io import triangles_mul_vec
from triangle_io import triangles_add_vec
from triangle_io import triangles_remove_below

from triangle import vec

from mesh import get_mesh

from triangle_io import triangles_flip_in_box
from shape import shape
from cal_path import get_sim_path
from gpvdm_json import gpvdm_data

class gl_draw_light_profile():

	def draw_light_profile(self):

		self.gl_objects_remove_regex("light_profile")
		data=gpvdm_data()
		if data.light.light_profile!="box":
			epi=get_epi()

			s=shape()
			s.type=data.light.light_profile
			s.load_triangles()
			s.triangles.data=triangles_remove_below(s.triangles.data,0.1)

			a=gl_base_object()
			a.id=["light_profile"]
			a.type="open_triangles"

			a.x=self.scale.project_m2screen_x(0)
			a.y=self.scale.project_m2screen_y(0)
			a.z=self.scale.project_m2screen_z(0)

			a.dx=1.0
			a.dy=self.scale.y_mul*1.0
			a.dz=1.0

			a.r=0.0
			a.g=1.0
			a.b=0.0
			a.alpha=1.0

			my_vec=vec()
			my_vec.x=gpvdm_data().mesh.mesh_z.get_len()
			my_vec.y=epi.ylen()*1.0
			my_vec.z=gpvdm_data().mesh.mesh_z.get_len()

			t=triangles_mul_vec(s.triangles.data,my_vec)

			my_vec=vec()
			my_vec.x=0.0
			my_vec.y=-epi.ylen()*3.0
			my_vec.z=0.0

			t=triangles_add_vec(t,my_vec)

			a.triangles=self.scale.scale_trianges_m2screen(t)
			self.gl_objects_add(a)


