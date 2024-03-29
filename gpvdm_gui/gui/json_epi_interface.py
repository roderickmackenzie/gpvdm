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


## @package json_epi_interface
#  Store the interface information for the interface
#

from json_base import json_base


class json_epi_interface(json_base):

	def __init__(self):
		json_base.__init__(self,"layer_interface")
		self.var_list=[]
		self.var_list.append(["interface_model","none"])
		self.var_list.append(["interface_eh_tau",1e-15])

		self.var_list.append(["interface_tunnel_e",False])
		self.var_list.append(["interface_Ge",1e-15])

		self.var_list.append(["interface_tunnel_h",False])
		self.var_list.append(["interface_Gh",1e-15])

		self.var_list.append(["interface_left_doping_enabled",False])
		self.var_list.append(["interface_left_doping",1e20])
		self.var_list.append(["interface_right_doping_enabled",False])
		self.var_list.append(["interface_right_doping",1e20])
		self.var_list_build()


