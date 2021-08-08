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


## @package json_world
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base
from shape import shape

class json_world_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["world_x",1.0])
		self.var_list.append(["world_y",1.0])
		self.var_list.append(["world_z",1.0])
		self.var_list_build()

class json_world_data(json_base):

	def __init__(self):
		json_base.__init__(self,"world_data",segment_class=True)

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=shape()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

		if len(self.segments)==0:
			self.segments.append(shape())

class json_world(json_base):

	def __init__(self):
		json_base.__init__(self,"world")
		self.var_list=[]
		self.var_list.append(["config",json_world_config()])
		self.var_list.append(["world_data",json_world_data()])
		self.var_list_build()

