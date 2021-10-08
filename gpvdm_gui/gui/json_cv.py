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


## @package json_cv_domain
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_cv_domain_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["cv_start_voltage",-2.0])
		self.var_list.append(["cv_stop_voltage",0.5])
		self.var_list.append(["cv_dv_step",0.1])
		self.var_list.append(["cv_fx",1000])
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
		self.var_list.append(["fxdomain_norm_tx_function",False])
		self.var_list.append(["dump_verbosity",0])
		self.var_list.append(["dump_screen_verbosity","dump_verbosity_key_results"])
		self.var_list_build()



class json_cv_domain_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"cv_domain_segment")
		self.var_list=[]
		self.var_list.append(["english_name","CV"])
		self.var_list.append(["icon","cv"])
		self.var_list.append(["config",json_cv_domain_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_cv(json_base):

	def __init__(self):
		json_base.__init__(self,"cv",segment_class=True,segment_example=json_cv_domain_simulation())

