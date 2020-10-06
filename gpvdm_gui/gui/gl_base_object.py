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

## @package gl_base_object
#  Save the OpenGL scene.
#

import sys
import glob
from triangle import vec

class gl_base_object():
	def __init__(self,x=0.0,y=0.0,z=0.0,dx=0.0,dy=0.0,dz=0.0,r=1.0,g=1.0,b=1.0):
		self.id=[]
		self.type=""
		self.xyz=vec()
		self.xyz.x=x
		self.xyz.y=y
		self.xyz.z=z

		self.dxyz=vec()
		self.dxyz.x=dx
		self.dxyz.y=dy
		self.dxyz.z=dz

		self.r=r
		self.g=g
		self.b=b

		self.r_false=None
		self.g_false=None
		self.b_false=None

		self.alpha=0.5
		self.selected=False
		self.selectable=False
		self.moveable=False
		self.allow_cut_view=True
		self.triangles=[]
		self.points=[]
		self.text=""
		self.texture=None

		self.origonal_object=False

	def copy(self,obj):
		self.id=obj.id
		self.type=obj.type

		self.xyz.cpy(obj.xyz)

		self.dxyz.cpy(obj.dxyz)

		self.r=obj.r
		self.g=obj.g
		self.b=obj.b

		self.r_false=obj.r_false
		self.g_false=obj.g_false
		self.b_false=obj.b_false

		self.alpha=obj.alpha
		self.selected=obj.selected
		self.selectable=obj.selectable
		self.moveable=obj.moveable
		self.allow_cut_view=obj.allow_cut_view
		self.triangles=obj.triangles
		self.points=obj.points

	def match_false_color(self,r,g,b):
		rf=int(self.r_false*255)
		gf=int(self.g_false*255)
		bf=int(self.b_false*255)

		ri=int(r*255)
		gi=int(g*255)
		bi=int(b*255)
		if ri==rf and gi==gf and bi==bf:
			return True

		return False

	def dump(self):
		print(self.id)
		print(self.type)
		print("x: "+str(self.xyz.x))
		print("y: "+str(self.xyz.y))
		print("z: "+str(self.xyz.z))
		print("dx: "+str(self.xyz.dx))
		print("dy: "+str(self.xyz.dy))
		print("dz: "+str(self.xyz.dz))
		print(self.r)
		print(self.g)
		print(self.b)
		print(self.alpha)
		print(self.selected)
		print(self.selectable)
		print(self.moveable)
		print(self.triangles)

	def id_starts_with(self,val):

		for id in self.id:
			if id.startswith(val)==True:
				return id

		return False

	def __str__(self):
		ret=""
		ret=ret+self.type

		ret=ret+";;"+str(self.xyz.x)
		ret=ret+";;"+str(self.xyz.y)
		ret=ret+";;"+str(self.xyz.z)

		ret=ret+";;"+str(self.dxyz.x)
		ret=ret+";;"+str(self.dxyz.y)
		ret=ret+";;"+str(self.dxyz.z)

		ret=ret+";;"+str(self.r)
		ret=ret+";;"+str(self.g)
		ret=ret+";;"+str(self.b)

		ret=ret+";;"+str(self.alpha)
		ret=ret+";;"+str(self.selected)
		ret=ret+";;"+str(self.selectable)
		ret=ret+";;"+str(self.moveable)
		ret=ret+";;"+str(self.allow_cut_view)

		ret=ret+";;"+str(self.text)

		ret=ret+";;"+str(self.origonal_object)

		return ret
