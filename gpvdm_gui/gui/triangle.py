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

## @package triangle
#  A base triangle class
#

import os
from math import pi,acos,sin,cos
from json_base import json_base

class vec(json_base):
	def __init__(self,name="vec"):
		json_base.__init__(self,name)
		self.var_list=[]
		self.var_list.append(["x",0.0])
		self.var_list.append(["y",0.0])
		self.var_list.append(["z",0.0])
		self.var_list_build()

		#self.x=0.0
		#self.y=0.0
		#self.z=0.0

	def __str__(self):
		return "(x="+str(self.x)+",y="+str(self.y)+",z="+str(self.z)+")"

	def __sub__(self,data):
		a=vec()
		a.x=self.x-data.x
		a.y=self.y-data.y
		a.z=self.z-data.z
		return a

	def __add__(self,data):
		if type(data)==vec:
			a=vec()
			a.x=self.x+data.x
			a.y=self.y+data.y
			a.z=self.z+data.z
		if type(data)==float or type(data)==int:
			a=vec()
			a.x=self.x+data
			a.y=self.y+data
			a.z=self.z+data
		return a

	def __truediv__(self,data):
		if type(data)==vec:
			a=vec()
			if data.x!=0:
				a.x=self.x/data.x

			if data.y!=0:
				a.y=self.y/data.y

			if data.z!=0:
				a.z=self.z/data.z

		if type(data)==float:
			a=vec()
			a.x=self.x/data
			a.y=self.y/data
			a.z=self.z/data

		return a

	def __mul__(self,data):
		a=vec()
		a.x=self.x*data.x
		a.y=self.y*data.y
		a.z=self.z*data.z
		return a
	
	def rotate(self,theta):
		theta_rad=(theta/360.0)*2*3.14159
		#print()
		a=vec()
		a.x=self.x*cos(theta_rad)-self.y*sin(theta_rad)
		a.y=self.x*sin(theta_rad)+self.y*cos(theta_rad)
		a.z=0.0
		return a

	def rotate_y(self,theta):		#rotate around the y axis
		theta_rad=(theta/360.0)*2*3.14159
		a=vec()
		a.x = self.x*cos(theta_rad) + self.z*sin(theta_rad)
		a.y = self.y
		a.z = self.z*cos(theta_rad) - self.x*sin(theta_rad)
		return a

	def cpy(self,data):
		self.x=data.x
		self.y=data.y
		self.z=data.z


class triangle(json_base):

	def rotate_y(self,ang):
		a=triangle()
		a.xyz0=self.xyz0.rotate_y(ang)
		a.xyz1=self.xyz1.rotate_y(ang)
		a.xyz2=self.xyz2.rotate_y(ang)
		return a

	def __init__(self):
		json_base.__init__(self,"vec")
		self.var_list=[]
		self.var_list.append(["xyz0",vec()])
		self.var_list.append(["xyz1",vec()])
		self.var_list.append(["xyz2",vec()])
		self.var_list_build()

		#self.xyz0=vec()
		#self.xyz1=vec()
		#self.xyz2=vec()
		self.points=3

	def __str__(self):
		return str(self.xyz0)+"\n"+str(self.xyz1)+"\n"+str(self.xyz2)+"\n"

	def min_x(self):
		x=self.xyz0.x

		if self.xyz1.x<x:
			x=self.xyz1.x

		if self.xyz2.x<x:
			x=self.xyz2.x

		return x

	def min_y(self):
		y=self.xyz0.y

		if self.xyz1.y<y:
			y=self.xyz1.y

		if self.xyz2.y<y:
			y=self.xyz2.y

		return y

	def min_z(self):
		z=self.xyz0.z

		if self.xyz1.z<z:
			z=self.xyz1.z

		if self.xyz2.z<z:
			z=self.xyz2.z

		return z

	def max_x(self):
		x=self.xyz0.x

		if self.xyz1.x>x:
			x=self.xyz1.x

		if self.xyz2.x>x:
			x=self.xyz2.x

		return x

	def max_y(self):
		y=self.xyz0.y

		if self.xyz1.y>y:
			y=self.xyz1.y

		if self.xyz2.y>y:
			y=self.xyz2.y

		return y

	def max_z(self):
		z=self.xyz0.z

		if self.xyz1.z>z:
			z=self.xyz1.z

		if self.xyz2.z>z:
			z=self.xyz2.z

		return z

	def __sub__(self,data):
		if type(data)==vec:
			a=triangle()
			a.xyz0=self.xyz0-data
			a.xyz1=self.xyz1-data
			a.xyz2=self.xyz2-data
			return a

	def __add__(self,data):
		if type(data)==vec:
			a=triangle()
			a.xyz0=self.xyz0+data
			a.xyz1=self.xyz1+data
			a.xyz2=self.xyz2+data
			return a

	def __truediv__(self,data):
		if type(data)==vec:
			a=triangle()
			a.xyz0=self.xyz0/data
			a.xyz1=self.xyz1/data
			a.xyz2=self.xyz2/data
			return a

	def __mul__(self,data):
		if type(data)==vec:
			a=triangle()
			a.xyz0=self.xyz0*data
			a.xyz1=self.xyz1*data
			a.xyz2=self.xyz2*data
			return a

