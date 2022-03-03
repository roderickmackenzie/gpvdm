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

## @package cal_path
#  Calculate the where files are, and if you can't find them look harder.
#

import sys
import os
#import shutil
from pathlib import Path
from win_lin import running_on_linux
from gui_enable import gui_get
from os.path import expanduser
from str2bool import str2bool
import json

root_materials_path=None
plugins_path=None
exe_command=None
share_path=None
device_lib_path=None
bin_path=None
lib_path=None
image_path=None
css_path=None
flag_path=None
lang_path=None
src_path=None
spectra_base_path=None
sim_path=None
materials_base_path=None
emission_base_path=None
atmosphere_path=None
shape_base_path=None
scripts_base_path=None
html_path=None
bib_path=None
fonts_path=None
video_path=None
cluster_path=None
cluster_libs_path=None
home_path=None
use_gpvdm_local=None
components_path=None
filters_base_path=None

def subtract_paths(root,b_in):
	a=root.replace("/","\\")
	b=b_in.replace("/","\\")
	a=a.split("\\")
	b=b.split("\\")
	a_len=len(a)
	b_len=len(b)
	m=a_len
	if b_len<m:
		m=b_len
	pos=0

	for i in range(0,m):
		if a[i]!=b[i]:
			break
		pos=pos+1

	ret=[]
	for i in range(pos,b_len):
		ret.append(b[i])

	return "/".join(ret)

def to_native_path(path):
	ret=path
	if running_on_linux()==False:
		ret=ret.replace("/","\\")
		ret=ret.lower()
	return ret

def remove_cwdfrompath(path):
	tmp=path
	if tmp.startswith(os.getcwd()):
		tmp=tmp[len(os.getcwd())+1:]
	return tmp

def remove_simpathfrompath(path):
	tmp=path
	if tmp.startswith(get_sim_path()):
		tmp=tmp[len(get_sim_path())+1:]
	return tmp

def join_path(one,two):
	output_file=os.path.join(one,two)

	if two[0]=='/':
		if one!="" :
			output_file=os.path.join(one,two[1:])

	return output_file


def cal_share_path():
	global share_path

	if os.path.isfile("configure.ac"):
		share_path=os.getcwd()
		return

	if os.path.isfile("ver.py"):
		share_path=os.path.abspath(os.path.join(os.getcwd(), os.pardir))
		return

	if running_on_linux()==False:
		if os.path.isfile(os.path.join(os.getcwd(),"gpvdm_core.exe")):
			share_path=os.getcwd()
		else:
			share_path="c:\\gpvdm"

	else:
		if os.path.isdir("/usr/lib64/gpvdm"):
			share_path="/usr/lib64/gpvdm/"
		elif os.path.isdir("/usr/lib/gpvdm"):
			share_path="/usr/lib/gpvdm/"
		else:
			share_path=os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
			print("I don't know where the shared files are assuming ",share_path)

