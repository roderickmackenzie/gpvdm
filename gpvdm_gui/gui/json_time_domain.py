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


## @package json_time_domain
#  Store the time domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base

class json_time_domain_mesh_segment(json_base):

	def __init__(self):
		json_base.__init__(self,"json_time_domain_mesh_segment")
		self.var_list=[]
		self.var_list.append(["len",10e-6])
		self.var_list.append(["dt",0.01e-6])
		self.var_list.append(["voltage_start",0.0])
		self.var_list.append(["voltage_stop",0.0])
		self.var_list.append(["mul",1.0])
		self.var_list.append(["sun_start",0.0])
		self.var_list.append(["sun_stop",0.0])
		self.var_list.append(["laser_start",0.0])
		self.var_list.append(["laser_stop",0.0])
		self.var_list_build()

class json_time_domain_mesh(json_base):

	def __init__(self):
		json_base.__init__(self,"mesh",segment_class=True)

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_time_domain_mesh_segment()
			segment_name="segment"+str(i)
			a.load_from_json(json[segment_name])
			try:
				laser=json[segment_name]['laser']
				a.laser_start=laser
				a.laser_stop=laser
			except:
				pass
			self.segments.append(a)

class json_time_domain_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["pulse_shift",5e-6])
		self.var_list.append(["load_type","load"])
		self.var_list.append(["pulse_L",0.0])
		self.var_list.append(["Rload",50])
		self.var_list.append(["pump_laser","green"])
		self.var_list.append(["pulse_bias",0.0])
		self.var_list.append(["pulse_light_efficiency",1.0])
		self.var_list.append(["pulse_subtract_dc","false"])
		self.var_list.append(["start_time",-4e-12])
		self.var_list.append(["fs_laser_time",-1.0])
		self.var_list.append(["text_output",""])
		self.var_list.append(["dump_verbosity",1])
		self.var_list.append(["dump_energy_space","false"])
		self.var_list.append(["dump_x",0])
		self.var_list.append(["dump_y",0])
		self.var_list.append(["dump_z",0])
		self.var_list_build()



class json_time_domain_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"time_domain_segment")
		self.var_list=[]
		self.var_list.append(["english_name","celiv"])
		self.var_list.append(["icon","celiv"])
		self.var_list.append(["config",json_time_domain_config()])
		self.var_list.append(["mesh",json_time_domain_mesh()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_time_domain(json_base):

	def __init__(self):
		json_base.__init__(self,"time_domain",segment_class=True,segment_example=json_time_domain_simulation())


