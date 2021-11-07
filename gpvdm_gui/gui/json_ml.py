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


## @package json_ml
#  Machine learning 
#


import sys
import os
import shutil
import json
from json_base import json_base


#############

class json_ml_random_item(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_random_item")
		self.var_list=[]
		self.var_list.append(["random_var_enabled",True])
		self.var_list.append(["json_var","one/two/three"])
		self.var_list.append(["human_var","one/two/three"])
		self.var_list.append(["min",0.0])
		self.var_list.append(["max",1.0])
		self.var_list.append(["random_distribution","log"])
		self.var_list_build()

class json_ml_random(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_random",segment_class=True,segment_example=json_ml_random_item())


####################

class json_ml_patch_item(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_patch")
		self.var_list=[]
		self.var_list.append(["ml_patch_enabled",True])
		self.var_list.append(["json_var","one/two/three"])
		self.var_list.append(["human_var","one/two/three"])
		self.var_list.append(["ml_patch_val","value"])
		self.var_list_build()


class json_ml_patch(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_patch",segment_class=True,segment_example=json_ml_patch_item())


###############

class json_ml_sims_item(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_sims_item")
		self.var_list=[]
		self.var_list.append(["ml_sim_enabled",True])
		self.var_list.append(["sim_name","segment0@jv"])
		self.var_list.append(["ml_patch",json_ml_patch()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()

class json_ml_sims(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_sims",segment_class=True,segment_example=json_ml_sims_item())


##############

class json_ml_config(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_config")
		self.var_list=[]
		self.var_list.append(["ml_number_of_archives",400])
		self.var_list.append(["ml_sims_per_archive",40])
		self.var_list.append(["ml_archive_path","cwd"])
		self.var_list.append(["ml_vector_file_name","vectors.json"])
		self.var_list_build()

class json_ml_segment(json_base):

	def __init__(self):
		json_base.__init__(self,"ml_segment")
		self.var_list=[]
		self.var_list.append(["english_name","ML"])
		self.var_list.append(["icon","ml"])
		self.var_list.append(["ml_random",json_ml_random()])
		self.var_list.append(["ml_patch",json_ml_patch()])
		self.var_list.append(["ml_sims",json_ml_sims()])
		self.var_list.append(["ml_config",json_ml_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_ml(json_base):

	def __init__(self):
		json_base.__init__(self,"ml",segment_class=True,segment_example=json_ml_segment())


