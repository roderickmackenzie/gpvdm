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


## @package json_detectors
#  Store the information on the optical detectors
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_detectors_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["viewpoint_enabled",False])
		self.var_list.append(["viewpoint_x0",0.1])
		self.var_list.append(["viewpoint_y0",0.1])
		self.var_list.append(["viewpoint_z0",0.1])
		self.var_list.append(["viewpoint_dx",0.8])
		self.var_list.append(["viewpoint_dz",0.8])
		self.var_list.append(["viewpoint_nx",20])
		self.var_list.append(["viewpoint_nz",20])
		self.var_list_build()



class json_detector(json_base):

	def __init__(self):
		json_base.__init__(self,"detector")
		self.var_list=[]
		self.var_list.append(["english_name","Detector"])
		self.var_list.append(["icon","jv"])
		self.var_list.append(["config",json_detectors_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_detectors(json_base):

	def __init__(self):
		json_base.__init__(self,"detectors",segment_class=True,segment_example=json_detector())


