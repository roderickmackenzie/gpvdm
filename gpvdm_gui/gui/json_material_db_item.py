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


## @package json_shape_db_item
#  Store the shape information
#


from json_base import json_base
from json_import_config import json_import_config

class json_material_db_electrical_constants(json_base):

	def __init__(self):
		json_base.__init__(self,"electrical_constants")
		self.var_list=[]
		self.var_list.append(["material_blend",False])
		self.var_list.append(["Xi0",-3.0])
		self.var_list.append(["Eg0",1.0])
		self.var_list.append(["Xi1",-3.0])
		self.var_list.append(["Eg1",1.0])
		self.var_list_build()

class json_material_db_thermal_constants(json_base):

	def __init__(self):
		json_base.__init__(self,"thermal_constants")
		self.var_list=[]
		self.var_list.append(["thermal_kl",1.0])
		self.var_list.append(["thermal_tau_e",1.0000e-8])
		self.var_list.append(["thermal_tau_h",1.0000e-9])
		self.var_list_build()

class json_material_db_item(json_base):

	def __init__(self):
		json_base.__init__(self,"material_db_item")
		self.var_list=[]
		self.var_list.append(["item_type","material"])
		self.var_list.append(["color_r",0.8])
		self.var_list.append(["color_g",0.8])
		self.var_list.append(["color_b",0.8])
		self.var_list.append(["color_alpha",0.8])
		self.var_list.append(["material_type","other"])
		self.var_list.append(["status","public"])
		self.var_list.append(["changelog",""])
		self.var_list.append(["mat_src",""])
		self.var_list.append(["n_import",json_import_config(name="n_import")])
		self.var_list.append(["alpha_import",json_import_config(name="alpha_import")])
		self.var_list.append(["electrical_constants",json_material_db_electrical_constants()])
		self.var_list.append(["thermal_constants",json_material_db_thermal_constants()])
		self.var_list_build()
		self.include_name=False
