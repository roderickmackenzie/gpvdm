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

## @package gl_draw_circuit
#  Draw a circuit
#

import sys
from math import fabs

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb
	from PyQt5.QtWidgets import QMenu
	from gl_scale import gl_scale
	from gl_scale import scale_get_zmul
	from gl_scale import scale_get_xmul
	from gl_scale import scale_get_ymul


except:
	pass

from PyQt5.QtCore import QTimer
from inp import inp

from epitaxy import get_epi
from mesh import get_mesh
from gl_base_object import gl_base_object

class gl_draw_circuit():
	def draw_circuit(self):
		return
		for c in self.graph_data.data:
			a=gl_base_object()
			a.id=["electrical_mesh"]
			if c.name=="R":
				a.type="resistor"
			elif c.name=="D":
				a.type="diode"
			#print(c.z0,c.x0,c.y0,c.z1,c.x1,c.y1)
			a.x=self.scale.project_m2screen_x(c.x0)
			a.y=self.scale.project_m2screen_y(c.y0)
			a.z=self.scale.project_m2screen_z(c.z0)
			a.dx=self.scale.project_m2screen_x(c.x1)-a.x
			a.dy=self.scale.project_m2screen_y(c.y1)-a.y
			a.dz=self.scale.project_m2screen_z(c.z1)-a.z
			a.r=1.0
			a.g=0.0
			a.b=0.0
			a.alpha=1.0
			self.gl_objects_add(a)

