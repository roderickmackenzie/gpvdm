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

## @package clone
#  Clone a simulation
#
#import sys
import os
import shutil
#import glob
from util_zip import zip_lsdir
from util_zip import read_lines_from_archive
from util_zip import write_lines_to_archive
from util_zip import archive_make_empty
from shutil import copyfile
from cal_path import get_base_material_path
from cp_gasses import copy_gasses
from cal_path import find_light_source
from cal_path import get_spectra_path
from cal_path import get_base_spectra_path
from win_lin import running_on_linux
from cal_path import gpvdm_paths

def clone_sim_dir(output_dir,input_dir):
	if os.path.isdir(output_dir)==False:
		os.makedirs(output_dir)
	for f in os.listdir(input_dir):
		if f.endswith(".inp") or f.endswith("sim.json") or f.endswith(".gpvdm"):
			copyfile(os.path.join(input_dir,f), os.path.join(output_dir,f))

#remove this if you can but you will need to remove inp_template first
def gpvdm_clone(path,copy_dirs=False,dest="archive"):


	src_dir=gpvdm_paths.get_inp_template_path()
	src_archive=os.path.join(src_dir,"sim.gpvdm")
		
	dest_archive=os.path.join(path,"sim.gpvdm")
	files=zip_lsdir(src_archive)
	lines=[]

	archive_make_empty(dest_archive)

	for i in range(0,len(files)):
		if files[i].endswith(".inp"):
			lines=read_lines_from_archive(src_archive,files[i],mode="b")
			write_lines_to_archive(dest_archive,files[i],lines,dest=dest,mode="b")


	if copy_dirs==True:
		if os.path.isdir(os.path.join(src_dir,"plot")):
			shutil.copytree(os.path.join(src_dir,"plot"), os.path.join(path,"plot"))

		if os.path.isdir(os.path.join(src_dir,"exp")):
			shutil.copytree(os.path.join(src_dir,"exp"), os.path.join(path,"exp"))



def clone_spectra(dest_spectra_dir,src_spectra_dir):
	#print(dest_spectra_dir,src_spectra_dir)
	if os.path.isdir(dest_spectra_dir)==False:
		os.makedirs(dest_spectra_dir)

	for copy_file in ["spectra.inp","data.json"]:
		src_spectra_file=os.path.join(src_spectra_dir,copy_file)
		if os.path.isfile(src_spectra_file)==True:
			copyfile(src_spectra_file,os.path.join(dest_spectra_dir,copy_file))
			

def clone_spectras(dest):
	if running_on_linux()==False:
		src_dir=os.path.join(get_base_spectra_path())
		dest_dir=dest

		if os.path.isdir(dest_dir)==False:
			os.mkdir(dest_dir)

		files=find_light_source(get_base_spectra_path())

		for i in range(0,len(files)):
			src_file=os.path.join(src_dir,files[i])
			dest_file=os.path.join(dest_dir,files[i])
			clone_spectra(dest_file,src_file)
	else:
		os.symlink(get_base_spectra_path(), dest)

