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


## @package json_light
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_light_spectrum(json_base):

	def __init__(self):
		json_base.__init__(self,"light_spectrum")
		self.var_list=[]
		self.var_list.append(["light_spectrum","AM1.5G"])
		self.var_list.append(["light_multiplyer",1.0])
		self.var_list_build()


class json_light_external_interface(json_base):

	def __init__(self):
		json_base.__init__(self,"external_interface")
		self.var_list=[]
		self.var_list.append(["enabled",False])
		self.var_list.append(["light_external_n",1.0])
		self.var_list_build()


class json_light_spectra(json_base):

	def __init__(self):
		json_base.__init__(self,"light_spectra",segment_class=True,segment_example=json_light_spectrum())


class json_filter_spectrum(json_base):

	def __init__(self):
		json_base.__init__(self,"light_filter")
		self.var_list=[]
		self.var_list.append(["filter_enabled",False])
		self.var_list.append(["filter_material","glasses/glass"])
		self.var_list.append(["filter_invert",True])
		self.var_list.append(["filter_db",1000.0])

		self.var_list_build()

class json_light_filters(json_base):

	def __init__(self):
		json_base.__init__(self,"light_filters",segment_class=True,segment_example=json_filter_spectrum())

class json_virtual_spectra(json_base):

	def __init__(self,name):
		json_base.__init__(self,name)
		self.var_list=[]
		self.var_list.append(["light_spectra",json_light_spectra()])
		self.var_list.append(["light_filters",json_light_filters()])
		self.var_list.append(["external_interface",json_light_external_interface()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_light(json_base):

	def __init__(self):
		json_base.__init__(self,"light")
		self.var_list=[]
		self.var_list.append(["light_model","full"])
		self.var_list.append(["Psun",1.0])
		self.var_list.append(["sun","AM1.5G"])
		self.var_list.append(["alignmesh",0])
		self.var_list.append(["meshpoints",200])
		self.var_list.append(["light_wavelength_auto_mesh",False])
		self.var_list.append(["electron_eff",1.0])
		self.var_list.append(["hole_eff",1.0])
		self.var_list.append(["Dphotoneff",8.718361e-01])
		self.var_list.append(["NDfilter",0.000000e+00])
		self.var_list.append(["light_flat_generation_rate",2e28])
		self.var_list.append(["light_file_generation","Gn.inp"])
		self.var_list.append(["light_file_qe_spectra",""])
		self.var_list.append(["light_file_generation_shift",200e-9])
		self.var_list.append(["light_profile","box"])
		self.var_list.append(["dump_verbosity",10])
		self.var_list_build()
		self.latex_banned=["all"]
		self.latex_allowed=["Dphotoneff"]
