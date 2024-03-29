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


## @package json_emission_db_item
#  Store the shape information
#


from json_base import json_base
from json_import_config import json_import_config

class json_filter_db_item(json_base):

	def __init__(self):
		json_base.__init__(self,"json_filter_db_item")
		self.var_list=[]
		self.var_list.append(["item_type","filter"])
		self.var_list.append(["color_r",0.8])
		self.var_list.append(["color_g",0.8])
		self.var_list.append(["color_b",0.8])
		self.var_list.append(["color_alpha",0.8])
		self.var_list.append(["status","private"])
		self.var_list.append(["changelog",""])
		self.var_list.append(["mat_src",""])
		self.var_list.append(["filter_import",json_import_config(name="filter_import")])
		self.var_list_build()
		self.include_name=False
