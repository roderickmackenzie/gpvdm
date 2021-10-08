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


## @package json_thermal
#  Store the thermal data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_thermal(json_base):

	def __init__(self):
		json_base.__init__(self,"thermal")
		self.var_list=[]
		self.var_list.append(["thermal",False])
		self.var_list.append(["thermal_model_type","thermal_lattice"])
		self.var_list.append(["thermal_l","true"])
		self.var_list.append(["thermal_e","false"])
		self.var_list.append(["thermal_h","false"])
		self.var_list.append(["nofluxl",1])
		self.var_list.append(["thermal_max_ittr",20])
		self.var_list.append(["thermal_min_error",1e-7])
		self.var_list.append(["joule_heating",True])
		self.var_list.append(["parasitic_heating",True])
		self.var_list.append(["thermal_couple_to_electrical_solver",True])
		self.var_list.append(["recombination_heating",False])
		self.var_list.append(["optical_heating",False])
		self.var_list.append(["dump_verbosity",1])
		self.var_list.append(["solver_verbosity","solver_verbosity_nothing"])
		self.var_list_build()

class json_thermal_boundary(json_base):

	def __init__(self):
		json_base.__init__(self,"thermal_boundary")
		self.var_list=[]
		self.var_list.append(["Ty0_boundry","neumann"])
		self.var_list.append(["Ty0",300.0])
		self.var_list.append(["heatsink_y0",100.0])
		self.var_list.append(["heatsink_length_y0",1e-3])
		self.var_list.append(["Ty1_boundry","heatsink"])
		self.var_list.append(["Ty1",300.0])
		self.var_list.append(["heatsink_y1",0.1])
		self.var_list.append(["heatsink_length_y1",1e-2])
		self.var_list.append(["Tx0_boundry","neumann"])
		self.var_list.append(["Tx0",300.0])
		self.var_list.append(["heatsink_x0",0.1])
		self.var_list.append(["heatsink_length_x0",0.1])
		self.var_list.append(["Tx1_boundry","neumann"])
		self.var_list.append(["Tx1",300.0])
		self.var_list.append(["heatsink_x1",200])
		self.var_list.append(["heatsink_length_x1",0.1])
		self.var_list.append(["Tz0_boundry","neumann"])
		self.var_list.append(["Tz0",300.0])
		self.var_list.append(["heatsink_z0",200])
		self.var_list.append(["heatsink_length_z0",0.1])
		self.var_list.append(["Tz1_boundry","neumann"])
		self.var_list.append(["Tz1",300.0])
		self.var_list.append(["heatsink_z1",200])
		self.var_list.append(["heatsink_length_z1",0.1])
		self.var_list_build()


