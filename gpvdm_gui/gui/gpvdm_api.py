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

from server import server_base
from server import server_get
from cal_path import get_sim_path
from progress_class import progress_class
from spctral2 import spctral2
from inp import inp

#scan
from scan_io import scan_build_nested_simulation
from scan_io import scan_io
from scans_io import scans_io

from cal_path import get_exe_command
from scan_tree import tree_load_flat_list
from gui_enable import set_gui
from ml_vectors import ml_vectors
from scan_io import scan_archive
from multiplot import multiplot
from multiplot_from_tokens import multiplot_from_tokens
from PIL import Image, ImageFilter,ImageOps
from epitaxy import get_epi
import codecs
from gpvdm_json import gpvdm_data
from inp import inp_get_token_value
from math import log10
from scan_tree import random_log
import random
from clone import clone_sim_dir
from clean_sim import clean_sim_dir

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
		
		
		self.server.server_base_init(watch_dir)

		for i in range(0, len(commands)):
			self.server.add_job(commands[i],"")
			print("Adding job"+commands[i])

		#simple_run(exe_command)

	def build_ml_vectors(self,path):
		set_gui(False)
		scan=ml_vectors()
		scan.build_vector(path)

	def archive(self,path):
		scan_archive(path)

class gpvdm_api():
	def __init__(self,verbose=True):
		self.save_dir=os.getcwd()
		self.server=server_base()
		if verbose==True:
			self.server.pipe_to_null=False
		else:
			self.server.pipe_to_null=True

		#self.server.server_base_init(get_sim_path())
		if server_get()!=False:
			self.server=server_get()
			self.server.clear_cache()

		self.scan=api_scan(self.server)
		#image ops
		self.Image=Image
		self.ImageFilter=ImageFilter
		self.ImageOps=ImageOps
		self.path=""
		self.callback=None

	def run(self,callback=None):
		if callback!=None:
			self.server.server_base_set_callback(callback)
		self.server.start()

	def add_job(self,path="",args=""):
		if path=="":
			path=get_sim_path()
		else:
			path=os.path.join(get_sim_path(),path)
		print("add path>",path)
		self.server.add_job(path,args)

	def random_file_name(self):
		return codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()

	def random_log(self,min,max):
		return float(random_log(min,max))

	def random(self,min,max):
		return random.uniform(min, max)

	def set_sim_path(self,path):
		set_sim_path(path)


	def set_save_dir(self,path):
		if os.path.isdir(path)==False:
			os.makedirs(path)

		self.save_dir=path

	def get(self,file_name,token):
		return inp_get_token_value(file_name,token)

	def save(self,dest,src):
		copyfile(src, os.path.join(self.save_dir,dest))

	def json_load(self,file_name):
		a=gpvdm_data()
		a.load(file_name)
		return a

	def mkdir(self,file_name):
		if os.path.isdir(file_name)==False:
			os.makedirs(file_name)

	def clone(self,output_dir,input_dir):
		clone_sim_dir(output_dir,input_dir)

	def build_multiplot(self,path,gnuplot=False,exp_data=""):
		a=multiplot(gnuplot=gnuplot,exp_data=exp_data)
		a.find_files(os.path.join(get_sim_path(),path))
		a.save()

	def clean_dir(self,path):
		clean_sim_dir(path)

	def graph_from_tokens(self,output_file,path,file0,token0,file1,token1):
		output_file=os.path.join(get_sim_path(),path,output_file)
		plot=multiplot_from_tokens()

	def log_range(self,start,stop,steps):
		lout=[]
		l_start=log10(start)
		l_stop=log10(stop)
		dl=(l_stop-l_start)/steps
		pos=l_start
		for i in range(0,steps+1):
			lout.append(pow(10,pos))
			pos=pos+dl

		return lout

	def load_snapshots(self,path):
		dirs=[]
		for root, dirs, files in os.walk(path):
			for name in files:
				if name.endswith("json.dat")==True:
					dirs.append(os.path.dirname(os.path.join(root, name)))
					
		print(dirs)

	def range(self,start,stop,steps):
		lout=[]
		dl=(stop-start)/steps
		pos=start
		for i in range(0,steps+1):
			lout.append(pos)
			pos=pos+dl

		return lout

	def find_simulations(self,path):
		ret=[]
		for root, dirs, files in os.walk(path):
			for name in files:
				full_name=os.path.join(root, name)
				if full_name.endswith("sim.gpvdm"):		
					ret.append(os.path.dirname(full_name))
		return ret


	def get_sim_path(self):
		return get_sim_path()

