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


## @package json_transfer_matrix
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_fdtd_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"fdtd_segment")
		self.var_list=[]
		self.var_list.append(["english_name","FDTD"])
		self.var_list.append(["icon","fdtd"])
		self.var_list.append(["text_excitation",""])
		self.var_list.append(["fdtd_excitation_type","sin"])
		self.var_list.append(["fdtd_lambda_start",520e-9])
		self.var_list.append(["fdtd_lambda_stop",525e-9])
		self.var_list.append(["fdtd_lambda_points",1])
		self.var_list.append(["fdtd_pulse_length",100])
		self.var_list.append(["fdtd_excite_Ex",False])
		self.var_list.append(["fdtd_excite_Ey",True])
		self.var_list.append(["fdtd_excite_Ez",False])

		self.var_list.append(["text_fdtd_time",""])
		self.var_list.append(["fdtd_max_time",0.1])
		self.var_list.append(["fdtd_max_steps",10000])
		self.var_list.append(["lam_jmax",12])
		self.var_list.append(["text_fdtd_mesh",""])
		self.var_list.append(["fdtd_xzy","zy"])
		self.var_list.append(["fdtd_zlen",60])
		self.var_list.append(["fdtd_xlen",1])
		self.var_list.append(["fdtd_ylen",60])
		self.var_list.append(["use_gpu",False])

		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_fdtd(json_base):

	def __init__(self):
		json_base.__init__(self,"fdtd",segment_class=True,segment_example=json_fdtd_simulation())


