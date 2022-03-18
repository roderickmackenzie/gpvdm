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

## @package multiplot
#  This will generate multiplot files
#


import sys
import os
import shutil

import i18n
_ = i18n.language.gettext


import zipfile
from util_zip import archive_add_dir
from scan_io import scan_list_simulations
from cal_path import subtract_paths
from inp import inp
from gui_enable import gui_get
from util import peek_data
from cal_path import get_sim_path
from dat_file import dat_file
from dat_files_to_gnuplot import dat_file_to_gnuplot_header
from cal_path import subtract_paths
if gui_get()==True:
	from plot_window import plot_window

class sim_dir:
	def __init__(self):
		self.path=""
		self.files=[]

	def dump(self):
		print(self.path)
		for f in self.files:
			print(f)

class multiplot:

	def __init__(self,gnuplot=False,exp_data=""):
		self.sims=[]
		if type(exp_data)==str:
			self.exp_data=[exp_data]
		else:
			self.exp_data=exp_data

		self.gnuplot=gnuplot

	def find_files(self,path):
		self.sims=[]
		self.path=os.path.abspath(path)
		sims=scan_list_simulations(path)
		#print(path,sims)
		for sim_path in sims:
			sim_data=sim_dir()
			sim_data.path=sim_path
			#print(sim_data.path)
			for root, dirs, files in os.walk(sim_data.path):
				for name in files:
					full_name=os.path.join(root, name)
					rel_path=subtract_paths(sim_data.path,full_name)
					if full_name.endswith(".dat") or full_name.endswith(".csv"):

						add=False
						if name==rel_path:		#find files which are only on the first level directory
							add=True
						if rel_path.startswith("dynamic"):		#Is the dynamic directory
							add=True

						if add==True:
							text=peek_data(full_name)
							if text.startswith(b"#gpvdm"):
								sim_data.files.append(rel_path)

			self.sims.append(sim_data)

	def make_dirs(self,file_name):
		path=os.path.dirname(file_name)
		if os.path.isdir(path)==False:
			os.makedirs(path)
			f=inp()
			f.lines=[]
			f.lines.append("#gpvdm_file_type")
			f.lines.append("multi_plot_dir")
			f.lines.append("#end")
			f.save_as(os.path.join(path,"mat.inp"),mode="l",dest="file")
	
	def gen_gnu_plot_files(self):
		path=self.path
		if len(self.sims)>0:
			for cur_file in self.sims[0].files:
				dat=dat_file()
				dat.load(os.path.join(self.sims[0].path,cur_file))
				found_files=[]
				found_files.extend(dat_file_to_gnuplot_header(dat))
				found_files.append("plot \\")
				for s in self.sims:
					if cur_file in s.files:
						full_path_name=os.path.join(s.path,cur_file)
						title=os.path.dirname(subtract_paths(self.path,full_path_name))
						found_files.append("\'"+full_path_name+"' using ($1):($2) with l lw 3 title '"+title+"',\\")

				for exp_file in self.exp_data:
					found_files.append("\'"+exp_file+"' using ($1):($2) with p title '"+os.path.basename(exp_file)+"',\\")

				found_files[-1]=found_files[-1][:-1]

				out_file=os.path.join(path,cur_file)
				out_file=os.path.splitext(out_file)[0]+".plot"

				self.make_dirs(out_file)
				f=inp()
				f.lines=found_files
				#print("saving as:",out_file)
				f.save_as(out_file)

	def save(self,gnuplot=True,multi_plot=True):
		if gnuplot==True:
			self.gen_gnu_plot_files()

		if 	multi_plot==True:
			path=self.path
			if len(self.sims)>0:
				for cur_file in self.sims[0].files:
					found_files=["#multiplot"]
					for s in self.sims:
						#print("compare>>",cur_file)
						#s.dump()
						if cur_file in s.files:
							found_files.append(os.path.join(s.path,cur_file))

					#print("save to>",os.path.join(path,cur_file))
					out_file=os.path.join(path,cur_file)
					self.make_dirs(os.path.splitext(out_file)[0]+".plot")
					f=inp()
					f.lines=found_files
					f.save_as(out_file)

	def plot(self,file_name):
		f=inp()
		f.load(file_name)
		files=f.lines[1:]

		window=plot_window()
		labels=[]
		for f in files:
			rel_path=subtract_paths(get_sim_path(),f)
			rel_path=rel_path.replace("\\", "/")
			labels.append(rel_path)
		window.init(files,labels)