def search_known_paths(file_or_dir_to_find,ext,key_file,is_file):
	global share_path
	global bin_path
	root_dir_rel_to_script_for_source_code=os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))	#gpvdm.py ../../ assuming it's in the right palce

	#print(root_dir_rel_to_script)
	#check cwd
	paths=[]
	for ex in ext:
		paths.append(os.path.join(os.getcwd(),file_or_dir_to_find)+ex)
		#chk cwd and in the gpvdm_* directories
		paths.append(os.path.join(os.getcwd(),"gpvdm_data",file_or_dir_to_find)+ex)
		paths.append(os.path.join(os.getcwd(),"gpvdm_gui",file_or_dir_to_find)+ex)
		paths.append(os.path.join(os.getcwd(),"gpvdm_core",file_or_dir_to_find)+ex)
		paths.append(os.path.join("C:\\Program Files\\gpvdm",file_or_dir_to_find)+ex)
		paths.append(os.path.join("D:\\Program Files\\gpvdm",file_or_dir_to_find)+ex)
		#check where the root directory for a normal install
		paths.append(os.path.join(root_dir_rel_to_script_for_source_code,"gpvdm_data",file_or_dir_to_find)+ex)
		paths.append(os.path.join(root_dir_rel_to_script_for_source_code,"gpvdm_gui",file_or_dir_to_find)+ex)
		paths.append(os.path.join(root_dir_rel_to_script_for_source_code,"gpvdm_core",file_or_dir_to_find)+ex)



		paths.append(os.path.join(os.path.abspath(os.path.join(os.getcwd(), os.pardir)),file_or_dir_to_find)+ex)

		paths.append(os.path.join(bin_path,file_or_dir_to_find)+ex)
		paths.append(os.path.join(get_sim_path(),file_or_dir_to_find)+ex)
		if running_on_linux()==True:
			paths.append(os.path.join("/usr/share/gpvdm/",file_or_dir_to_find)+ex)
			paths.append(os.path.join("/usr/local/bin/",file_or_dir_to_find)+ex)
			paths.append(os.path.join("/usr/bin/",file_or_dir_to_find)+ex)

	for item in paths:
		#print(item,os.path.isfile(item),os.path.dirname(os.path.realpath(__file__)))
		if key_file==None:
			if os.path.isdir(item) and is_file==False:
				#print "found",item
				return to_native_path(item)
			if os.path.isfile(item) and is_file==True:
				#print "found",item
				return to_native_path(item)

		else:
			if os.path.isfile(os.path.join(item,key_file)) and is_file==True:
				return to_native_path(item)

	#print "Can't find",file_or_dir_to_find, "setting it to",paths[0]
	return paths[2]

def cal_bin_path():
	global bin_path
	if running_on_linux()==True:
			bin_path="/bin/"
	else:
			bin_path=share_path

def test_arg_for_sim_file():
	if len(sys.argv)>1:
		f=os.path.realpath(sys.argv[1])
		if os.path.isfile(f)==True and f.endswith("sim.gpvdm"):
			return os.path.dirname(f)
		elif os.path.isdir(f)==True and os.path.isfile(os.path.join(f,"sim.gpvdm"))==True:
			return f
	return False

def calculate_paths_init():
	set_sim_path(os.getcwd())
	path=test_arg_for_sim_file()
	if path!=False:
		set_sim_path(path)

	cal_share_path()
	cal_bin_path()

def get_icon_path(name,size=-1):
	global image_path
	if name.endswith(".png"):
		name=name[:-4]

	if size==-1:
		path=os.path.join(image_path,"64x64",name+".png")
		return path

	return os.path.join(image_path,str(size)+"x"+str(size),name+".png")


def calculate_paths():
	global share_path
	global lib_path
	global exe_command
	global device_lib_path
	global root_materials_path
	global image_path
	global css_path
	global flag_path	
	global plugins_path
	global lang_path
	global src_path
	global ui_path
	global spectra_base_path
	global materials_base_path
	global emission_base_path
	global shape_base_path
	global scripts_base_path
	global html_path
	global bib_path
	global fonts_path
	global video_path
	global cluster_path
	global cluster_libs_path
	global atmosphere_path
	global components_path
	global inp_template_path
	global filters_base_path

	root_materials_path=os.path.join(get_sim_path(),"materials")
	if os.path.isdir(root_materials_path)==False:
		root_materials_path=search_known_paths("materials",[""],None,False)
	materials_base_path=search_known_paths("materials",[""],None,False)
	emission_base_path=search_known_paths("emission",[""],None,False)
	shape_base_path=search_known_paths("shape",[""],None,False)
	filters_base_path=search_known_paths("filters",[""],None,False)
	scripts_base_path=search_known_paths("scripts",[""],None,False)
	atmosphere_path=search_known_paths("atmosphere",[""],None,False)

	device_lib_path=search_known_paths("device_lib",[""],None,False)
	plugins_path=search_known_paths("plugins",[""],None,False)
	image_path=search_known_paths("images",[""],"image.jpg",True)
	css_path=search_known_paths("css",[""],"style.css",True)
	flag_path=search_known_paths("flags",[""],"gb.png",True)
	lang_path=search_known_paths("lang",[""],None,False)
	exe_command=search_known_paths("gpvdm_core",["",".exe",".o"],None,True)

	src_path=os.path.dirname(search_known_paths("Makefile",[".am"],None,True))
	ui_path=search_known_paths("ui",[""],None,False)
	spectra_base_path=search_known_paths("spectra",[""],None,False)
	cluster_path=search_known_paths("cluster",[""],None,False)
	cluster_libs_path=search_known_paths("cluster_libs",[""],None,False)


	html_path=search_known_paths("html",[""],"info0.html",True)
	bib_path=search_known_paths("bib",[""],"cite.bib",True)
	fonts_path=search_known_paths("fonts",[""],"LiberationSans-Regular.ttf",True)
	video_path=search_known_paths("video",[""],"welcome.wmv",True)
	components_path=search_known_paths("components",[""],"resistor.inp",True)
	inp_template_path=search_known_paths("inp_template",[""],"server.inp",True)

