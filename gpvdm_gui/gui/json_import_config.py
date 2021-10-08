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


## @package json_import_config
#  Used to store json_base
#


from json_base import json_base

class json_import_config(json_base):

	def __init__(self,name="import_config"):
		json_base.__init__(self,name)
		self.var_list=[]
		self.var_list.append(["import_file_path","none"])
		self.var_list.append(["import_x_combo_pos",9])
		self.var_list.append(["import_data_combo_pos",5])
		self.var_list.append(["import_x_spin",0])
		self.var_list.append(["import_data_spin",1])
		self.var_list.append(["import_title","Voltage - J"])
		self.var_list.append(["import_xlabel","Voltage"])
		self.var_list.append(["import_data_label","J"])
		self.var_list.append(["import_area",0.104])
		self.var_list.append(["import_data_invert",False])
		self.var_list.append(["import_x_invert",False])
		self.var_list.append(["data_file","none"])
		self.var_list_build()


