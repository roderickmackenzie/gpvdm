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

class json_sub_scan(json_base):

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

class json_all_scans:

	def __init__(self):
		self.segments=[]
		self.segments.append(json_sub_scan())
		#self.segments.append(json_mesh_segment())

	def gen_json(self):
		out=[]
		out.append("\"scans\": {")
		out.append("\"segments\": "+str(len(self.segments))+",")
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
			a=json_sub_scan()
			segment_name="segment"+str(i)
			a.load_from_json(json[segment_name])
			self.segments.append(a)

class json_scan_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["none","none"])
		self.var_list_build()


class json_scans(json_base):

	def __init__(self):
		json_base.__init__(self,"scans")
		self.var_list=[]
		self.var_list.append(["config",json_scan_config()])
		self.var_list.append(["scans",json_all_scans()])
		self.var_list_build()


