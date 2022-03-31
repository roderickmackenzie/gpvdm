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

class json_math(json_base):

	def __init__(self):
		json_base.__init__(self,"math")
		self.var_list=[]
		self.var_list.append(["text_newton_first_itt",""])
		self.var_list.append(["maxelectricalitt_first",1000])
		self.var_list.append(["electricalclamp_first",0.1])
		self.var_list.append(["math_electrical_error_first",1e-9])

		self.var_list.append(["text_newton_later_itt",""])
		self.var_list.append(["maxelectricalitt",100])
		self.var_list.append(["electricalclamp",1.0])
		self.var_list.append(["electricalerror",1e-8])

		self.var_list.append(["text_newton_exit_strategy",""])
		self.var_list.append(["newton_clever_exit",True])
		self.var_list.append(["newton_min_itt",5])
		self.var_list.append(["remesh",0])
		self.var_list.append(["newmeshsize",8])
		self.var_list.append(["kl_in_newton",1])

		self.var_list.append(["text_newton_solver_type",""])
		self.var_list.append(["solver_name","umfpack"])
		self.var_list.append(["newton_name","newton"])
		self.var_list.append(["complex_solver_name","complex_umfpack"])
		self.var_list.append(["math_t0",300.0])
		self.var_list.append(["math_d0",243.75])
		self.var_list.append(["math_n0",1e20])

		self.var_list.append(["text_newton_output",""])
		self.var_list.append(["math_newton_cache",False])
		self.var_list.append(["math_dynamic_mesh",False])
		self.var_list.append(["cache_path","/home/rod/cache"])
		self.var_list.append(["math_stop_on_convergence_problem",False])
		self.var_list.append(["math_stop_on_inverted_fermi_level",False])
		self.var_list.append(["solver_verbosity","solver_verbosity_at_end"])
		self.var_list_build()