def get_license_path():
	return get_exe_path()

def get_cluster_path():
	global cluster_path
	return cluster_path

def get_cluster_libs_path():
	global cluster_libs_path
	return cluster_libs_path

def get_share_path():
	global share_path
	return share_path

def get_src_path():
	global src_path
	return src_path

def get_base_spectra_path():
	global spectra_base_path
	return spectra_base_path

def get_spectra_path():
	return os.path.join(get_user_settings_dir(),"spectra")

def get_scripts_path():
	return os.path.join(get_user_settings_dir(),"scripts")


def get_user_settings_dir():
	global use_gpvdm_local
	if use_gpvdm_local==None:
		use_gpvdm_local=True

		file_path=os.path.join(os.getcwd(),"sim.json")

		if os.path.isfile(file_path):
			f=open(file_path, mode='r')
			read_lines = f.read()
			f.close()

			data=json.loads(read_lines)
			try:
				use_gpvdm_local=str2bool(data["sim"]["use_gpvdm_local"])
			except:
				pass

	if use_gpvdm_local==True:
		ret=os.path.join(get_home_path(),"gpvdm_local")
		if os.path.isdir(ret)==False:
			os.makedirs(ret)
	else:
		return os.getcwd()
	return ret


def get_web_cache_path():
	ret=os.path.join(get_user_settings_dir(),"web_cache")

	if os.path.isdir(ret)==False:
		os.makedirs(ret)

	return ret

def get_cache_path():
	ret=os.path.join(get_user_settings_dir(),"cache")

	if os.path.isdir(ret)==False:
		os.makedirs(ret)

	return ret

def get_html_path():
	global html_path
	return html_path


def get_fonts_path():
	global fonts_path
	return fonts_path

#dont use any more
def get_materials_path():
	return os.path.join(get_user_settings_dir(),"materials")

def get_emission_path():
	return os.path.join(get_user_settings_dir(),"emission")

def get_user_data_path():
	ret=os.path.join(get_user_settings_dir(),"user_data")
	if os.path.isdir(ret)==False:
		os.makedirs(ret)
	return ret


def get_base_material_path():
	global materials_base_path
	return materials_base_path

def get_atmosphere_path():
	global atmosphere_path
	return atmosphere_path

def get_base_emission_path():
	global emission_base_path
	return emission_base_path

def get_base_shape_path():
	global shape_base_path
	return shape_base_path

def get_base_scripts_path():
	global scripts_base_path
	return scripts_base_path


def get_default_material_path():
	global materials_base_path
	return os.path.join(materials_base_path,"generic","default")

def get_device_lib_path():
	global device_lib_path
	return device_lib_path

def get_bin_path():
	global bin_path
	return bin_path

def get_plugins_path():
	global plugins_path
	return plugins_path

def get_exe_path():
	global exe_command
	ret=os.path.dirname(exe_command)
	return ret

def get_exe_command():
	global exe_command
	return exe_command

