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


## @package json_ray
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base

class json_ray_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["ray_wavelength_points",100])
		self.var_list.append(["ray_auto_wavelength_range","true"])
		self.var_list.append(["ray_lambda_start",300e-9])
		self.var_list.append(["ray_lambda_stop",700e-9])
		self.var_list.append(["ray_auto_run","ray_run_once"])
		self.var_list.append(["ray_escape_bins",20])
		self.var_list_build()


class json_ray_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"ray_segment")
		self.var_list=[]
		self.var_list.append(["english_name","Ray\ntrace"])
		self.var_list.append(["icon","ray"])
		self.var_list.append(["config",json_ray_config()])
		self.var_list_build()


class json_ray(json_base):

	def __init__(self):
		json_base.__init__(self,"ray",segment_class=True)
		self.segments.append(json_ray_simulation())

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_ray_simulation()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

