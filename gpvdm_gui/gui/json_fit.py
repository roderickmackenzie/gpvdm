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


## @package json_fit
#  Store the fit config in a json file
#


import sys
import os
import shutil
import json
from json_base import json_base
from json_import_config import json_import_config
import codecs

class json_fit_patch_line(json_base):

	def __init__(self):
		json_base.__init__(self,"fit_patch_line")
		self.var_list=[]
		self.var_list.append(["json_path","one/two/three"])
		self.var_list.append(["human_path","one/two/three"])
		self.var_list.append(["val","jv@jv"])
		self.var_list_build()

class json_fit_config(json_base):

	def __init__(self):
		json_base.__init__(self,"fit_config")
		self.var_list=[]
		self.var_list.append(["fit_randomize",False])
		self.var_list.append(["fit_random_reset_ittr",400])
		self.var_list.append(["fit_stall_steps",400])
		self.var_list.append(["fit_disable_reset_at",0.095])
		self.var_list.append(["fit_converge_error",1e-3])
		self.var_list.append(["fit_enable_simple_reset",True])
		self.var_list.append(["fit_simplexmul",0.1])
		self.var_list.append(["fit_simplex_reset",200])
		self.var_list.append(["fit_method","simplex"])
		self.var_list_build()

class json_fit_patch(json_base):

	def __init__(self):
		json_base.__init__(self,"fit_patch",segment_class=True,segment_example=json_fit_patch_line())
		self.segments.append(json_fit_patch_line())


class json_data_set_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["enabled",True])
		self.var_list.append(["plugin","fit_stark"])
		self.var_list.append(["fit_name","jv_light_0h"])
		self.var_list.append(["fit_error_mul",1.0])
		self.var_list.append(["time_shift",0.0])
		self.var_list.append(["fit_shift_y",0.0])
		self.var_list.append(["start",0.0])
		self.var_list.append(["stop",0.85])
		#self.var_list.append(["end_data",1.0])
		self.var_list.append(["sim_data","jv.dat"])
		self.var_list.append(["log_x",False])
		self.var_list.append(["log_y",False])
		self.var_list.append(["log_y_keep_sign",False])
		self.var_list.append(["fit_invert_simulation_y",False])
		self.var_list.append(["fit_subtract_lowest_point",False])
		self.var_list.append(["fit_set_first_point_to_zero",False])
		self.var_list.append(["fit_set_error_to_zero_before",-100.0])
		self.var_list.append(["fit_hidden",False])
		self.var_list.append(["fit_against","self"])
		self.var_list_build()

class json_data_set(json_base):

	def __init__(self):
		json_base.__init__(self,"data_set")
		self.var_list=[]
		self.var_list.append(["fit_patch",json_fit_patch()])
		self.var_list.append(["config",json_data_set_config()])
		self.var_list.append(["import_config",json_import_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_fit_vars_line(json_base):

	def __init__(self):
		json_base.__init__(self,"segment")
		self.var_list=[]
		self.var_list.append(["fit_var_enabled",True])
		#self.var_list.append(["enabled",True])
		self.var_list.append(["human_var","one/two/three"])
		self.var_list.append(["json_var","one/two/three"])
		self.var_list.append(["min",0.0])
		self.var_list.append(["max",100.0])
		self.var_list.append(["log_fit",False])
		self.var_list.append(["error",100.0])
		self.var_list_build()

class json_fit_vars(json_base):

	def __init__(self):
		json_base.__init__(self,"vars",segment_class=True)
		self.segments.append(json_fit_vars_line())


	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_fit_vars_line()
			data_set="segment"+str(i)
			a.load_from_json(json[data_set])
			if "enabled" in json[data_set]:						#backwards compatability remove in the future 27/05/21
				a.fit_var_enabled=json[data_set]["enabled"]
			self.segments.append(a)

class json_fit_rules_line(json_base):

	def __init__(self):
		json_base.__init__(self,"segment")
		self.var_list=[]
		self.var_list.append(["fit_rule_enabled",False])
		self.var_list.append(["human_x","one/two/three"])
		self.var_list.append(["human_y","one/two/three"])
		self.var_list.append(["function","x>y"])
		self.var_list.append(["json_x","one/two/three"])
		self.var_list.append(["json_y","one/two/three"])
		self.var_list_build()

class json_fit_rules(json_base):

	def __init__(self):
		json_base.__init__(self,"rules",segment_class=True,segment_example=json_fit_rules_line())
		self.segments.append(json_fit_rules_line())


class json_duplicate_line(json_base):

	def __init__(self):
		json_base.__init__(self,"segment")
		self.var_list=[]
		self.var_list.append(["duplicate_var_enabled",True])
		self.var_list.append(["human_src","one/two/three"])
		self.var_list.append(["human_dest","one/two/three"])
		self.var_list.append(["multiplier","x"])
		self.var_list.append(["json_src","one/two/three"])
		self.var_list.append(["json_dest","one/two/three"])
		self.var_list_build()

class json_fit_duplicate(json_base):

	def __init__(self):
		json_base.__init__(self,"duplicate",segment_class=True,segment_example=json_duplicate_line())

class json_all_fits(json_base):

	def __init__(self):
		json_base.__init__(self,"fits")
		self.segments=[]
		self.segments.append(json_data_set())
		#self.simulations.append(json_fx_domain_simulation())

	def gen_json(self):
		out=[]
		out.append("\"fits\": {")
		out.append("\"data_sets\":"+str(len(self.segments))+",")
		i=0
		for s in self.segments:
			s.base_name="data_set"+str(i)
			out.extend(s.gen_json())
			out[-1]=out[-1]+","
			i=i+1
		out[-1]=out[-1][:-1]
		out.append("}")

		return out

	def load_from_json(self,json):
		self.segments=[]
		segs=json['data_sets']
		for i in range(0,segs):
			a=json_data_set()
			data_set="data_set"+str(i)
			a.load_from_json(json[data_set])
			self.segments.append(a)

class json_fits(json_base):

	def __init__(self):
		json_base.__init__(self,"fits")
		self.var_list=[]
		self.var_list.append(["fit_config",json_fit_config()])
		self.var_list.append(["duplicate",json_fit_duplicate()])
		self.var_list.append(["vars",json_fit_vars()])
		self.var_list.append(["rules",json_fit_rules()])
		self.var_list.append(["fits",json_all_fits()])
		self.var_list_build()

