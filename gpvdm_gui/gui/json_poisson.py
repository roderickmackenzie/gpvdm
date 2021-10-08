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


## @package json_math
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base

class json_poisson(json_base):
	def __init__(self):
		json_base.__init__(self,"poisson")
		self.var_list=[]
		self.var_list.append(["posclamp",1.0])
		self.var_list.append(["pos_max_ittr",100])
		self.var_list.append(["pos_min_error",1e-4])
		self.var_list.append(["math_enable_pos_solver",True])
		self.var_list.append(["dump_print_pos_error",False])
		self.var_list.append(["pos_dump_verbosity",0])
		self.var_list_build()


