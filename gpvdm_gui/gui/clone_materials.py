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

## @package clone_materials
#  Clone a materials
#
#import sys
import os
import shutil
#import glob
from shutil import copyfile
from cal_path import get_base_material_path
from cal_path import subtract_paths
from materials_io import find_db_items
from progress_class import progress_class
from process_events import process_events
from win_lin import running_on_linux

def clone_material(dest_material_dir,src_material_dir):
	if os.path.isdir(dest_material_dir)==False:
		try:
			os.makedirs(dest_material_dir)
		except:
			return False

	files=os.listdir(src_material_dir)
	all_files=["alpha.gmat","n.gmat"]
	all_files.append("spectra.inp")
	all_files.append("shape.inp")
	all_files.append("filter.inp")
	all_files.append("shape_import.inp")
	all_files.append("data.json")
	all_files.append("image.png")
	all_files.append("image_original.png")


	for i in range(0,len(files)):
		if files[i] in all_files:
			src_mat_file=os.path.join(src_material_dir,files[i])
			if os.path.isfile(src_mat_file)==True:
				copyfile(src_mat_file,os.path.join(dest_material_dir,files[i]))
	return True

def clone_materials(dest,src_dir,file_type):
	if running_on_linux()==False:
		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		process_events()

		if os.path.isdir(dest)==False:
			os.makedirs(dest)

		files=find_db_items(mat_path=src_dir,file_type=file_type)
		for i in range(0,len(files)):

			src_file=os.path.join(src_dir,files[i])
			dest_file=os.path.join(dest,files[i])
			clone_material(dest_file,src_file)

			progress_window.set_fraction(float(i)/float(len(files)))
			progress_window.set_text("Configuring "+files[i])
			process_events()

		progress_window.stop()
	else:
		os.symlink(src_dir, dest)

