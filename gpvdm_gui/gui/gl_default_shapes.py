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

## @package gl_default_shapes
#  The gl_default_shapes class for the OpenGL display.
#

import sys
import os
from math import fabs

import math

from triangle import triangle
from cal_path import gpvdm_paths
from dat_file import dat_file
from triangle_io import triangles_get_min
from triangle_io import triangles_get_max
from triangle_io import triangles_sub_vec
from triangle_io import triangles_div_vec
from triangle_io import triangles_mul_vec

class gl_default_shapes():
	def __init__(self):
		shape_path=os.path.join(gpvdm_paths.get_shape_path(),"box","shape.inp")
		if os.path.isfile(shape_path)==True:
			self.box=dat_file()
			self.box.load(shape_path)
			if self.box.data!=None:
				min_vec=triangles_get_min(self.box.data)
				self.box.data=triangles_sub_vec(self.box.data,min_vec)
				max_vec=triangles_get_max(self.box.data)
				self.box.data=triangles_div_vec(self.box.data,max_vec)

