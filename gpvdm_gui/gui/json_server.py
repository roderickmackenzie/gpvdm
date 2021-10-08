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


## @package json_server
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_server(json_base):

	def __init__(self):
		json_base.__init__(self,"server")
		self.var_list=[]
		self.var_list.append(["gpvdm_core_max_threads",0])
		self.var_list.append(["server_stall_time",2000])
		self.var_list.append(["server_exit_on_dos_error",1])
		self.var_list.append(["server_max_run_time",345600])
		self.var_list.append(["server_auto_cpus",0])
		self.var_list.append(["server_min_cpus",1])
		self.var_list.append(["server_steel",0])
		self.var_list.append(["cluster",1])
		self.var_list.append(["server_use_dos_disk_cache",True])
		self.var_list.append(["max_gpvdm_instances",0])
		self.var_list_build()


