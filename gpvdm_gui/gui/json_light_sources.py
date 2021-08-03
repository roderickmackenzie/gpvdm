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


## @package json_light
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base
from json_light import json_virtual_spectra

class json_light_sources_config(json_base):
	def __init__(self,name):
		json_base.__init__(self,name)
		self.var_list.append(["lpoints",200])
		self.var_list_build()

class json_light_source(json_base):

	def __init__(self,name):
		json_base.__init__(self,name)
		self.var_list=[]
		self.var_list.append(["english_name","Light\\nsource"])
		self.var_list.append(["icon","lighthouse"])
		self.var_list.append(["light_illuminate_from","y0"])
		self.var_list.append(["lstart",270e-9])
		self.var_list.append(["lstop",1600e-9])
		self.var_list.append(["virtual_spectra",json_virtual_spectra("virtual_spectra")])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()

class json_lights(json_base):
	def __init__(self):
		json_base.__init__(self,"lights",segment_class=True)


	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_light_source("light_source")
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

		if len(self.segments)==0:
			self.segments.append(json_light_source("light_source"))

class json_light_sources(json_base):
	def __init__(self):
		json_base.__init__(self,"light_sources")
		self.var_list.append(["config",json_light_sources_config("config")])
		self.var_list.append(["lights",json_lights()])
		self.var_list_build()


