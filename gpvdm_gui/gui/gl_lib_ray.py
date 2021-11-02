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

from gl_base_object import gl_base_object

from dat_file import dat_file

class gl_lib_ray():

	def __init__(self):
		self.ray_data=dat_file()
		self.ray_mesh_data=dat_file()
		self.triangle_file="triangles.dat"

	def draw_rays(self,ray_file):
		r=1.0
		g=0.0
		b=0.0
		if self.ray_data.load(ray_file)==True:
			self.gl_objects_remove_regex("ray_trace_results")
			self.draw_graph_rays(self.ray_data)

	def draw_ray_mesh(self):
		r=1.0
		g=0.0
		b=0.0
		if self.ray_mesh_data.load(self.triangle_file)==True:
			if self.ray_mesh_data.new_read==True or self.gl_objects_count_regex("ray_mesh")==0:
				self.gl_objects_remove_regex("ray_mesh")
				a=gl_base_object()
				a.id=["ray_mesh"]
				a.type="open_triangles"
				a.r=r
				a.g=g
				a.b=b
				a.triangles=self.scale.project_trianges_m2screen(self.ray_mesh_data.data)
				self.gl_objects_add(a)


