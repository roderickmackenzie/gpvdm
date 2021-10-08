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


## @package json_jv
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base

class json_gl_view(json_base):

	def __init__(self):
		json_base.__init__(self,"gl_view")
		self.var_list=[]
		self.var_list.append(["xRot",-90.0])
		self.var_list.append(["yRot",90.0])
		self.var_list.append(["zRot",0.0])
		self.var_list.append(["x_pos",0.0])
		self.var_list.append(["y_pos",0.0])
		self.var_list.append(["zoom",16])
		self.var_list_build()


class json_gl(json_base):
	def __init__(self):
		json_base.__init__(self,"gl",segment_class=True)

	def load_from_json(self,json):
		self.segments=[]
		try:
			segs=json['segments']
		except:
			self.segments.append(json_gl_view())
			self.segments.append(json_gl_view())
			return
		for i in range(0,segs):
			a=json_gl_view()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)
