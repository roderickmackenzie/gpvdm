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


## @package json_mesh
#  Store the fx domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base

class json_mesh_segment(json_base):

	def __init__(self):
		json_base.__init__(self,"json_mesh_segment")
		self.var_list=[]
		self.var_list.append(["len",0.0032249])
		self.var_list.append(["points",1.0])
		self.var_list.append(["mul",1.0])
		self.var_list.append(["left_right","left"])
		self.var_list_build()

		self.start=0.0
		self.end=0.0

class json_mesh_xyz(json_base):

	def __init__(self,xyz):
		json_base.__init__(self,"mesh",segment_class=True)
		self.xyz=xyz
		self.segments.append(json_mesh_segment())
		#self.segments.append(json_mesh_segment())

	def gen_json(self):
		out=[]
		out.append("\"mesh_"+self.xyz+"\": {")
		out.append("\"segments\":"+str(len(self.segments))+",")
		i=0
		for s in self.segments:
			s.base_name="segment"+str(i)
			out.extend(s.gen_json())
			out[-1]=out[-1]+","
			i=i+1
		out[-1]=out[-1][:-1]
		out.append("}")

		return out

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_mesh_segment()
			segment_name="segment"+str(i)
			a.load_from_json(json[segment_name])
			self.segments.append(a)

	def get_len(self):
		tot=0.0
		for l in self.segments:
			tot=tot+l.len

		return tot

	def get_points(self):
		tot=0.0
		for l in self.segments:
			tot=tot+l.points

		return tot

	def set_len(self,value):
		if len(self.segments)==1:
			self.segments[0].len=value
			return True
		else:
			return False

class json_mesh_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["remesh_x",True])
		self.var_list.append(["remesh_y",True])
		self.var_list.append(["remesh_z",True])
		self.var_list_build()


class json_mesh(json_base):

	def __init__(self):
		json_base.__init__(self,"mesh")
		self.var_list=[]
		self.var_list.append(["config",json_mesh_config()])
		self.var_list.append(["mesh_x",json_mesh_xyz("x")])
		self.var_list.append(["mesh_y",json_mesh_xyz("y")])
		self.var_list.append(["mesh_z",json_mesh_xyz("z")])
		self.var_list_build()


