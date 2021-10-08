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


## @package json_suns_voc
#  Store the suns_voc json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_ce_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"ce_segment")
		self.var_list=[]
		self.var_list.append(["english_name","Charge\nExtraction"])
		self.var_list.append(["icon","ce"])
		self.var_list.append(["ce_start_sun",0.01])
		self.var_list.append(["ce_stop_sun",10.0])
		self.var_list.append(["ce_number_of_simulations",10.0])
		self.var_list.append(["ce_on_time",1e-6])
		self.var_list.append(["ce_off_time",1.0])
		self.var_list.append(["dump_verbosity",1])
		self.var_list.append(["load_type","open_circuit"])
		self.var_list.append(["dump_screen_verbosity","dump_verbosity_key_results"])
		self.var_list_build()


class json_ce(json_base):

	def __init__(self):
		json_base.__init__(self,"ce",segment_class=True,segment_example=json_ce_simulation())

