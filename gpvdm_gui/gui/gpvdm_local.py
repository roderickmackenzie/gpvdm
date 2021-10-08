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


## @package gpvdm_local
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base
from cal_path import get_user_settings_dir
from json_cluster import json_cluster

class json_save_window(json_base):

	def __init__(self):
		json_base.__init__(self,"save_window")
		self.var_list=[]
		self.var_list.append(["name","none"])
		self.var_list.append(["x",-1])
		self.var_list.append(["y",-1])
		self.var_list_build()

class json_save_windows(json_base):

	def __init__(self):
		json_base.__init__(self,"windows",segment_class=True)

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_save_window()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

class json_icon_lib(json_base):

	def __init__(self):
		json_base.__init__(self,"json_icon_lib")
		self.var_list=[]
		self.var_list.append(["text-x-generic","text-x-generic"])
		self.var_list.append(["wps-office-xls","wps-office-xls"])
		self.var_list.append(["info","info_file"])
		self.var_list.append(["text-x-generic","text-x-generic"])
		self.var_list.append(["wps-office-xls","wps-office-xls"])
		self.var_list.append(["spectra","spectra_file"])
		self.var_list.append(["organic_material","organic_material"])
		self.var_list.append([".png","image-png"])
		self.var_list.append([".gpvdm","si"])
		self.var_list.append([".xlsx","wps-office-xls"])
		self.var_list.append([".pdf","pdf"])
		self.var_list.append(["desktop","folder"])
		self.var_list.append(["constraints","dat_file"])
		self.var_list_build()

class json_gui_config(json_base):

	def __init__(self):
		json_base.__init__(self,"gui_config")
		self.var_list=[]
		self.var_list.append(["enable_opengl",True])
		self.var_list.append(["gui_use_icon_theme",False])
		self.var_list.append(["matlab_interpreter","octave"])
		self.var_list.append(["enable_webbrowser",False])
		self.var_list.append(["enable_betafeatures",False])
		self.var_list_build()

class json_international(json_base):

	def __init__(self):
		json_base.__init__(self,"international")
		self.var_list=[]
		self.var_list.append(["lang","auto"])
		self.var_list_build()

class json_local(json_base):

	def __init__(self):
		json_base.__init__(self,"local")
		self.var_list=[]
		self.var_list.append(["international",json_international()])
		self.var_list.append(["gui_config",json_gui_config()])
		self.var_list.append(["icon_lib",json_icon_lib()])
		self.var_list.append(["cluster",json_cluster()])
		self.var_list.append(["windows",json_save_windows()])
		self.var_list_build()
		self.include_name=False

my_data=json_local()
my_data.load(os.path.join(get_user_settings_dir(),"data.json"))
if my_data.loaded==True:
	my_data.save()

my_data.loaded=True

def gpvdm_local():
	return my_data