def get_exe_name():
	global exe_command
	return os.path.basename(exe_command)

def get_components_path():
	global components_path
	return components_path


def get_image_file_path():
	global image_path
	return image_path

def get_css_path():
	global css_path
	return css_path

class gpvdm_paths:

	def get_inp_template_path():
		global inp_template_path
		return inp_template_path

	def is_plugin(name):
		global plugins_path
		if os.path.isfile(os.path.join(plugins_path,name+".dll"))==True:
			return True

		if os.path.isfile(os.path.join(plugins_path,name+".so"))==True:
			return True

		return False

	def get_shape_template_path():
		global inp_template_path
		return os.path.join(inp_template_path,"shape")

	def get_shape_path():
		return os.path.join(get_user_settings_dir(),"shape")

	def get_bib_path():
		global bib_path
		return bib_path

	def get_tmp_path():
		ret=os.path.join(get_user_settings_dir(),"tmp")

		if os.path.isdir(ret)==False:
			os.makedirs(ret)

		return ret

	def get_use_gpvdm_local():
		global use_gpvdm_local
		return use_gpvdm_local

	def get_materials_path():
		return os.path.join(get_user_settings_dir(),"materials")

	def get_sim_path():
		global sim_path
		if sim_path==None:
			return os.getcwd()
		return sim_path

	def get_filters_path():
		return os.path.join(get_user_settings_dir(),"filters")

	def get_base_filters_path():
		global filters_base_path
		return filters_base_path

	def get_spectra_path():
		return os.path.join(get_user_settings_dir(),"spectra")

	def am_i_rod():
		path=os.path.join(get_home_path(),"gpvdm_rod")
		if os.path.isfile(path):
			return True
		return False

def get_flag_file_path():
	global flag_path
	return flag_path

def get_lang_path():
	global lang_path
	return lang_path

def get_ui_path():
	global ui_path
	return ui_path

def set_sim_path(path):
	global sim_path
	sim_path=os.path.abspath(path)

def get_sim_path():
	global sim_path
	if sim_path==None:
		return os.getcwd()
	return sim_path

def set_sim_path(path):
	global sim_path
	sim_path=path

def get_backup_path():
	global sim_path
	path=os.getcwd()
	if sim_path!=None:
		path=sim_path

	backup_path=os.path.join(path,"backup")

	return backup_path

def get_exe_args():
	if gui_get()==True:
		return "--gui --html" #--english
	else:
		return ""

def get_home_path():
	global home_path
	if home_path==None:
		if running_on_linux()==True:
			home_path=str(Path.home())
		else:
			home_path=str(os.environ['USERPROFILE'])
			if home_path.endswith("NTUSER.DAT")==True:
				home_path=os.path.dirname(home_path)

	return home_path

	
def get_desktop_path():
	path=os.path.join(get_home_path(),"Desktop")
	if os.path.isdir(path):
		return path
	return False

def get_music_path():
	path=os.path.join(get_home_path(),"Music")
	if os.path.isdir(path):
		return path
	return False

def get_videos_path():
	path=os.path.join(get_home_path(),"Videos")
	if os.path.isdir(path):
		return path
	return False

def get_video_path():
	global video_path
	return video_path

def get_downloads_path():
	path=os.path.join(get_home_path(),"Downloads")
	if os.path.isdir(path):
		return path
	return False

def find_light_source(path=None):
	ret=[]

	if path==None:
		spectra_path=get_spectra_path()
	else:
		spectra_path=path

	for dirpath, dirnames, filenames in os.walk(spectra_path):
		for filename in [f for f in filenames if f=="data.json"]:
			path=os.path.join(dirpath, filename)
			path=os.path.dirname(path)
			s=os.path.relpath(path, spectra_path)
			s=s.replace("\\","/")
			ret.append(s)


	return ret


def multiplatform_exe_command(command):
	if running_on_linux()==False:
		if command.count(".exe")>0:
			command="\""+command
			command=command.replace(".exe",".exe\"",1)

	#print("exe command=",command)
	return command

