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


## @package json_fx_domain
#  Store the fx domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base

class json_fx_domain_mesh_segment(json_base):

	def __init__(self):
		json_base.__init__(self,"json_fx_domain_mesh_segment")
		self.var_list=[]
		self.var_list.append(["start",20.0])
		self.var_list.append(["stop",20.0])
		self.var_list.append(["points",1.0])
		self.var_list.append(["mul",1.0])
		self.var_list_build()

class json_fx_domain_mesh(json_base):

	def __init__(self):
		json_base.__init__(self,"mesh",segment_class=True)
		#self.segments.append(json_fx_domain_mesh_segment())
		#self.segments.append(json_fx_domain_mesh_segment())

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_fx_domain_mesh_segment()
			segment_name="segment"+str(i)
			a.load_from_json(json[segment_name])
			self.segments.append(a)

class json_fx_domain_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["is_Vexternal",0.0])
		self.var_list.append(["load_type","load"])
		self.var_list.append(["fxdomain_large_signal",True])
		self.var_list.append(["fxdomain_Rload",50])
		self.var_list.append(["fxdomain_points",30])
		self.var_list.append(["fxdomain_n",5])
		self.var_list.append(["fx_modulation_type","voltage"])
		self.var_list.append(["fxdomain_measure","measure_current"])
		self.var_list.append(["fxdomain_voltage_modulation_max",0.01])
		self.var_list.append(["fxdomain_light_modulation_depth",0.01])
		self.var_list.append(["fxdomain_do_fit",True])
		self.var_list.append(["fxdomain_L",0.0])
		self.var_list.append(["periods_to_fit",2])
		self.var_list.append(["fxdomain_modulation_rolloff_enable",False])
		self.var_list.append(["fxdomain_modulation_rolloff_start_fx",1e3])
		self.var_list.append(["fxdomain_modulation_rolloff_speed",1.6026e-05])
		self.var_list.append(["fxdomain_norm_tx_function",True])
		self.var_list.append(["dump_verbosity",0])
		self.var_list.append(["dump_screen_verbosity","dump_verbosity_key_results"])
		self.var_list_build()



class json_fx_domain_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"fx_domain_segment")
		self.var_list=[]
		self.var_list.append(["english_name","celiv"])
		self.var_list.append(["icon","celiv"])
		self.var_list.append(["config",json_fx_domain_config()])
		self.var_list.append(["mesh",json_fx_domain_mesh()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_fx_domain(json_base):

	def __init__(self):
		json_base.__init__(self,"fx_domain",segment_class=True,segment_example=json_fx_domain_simulation())


