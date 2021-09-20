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

import sys
from math import fabs

from epitaxy import get_epi
import math

from triangle import triangle
from gpvdm_json import gpvdm_data
from triangle import vec

x_mul=1.0
y_mul=1.0
z_mul=1.0
x_start=0.0
y_start=0.0
z_start=0.0


class gl_scale():

	def __init__(self):
		self.world_min=None
		self.world_max=None
		self.refresh_world_size=True

	def project_screen_x_to_m(self,x):
		global x_mul
		global x_start
		if type(x)==float or type(x)==int:
			return (x-x_start)/x_mul
		elif type(x)==list:
			ret=[]
			for val in x:
				ret.append((val-x_start)/x_mul)
			return ret

	def project_screen_y_to_m(self,y):
		global y_mul
		global y_start
		if type(y)==float or type(y)==int:
			return (y-y_start)/y_mul
		elif type(y)==list:
			ret=[]
			for val in y:
				ret.append((val-y_start)/y_mul)
			return ret

	def project_screen_z_to_m(self,z):
		global z_mul
		global z_start
		if type(z)==float or type(z)==int:
			return (z-z_start)/z_mul
		elif type(z)==list:
			ret=[]
			for val in z:
				ret.append((val-z_start)/z_mul)
			return ret

	def project_m2screen_x(self,x):
		global x_mul
		global x_start
		if type(x)==float or type(x)==int:
			return x_start+x_mul*x
		elif type(x)==list:
			ret=[]
			for val in x:
				ret.append(x_start+x_mul*val)
			return ret


	def project_m2screen_y(self,y):
		global y_mul
		global y_start
		if type(y)==float or type(y)==int :
			return y_start+y_mul*y
		elif type(y)==list:
			ret=[]
			for val in y:
				ret.append(y_start+y_mul*val)
			return ret

		
	def project_m2screen_z(self,z):
		global z_mul
		global z_start

		if type(z)==float or type(z)==int:
			return z_start+z_mul*z
		elif type(z)==list:
			ret=[]
			for val in z:
				ret.append(z_start+z_mul*val)
			return ret

	def project_base_objects_from_m_2_screen(self,objs):
		ret=[]
		for o in objs:
			for xyz in o.xyz:
				xyz.x=self.project_m2screen_x(xyz.x)
				xyz.y=self.project_m2screen_y(xyz.y)
				xyz.z=self.project_m2screen_z(xyz.z)
				break

			o.dxyz.x=o.dxyz.x*scale_get_xmul()
			o.dxyz.y=-o.dxyz.y*scale_get_ymul()
			o.dxyz.z=o.dxyz.z*scale_get_zmul()

			ret.append(o)
		return ret

	def set_m2screen(self):
		global x_mul
		global y_mul
		global z_mul
		global x_start
		global y_start
		global z_start
		if self.refresh_world_size==True:
			self.world_min,self.world_max=gpvdm_data().get_world_size()
			
		max_dist_x=10
		max_dist_z=10
		max_dist_y=10

		x_len= self.world_max.x-self.world_min.x
		y_len= self.world_max.y-self.world_min.y 
		z_len= self.world_max.z-self.world_min.z

		xyz_max=max([x_len,y_len,z_len])

		#all the same
		x_mul=max_dist_x/xyz_max
		y_mul=max_dist_y/xyz_max
		z_mul=max_dist_z/xyz_max

		if y_len*10.0<xyz_max:		#rescale for thin devices
			max_dist_y=2
			y_mul=2.0*max_dist_y/y_len

		size_x=x_len*x_mul
		size_z=z_len*z_mul

		x_start=-size_x/2.0
		z_start=-size_z/2.0
		y_start=0.0

	def scale_trianges_m2screen(self,triangles):
		global x_mul
		global y_mul
		global z_mul
		ret=[]
		for t in triangles:
			t0=triangle()
			t0.points=t.points
			t0.xyz0.x=t.xyz0.x*x_mul
			t0.xyz0.y=t.xyz0.y*y_mul
			t0.xyz0.z=t.xyz0.z*z_mul

			t0.xyz1.x=t.xyz1.x*x_mul
			t0.xyz1.y=t.xyz1.y*y_mul
			t0.xyz1.z=t.xyz1.z*z_mul

			t0.xyz2.x=t.xyz2.x*x_mul
			t0.xyz2.y=t.xyz2.y*y_mul
			t0.xyz2.z=t.xyz2.z*z_mul
			ret.append(t0)

		return ret

	def project_trianges_m2screen(self,triangles):
		ret=[]
		for t in triangles:
			t0=triangle()
			print(t)
			t0.points=t.points
			t0.xyz0.x=self.project_m2screen_x(t.xyz0.x)
			t0.xyz0.y=self.project_m2screen_y(t.xyz0.y)
			t0.xyz0.z=self.project_m2screen_z(t.xyz0.z)

			t0.xyz1.x=self.project_m2screen_x(t.xyz1.x)
			t0.xyz1.y=self.project_m2screen_y(t.xyz1.y)
			t0.xyz1.z=self.project_m2screen_z(t.xyz1.z)

			t0.xyz2.x=self.project_m2screen_x(t.xyz2.x)
			t0.xyz2.y=self.project_m2screen_y(t.xyz2.y)
			t0.xyz2.z=self.project_m2screen_z(t.xyz2.z)

			ret.append(t0)

		return ret

	def get_xmul(self):
		global x_mul
		return x_mul

	def get_ymul(self):
		global y_mul
		return y_mul

	def get_zmul(self):
		global z_mul
		return z_mul

def scale_get_xmul():
	global x_mul
	return x_mul

def scale_get_ymul():
	global y_mul
	return y_mul

def scale_get_zmul():
	global z_mul
	return z_mul


