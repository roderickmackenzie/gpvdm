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


## @package gpvdm_data
#  Used to store json data
#


import sys
import os
import shutil
import json
from inp import inp
from epitaxy_class import epitaxy
from json_base import json_base
from json_time_domain import json_time_domain
from json_fx_domain import json_fx_domain
from json_cv import json_cv
from json_jv import json_jv
from json_pl import json_pl
from json_transfer_matrix import json_transfer_matrix
from json_mesh import json_mesh
from json_fit import json_fits
from json_suns_jsc import json_suns_jsc
from json_suns_voc import json_suns_voc
from json_ce import json_ce
from json_eqe import json_eqe
from json_fdtd import json_fdtd
from json_ray import json_ray
from json_spm import json_spm
from json_math import json_math
from json_hard_limit import json_hard_limit
from json_perovskite import json_perovskite
from json_server import json_server
from json_thermal import json_thermal
from json_thermal import json_thermal_boundary
from json_exciton import json_exciton
from json_exciton import json_exciton_boundary
from json_electrical_solver import json_electrical_solver
from json_spctral2 import json_spctral2
from json_lasers import json_lasers
from json_circuit import json_circuit
from json_opengl_config import json_opengl_config
from json_gl import json_gl
from json_light import json_light
from json_light_sources import json_light_sources
from json_world import json_world
from json_world_stats import json_world_stats
from json_detectors import json_detectors
from json_ml import json_ml
from json_dump import json_dump
from json_singlet import json_singlet


class json_parasitic(json_base):

	def __init__(self):
		json_base.__init__(self,"parasitic")
		self.var_list=[]
		self.var_list.append(["Rshunt",1300.0])
		self.var_list.append(["Rcontact",1.439413e+01])
		self.var_list.append(["otherlayers",0.0])
		self.var_list.append(["Rshort",1.0])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()

class json_sim(json_base):

	def __init__(self):
		json_base.__init__(self,"sim")
		self.var_list=[]
		self.var_list.append(["simmode","jv@jv"])
		self.var_list.append(["stoppoint",0])
		self.var_list.append(["version","v7.88.17"])
		self.var_list.append(["notes",""])
		self.var_list.append(["first_sim_message",""])
		self.var_list.append(["use_gpvdm_local",True])
		self.var_list.append(["opengl",json_opengl_config()])
		self.var_list_build()

class all_gpvdm_data(json_base,json_world_stats):
	def __init__(self):
		self.loaded=False
		json_base.__init__(self,"main")
		json_world_stats.__init__(self)
		self.var_list=[]
		self.var_list.append(["sim",json_sim()])
		self.var_list.append(["jv",json_jv()])
		self.var_list.append(["dump",json_dump()])
		self.var_list.append(["math",json_math()])
		self.var_list.append(["light",json_light()])
		self.var_list.append(["light_sources",json_light_sources()])
		self.var_list.append(["detectors",json_detectors()])
		self.var_list.append(["server",json_server()])
		self.var_list.append(["epitaxy",epitaxy()])
		self.var_list.append(["thermal",json_thermal()])
		self.var_list.append(["thermal_boundary",json_thermal_boundary()])
		self.var_list.append(["exciton",json_exciton()])
		self.var_list.append(["exciton_boundary",json_exciton_boundary()])
		self.var_list.append(["ray",json_ray()])
		self.var_list.append(["suns_voc",json_suns_voc()])
		self.var_list.append(["suns_jsc",json_suns_jsc()])
		self.var_list.append(["ce",json_ce()])
		self.var_list.append(["transfer_matrix",json_transfer_matrix()])
		self.var_list.append(["pl_ss",json_pl()])
		self.var_list.append(["eqe",json_eqe()])
		self.var_list.append(["fdtd",json_fdtd()])
		self.var_list.append(["fits",json_fits()])
		self.var_list.append(["mesh",json_mesh()])
		self.var_list.append(["time_domain",json_time_domain()])
		self.var_list.append(["fx_domain",json_fx_domain()])
		self.var_list.append(["cv",json_cv()])
		self.var_list.append(["parasitic",json_parasitic()])
		self.var_list.append(["spm",json_spm()])
		self.var_list.append(["hard_limit",json_hard_limit()])
		self.var_list.append(["perovskite",json_perovskite()])
		self.var_list.append(["singlet",json_singlet()])
		self.var_list.append(["electrical_solver",json_electrical_solver()])
		self.var_list.append(["spctral2",json_spctral2()])
		self.var_list.append(["lasers",json_lasers()])
		self.var_list.append(["circuit",json_circuit()])
		self.var_list.append(["gl",json_gl()])
		self.var_list.append(["world",json_world()])
		self.var_list.append(["ml",json_ml()])
		self.var_list.append(["icon","icon"])
		self.var_list.append(["english_name","Simulation"])
		self.var_list.append(["hidden",False])
		self.var_list.append(["password",""])
		self.var_list.append(["status","open"])

		#public
		#private

		self.var_list_build()

		#short hands
		self.epi=self.epitaxy
		self.include_name=False
		self.auto_reload=False
		self.last_time=-1
		self.call_backs=[]
		

	def check_reload(self):
		if self.last_time!=-1:
			if self.loaded==True:
				#print("check",self.last_time,self.f.time())
				if self.last_time!=self.f.time():
					#print("reloading")
					self.reload()
					for f in self.call_backs:
						f()

	def add_call_back(self,function):
		if function not in self.call_backs:
			self.call_backs.append(function)

	def remove_call_back(self,function):
		if function in self.call_backs:
			self.call_backs.remove(function)


my_data=all_gpvdm_data()

def gpvdm_data():
	return my_data

#
