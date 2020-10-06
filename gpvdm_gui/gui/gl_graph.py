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

## @package gl_view_point
#  The gl_view_point class for the OpenGL display.
#

import os
import sys
from math import fabs
from cal_path import get_sim_path

from dat_file import dat_file
from dat_file_math import dat_file_max_min

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb
	from gl_scale import gl_scale
	from gl_list import gl_base_object
	from gl_scale import scale_get_xmul
	from gl_scale import scale_get_ymul
	from gl_scale import scale_get_zmul

except:
	pass



class gl_graph():

	def __init__(self):
		self.frac_max=1.0
		self.frac_min=0.0

	def draw_graph(self):
		z=0
		max_y_pos=int(len(self.graph_data.y_scale)*self.frac_max)
		min_y_pos=int(len(self.graph_data.y_scale)*self.frac_min)
		if min_y_pos<0:
			min_y_pos=0

		if max_y_pos>=len(self.graph_data.y_scale):
			max_y_pos=len(self.graph_data.y_scale)-1

		if min_y_pos>=max_y_pos:
			min_y_pos=max_y_pos-1

		#print(min_y_pos,max_y_pos)
		#for z in range(0,len(self.graph_data.z_scale)):
		my_max,my_min=dat_file_max_min(self.graph_data)

		if len(self.graph_data.z_scale)==1:
			zi_list=[0]
		else:
			zi_list=[0,len(self.graph_data.z_scale)-1]

		glBegin(GL_QUADS)

		if len(self.graph_data.z_scale)>1:
			dz=self.graph_data.z_scale[1]-self.graph_data.z_scale[0]
		else:
			dz=0.0

		dx=self.graph_data.x_scale[1]-self.graph_data.x_scale[0]
		dy=self.graph_data.y_scale[1]-self.graph_data.y_scale[0]

		#front,back
		for zi in zi_list:
			for xi in range(0,len(self.graph_data.x_scale)):
				for yi in range(min_y_pos,max_y_pos):
					x0=gl_scale.project_m2screen_x(self.graph_data.x_scale[xi])
					y0=gl_scale.project_m2screen_y(self.graph_data.y_scale[yi])
					z0=gl_scale.project_m2screen_z(self.graph_data.z_scale[zi])
					x1=gl_scale.project_m2screen_x(self.graph_data.x_scale[xi]+dx)
					y1=gl_scale.project_m2screen_y(self.graph_data.y_scale[yi]+dy)
					z1=gl_scale.project_m2screen_z(self.graph_data.z_scale[zi])
					if self.graph_data.data[zi][xi][yi]!=0.0:
						r,g,b=val_to_rgb((self.graph_data.data[zi][xi][yi]-my_min)/(my_max-my_min),grey=True)

						#glColor4f(r,g,b, 1.0)
						col = [r, g, b, 1.0]
						glMaterialfv(GL_FRONT, GL_DIFFUSE, col)

						glVertex3f(x0, y0, z0)
						glVertex3f(x1, y0, z0)
						glVertex3f(x1, y1, z0)
						glVertex3f(x0, y1, z0) 

		if len(self.graph_data.z_scale)==1:
			glEnd()
			return
 
		#left,right
		for xi in [0, len(self.graph_data.x_scale)-1]:
			for zi in range(0,len(self.graph_data.z_scale)):
				for yi in range(min_y_pos,max_y_pos):
					x0=gl_scale.project_m2screen_x(self.graph_data.x_scale[xi])
					y0=gl_scale.project_m2screen_y(self.graph_data.y_scale[yi])
					z0=gl_scale.project_m2screen_z(self.graph_data.z_scale[zi])
					x1=gl_scale.project_m2screen_x(self.graph_data.x_scale[xi])
					y1=gl_scale.project_m2screen_y(self.graph_data.y_scale[yi]+dy)
					z1=gl_scale.project_m2screen_z(self.graph_data.z_scale[zi]+dz)
					r,g,b=val_to_rgb((self.graph_data.data[zi][xi][yi]-my_min)/(my_max-my_min),grey=True)

					#glColor4f(r,g,b, 1.0)
					col = [r, g, b, 1.0]
					glMaterialfv(GL_FRONT, GL_DIFFUSE, col)

					glVertex3f(x0, y0, z0)
					glVertex3f(x0, y1, z0)
					glVertex3f(x0, y1, z1)
					glVertex3f(x0, y0, z1)


		#top,bottom
		for yi in [min_y_pos,max_y_pos-1]:
			for zi in range(0,len(self.graph_data.z_scale)):
				for xi in range(0,len(self.graph_data.x_scale)):
					x0=gl_scale.project_m2screen_x(self.graph_data.x_scale[xi])
					y0=gl_scale.project_m2screen_y(self.graph_data.y_scale[yi])
					z0=gl_scale.project_m2screen_z(self.graph_data.z_scale[zi])
					x1=gl_scale.project_m2screen_x(self.graph_data.x_scale[xi]+dx)
					y1=gl_scale.project_m2screen_y(self.graph_data.y_scale[yi])
					z1=gl_scale.project_m2screen_z(self.graph_data.z_scale[zi]+dz)

					r,g,b=val_to_rgb((self.graph_data.data[zi][xi][yi]-my_min)/(my_max-my_min),grey=True)

					#glColor4f(r,g,b, 1.0)
					col = [r, g, b, 1.0]
					glMaterialfv(GL_FRONT, GL_DIFFUSE, col)

					glVertex3f(x0, y0, z0)
					glVertex3f(x1, y0, z0)
					glVertex3f(x1, y0, z1)
					glVertex3f(x0, y0, z1)

		glEnd()



	def draw_mode(self):
		if self.false_color==True:
			return

		glLineWidth(5)
		glColor3f(1.0, 1.0, 1.0)

		t=[]
		s=[]

		data=dat_file()
		
		path=os.path.join(get_sim_path(),"optical_output","light_1d_photons_tot_norm.dat")

		if data.load(path)==True:
			glBegin(GL_LINES)
			array_len=data.y_len
			s=data.data[0][0]
			#print(path)
			#print(data.data)
			for i in range(1,array_len):
				x=gl_scale.project_m2screen_x(0)
				y=gl_scale.project_m2screen_y(data.y_scale[i-1])
				z=gl_scale.project_m2screen_z(0)-s[i-1]*0.5
				glVertex3f(x, y, z)

				y=gl_scale.project_m2screen_y(data.y_scale[i])
				z=gl_scale.project_m2screen_z(0)-s[i]*0.5
				glVertex3f(x, y, z)

			glEnd()

