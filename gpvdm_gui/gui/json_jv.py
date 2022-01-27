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


class json_jv_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["Vstart",0.0])
		self.var_list.append(["Vstop",1.2])
		self.var_list.append(["Vstep",0.01])
		self.var_list.append(["jv_step_mul",1.00])
		self.var_list.append(["jv_use_external_voltage_as_stop",True])
		self.var_list.append(["jv_light_efficiency",1.0])
		self.var_list.append(["jv_max_j",1e3])
		self.var_list.append(["jv_Rcontact",-1.0])
		self.var_list.append(["jv_Rshunt",-1.0])
		self.var_list.append(["jv_single_point",False])
		self.var_list.append(["text_output",""])
		self.var_list.append(["dump_verbosity",1])
		self.var_list.append(["dump_energy_space","false"])
		self.var_list.append(["dump_x",0])
		self.var_list.append(["dump_y",0])
		self.var_list.append(["dump_z",0])
		self.var_list_build()



class json_jv_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"jv_segment")
		self.var_list=[]
		self.var_list.append(["english_name","JV\\ncurve"])
		self.var_list.append(["icon","jv"])
		self.var_list.append(["config",json_jv_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_jv(json_base):

	def __init__(self):
		json_base.__init__(self,"jv",segment_class=True,segment_example=json_jv_simulation())


