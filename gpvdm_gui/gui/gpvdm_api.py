# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2012-2017 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#
#   https://www.gpvdm.com
#   Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
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
# 


## @package gpvdm_api
#  An api used to run gpvdm
#

#import sys
import os
import sys
import shutil
from shutil import copyfile
from cal_path import calculate_paths
from cal_path import calculate_paths_init
from cal_path import set_sim_path

calculate_paths_init()
calculate_paths()

from server import base_server
from server import server_get
from cal_path import get_sim_path
from progress_class import progress_class
from spectral2 import spectral2
from inp import inp_update_token_value
from inp import inp_get_token_value
from inp import inp

#scan
from scan_io import scan_build_nested_simulation
from scan_io import scan_io
from scans_io import scans_io

from cal_path import get_exe_command
from scan_tree import tree_load_flat_list
from gui_enable import set_gui
from scan_ml import scan_ml
from scan_io import scan_archive
from multiplot import multiplot
from multiplot_from_tokens import multiplot_from_tokens
from PIL import Image, ImageFilter,ImageOps
from epitaxy import get_epi
from scan_human_labels import get_scan_human_labels
import codecs
from inp_template import inp_template_numeric_cpy

class api_scan():

	def __init__(self,server):
		self.server=server

	def build(self,scan_dir_path,base_dir,interactive=True):
		scans=scans_io(base_dir)
		config_file=scans.find_path_by_name(os.path.basename(scan_dir_path))
		#print(config_file)
		s=scan_io()
		s.interactive=interactive
		s.load(config_file)
		s.set_path(scan_dir_path)
		s.set_base_dir(base_dir)
		s.build_scan()

	def build_nested(self,built_scan_dir,dir_to_insert):
		scan_dir_path=os.path.abspath(built_scan_dir)	#program file
		sim_to_nest=os.path.abspath(dir_to_insert)	#program file
		scan_build_nested_simulation(scan_dir_path,os.path.join(os.getcwd(),sim_to_nest))

	def add_jobs(self,scan_dir_path):
		exe_command=get_exe_command()
		scans=scans_io(os.getcwd())
		scan_config_file=scans.find_path_by_name(os.path.basename(scan_dir_path))
		s=scan_io()
		s.load(scan_config_file)

		program_list=s.program_list

		watch_dir=os.path.join(os.getcwd(),scan_dir_path)

		commands=[]
		#server_find_simulations_to_run(commands,scan_dir_path)
		commands=tree_load_flat_list(scan_dir_path)
		print(commands)
		
		
		self.server.base_server_init(watch_dir)

		for i in range(0, len(commands)):
			self.server.add_job(commands[i],"")
			print("Adding job"+commands[i])

		#simple_run(exe_command)

	def build_ml_vectors(self,path):
		set_gui(False)
		scan=scan_ml(path)
		scan.build_vector()

	def archive(self,path):
		scan_archive(path)

class gpvdm_api():
	def __init__(self,verbose=True):
		self.save_dir=os.getcwd()
		self.server=base_server()
		if verbose==True:
			self.server.pipe_to_null=False
		else:
			self.server.pipe_to_null=True

		#self.server.base_server_init(get_sim_path())
		if server_get()!=False:
			self.server=server_get()
			self.server.clear_cache()

		self.epi=get_epi()
		if self.epi.loaded==False:
			self.epi.load(os.getcwd())

		self.scan=api_scan(self.server)
		#image ops
		self.Image=Image
		self.ImageFilter=ImageFilter
		self.ImageOps=ImageOps
		self.path=""
		self.callback=None
		self.get_scan_human_labels=get_scan_human_labels()
		self.get_scan_human_labels.clear()
		self.get_scan_human_labels.populate_from_known_tokens()
		self.get_scan_human_labels.populate_from_files()
		#self.get_scan_human_labels.dump()

	def run(self,callback=None):
		if callback!=None:
			self.server.base_server_set_callback(callback)
		self.server.start()

	def add_job(self,path=""):
		if path=="":
			path=get_sim_path()
		else:
			path=os.path.join(get_sim_path(),path)
		print("add path>",path)
		self.server.add_job(path,"")

	def random_file_name(self):
		return codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()

	def set_sim_path(self,path):
		set_sim_path(path)

	def spectral2(self):
		s=spectral2()
		s.calc()

	def set_save_dir(self,path):
		if os.path.isdir(path)==False:
			os.makedirs(path)

		self.save_dir=path

	def save(self,dest,src):
		copyfile(src, os.path.join(self.save_dir,dest))

	def edit(self,file_name,token,value):
		inp_update_token_value(file_name,token,value)

	def get(self,file_name,token):
		return inp_get_token_value(file_name,token)

	def mkdir(self,file_name):
		if os.path.isdir(file_name)==False:
			os.makedirs(file_name)

	def clone(self,file_name):
		output_dir=file_name
		if os.path.isdir(output_dir)==False:
			os.mkdir(output_dir)
		for f in os.listdir(get_sim_path()):
			if f.endswith(".inp") or f.endswith(".gpvdm"):
				copyfile(os.path.join(get_sim_path(),f), os.path.join(output_dir,f))

	def build_multiplot(self,path,gnuplot=False,exp_data=""):
		a=multiplot(gnuplot=gnuplot,exp_data=exp_data)
		a.find_files(os.path.join(get_sim_path(),path))
		a.save()

	def graph_from_tokens(self,output_file,path,file0,token0,file1,token1):
		output_file=os.path.join(get_sim_path(),path,output_file)
		plot=multiplot_from_tokens()
		print("here")

	def find_simulations(self,path):
		ret=[]
		for root, dirs, files in os.walk(path):
			for name in files:
				full_name=os.path.join(root, name)
				if full_name.endswith("sim.gpvdm"):		
					ret.append(os.path.dirname(full_name))
		return ret

	def shape_name_to_file_name(self,path,search_name):
		f=inp()
		f.set_file_name(os.path.join(path,"sim.gpvdm"))
		files=f.lsdir()
		for fname in files:
			if fname.startswith("shape")==True and fname.endswith(".inp")==True:
				data=inp()
				data.load(os.path.join(path,fname))
				name=data.get_token("#shape_name")
				if name==search_name:
					return fname

		return None

	def get_sim_path(self):
		return get_sim_path()

	def template_numeric_cpy(self,dest_path,dest_number,template_name):
		inp_template_numeric_cpy(dest_path,dest_number,template_name)

