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


## @package json_exciton
#  Store the exciton data
#


import sys
import os
import shutil
import json
from json_base import json_base

class json_exciton_config(json_base):

	def __init__(self):
		json_base.__init__(self,"exciton_config")
		self.var_list=[]
		self.var_list.append(["test","JV\\ncurve"])
		self.var_list_build()

class json_exciton_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"exciton_segment")
		self.var_list=[]
		self.var_list.append(["english_name","Exciton"])
		self.var_list.append(["icon","exciton"])
		self.var_list.append(["config",json_exciton_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_exciton(json_base):

	def __init__(self):
		json_base.__init__(self,"exciton",segment_class=True,segment_example=json_exciton_simulation())
		self.var_list=[]
		self.var_list.append(["exciton_enabled",False])
		self.var_list.append(["exciton_max_ittr",20])
		self.var_list.append(["exciton_min_error",1e-7])
		self.var_list.append(["dump_verbosity",1])
		self.var_list.append(["solver_verbosity","solver_verbosity_nothing"])
		self.var_list_build()

class json_exciton_boundary(json_base):

	def __init__(self):
		json_base.__init__(self,"exciton_boundary")
		self.var_list=[]
		self.var_list.append(["y0_boundry","neumann"])
		self.var_list.append(["n_y0",1e24])
		self.var_list.append(["excitonsink_y0",100.0])
		self.var_list.append(["excitonsink_length_y0",1e-3])
		self.var_list.append(["y1_boundry","heatsink"])
		self.var_list.append(["n_y1",1e24])
		self.var_list.append(["excitonsink_y1",0.1])
		self.var_list.append(["excitonsink_length_y1",1e-2])
		self.var_list.append(["x0_boundry","neumann"])
		self.var_list.append(["n_x0",1e24])
		self.var_list.append(["excitonsink_x0",0.1])
		self.var_list.append(["excitonsink_length_x0",0.1])
		self.var_list.append(["x1_boundry","neumann"])
		self.var_list.append(["n_x1",1e24])
		self.var_list.append(["excitonsink_x1",200])
		self.var_list.append(["excitonsink_length_x1",0.1])
		self.var_list.append(["z0_boundry","neumann"])
		self.var_list.append(["n_z0",1e24])
		self.var_list.append(["excitonsink_z0",200])
		self.var_list.append(["excitonsink_length_z0",0.1])
		self.var_list.append(["z1_boundry","neumann"])
		self.var_list.append(["n_z1",1e24])
		self.var_list.append(["excitonsink_z1",200])
		self.var_list.append(["excitonsink_length_z1",0.1])
		self.var_list_build()


