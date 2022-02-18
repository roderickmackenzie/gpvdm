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


## @package json_dump
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_banned_file(json_base):

	def __init__(self):
		json_base.__init__(self,"banned_file")
		self.var_list=[]
		self.var_list.append(["banned_enabled","True"])
		self.var_list.append(["banned_file_name","jv.dat"])
		self.var_list_build()


class json_banned_files(json_base):

	def __init__(self):
		json_base.__init__(self,"banned_files",segment_class=True,segment_example=json_banned_file())

class json_dump(json_base):

	def __init__(self):
		json_base.__init__(self,"dump")
		self.var_list=[]
		self.var_list.append(["plot",False])
		self.var_list.append(["newton_dump",False])
		self.var_list.append(["dump_dynamic",False])
		self.var_list.append(["startstop",False])
		self.var_list.append(["plotfile","plot"])
		self.var_list.append(["start_stop_time",0.0])
		self.var_list.append(["dump_1d_slice_xpos",-1])
		self.var_list.append(["dump_1d_slice_zpos",-1])
		self.var_list.append(["dump_verbose_electrical_solver_results",True])
		self.var_list.append(["dump_write_converge",True])
		self.var_list.append(["dump_norm_time_to_one",False])
		self.var_list.append(["dump_norm_y_axis",False])
		self.var_list.append(["dump_print_hardware_info",0])
		self.var_list.append(["dump_write_out_band_structure",0])
		self.var_list.append(["dump_first_guess",False])
		self.var_list.append(["dump_log_level","screen_and_disk"])
		self.var_list.append(["dump_optical_probe",False])
		self.var_list.append(["dump_optical_probe_spectrum",False])
		self.var_list.append(["dump_print_text",0])
		self.var_list.append(["dump_info_text",True])
		self.var_list.append(["dump_ray_trace_map",False])
		self.var_list.append(["dump_use_cache",False])
		self.var_list.append(["dump_write_headers",True])
		self.var_list.append(["dump_remove_dos_cache",False])
		self.var_list.append(["dump_dynamic_pl_energy",False])
		self.var_list.append(["banned_files",json_banned_files()])
		self.var_list_build()






