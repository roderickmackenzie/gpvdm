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


## @package json_lasers
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_laser_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["laserwavelength",520e-9])
		self.var_list.append(["spotx",5e-3])
		self.var_list.append(["spoty",5e-3])
		self.var_list.append(["pulseJ",5e-14])
		self.var_list.append(["laser_pulse_width",5e-9])
		self.var_list.append(["laser_photon_efficiency",1.0])
		self.var_list_build()



class json_laser(json_base):

	def __init__(self):
		json_base.__init__(self,"laser_segment")
		self.var_list=[]
		self.var_list.append(["english_name","Green"])
		self.var_list.append(["icon","laser"])
		self.var_list.append(["config",json_laser_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_lasers(json_base):

	def __init__(self):
		json_base.__init__(self,"lasers",segment_class=True)
		self.segments.append(json_laser())

	def load_from_json(self,json):
		segs=json['segments']
		if segs==0:
			return
		self.segments=[]
		for i in range(0,segs):
			a=json_laser()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

