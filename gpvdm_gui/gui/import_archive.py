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

## @package import_archive
#  Logic to import .gpvdm files, even if they are in an older fromat.
#


import sys
import os
import shutil
from scans_io import scans_io

from util import copy_scan_dir


from util_zip import zip_lsdir
from util_zip import zip_remove_file
from util_zip import archive_copy_file
from util_zip import archive_isfile
from util_zip import read_lines_from_file

from cal_path import get_materials_path
from util_zip import extract_file_from_archive

from progress_class import progress_class
from process_events import process_events
import re
from cal_path import gpvdm_paths
from inp import inp

class file_type():
	JUST_COPY=0
	IGNORE=3

	
	def __init__(self,name="fit_data",dest="archive",copy_opp=JUST_COPY,base_file="",needed=True):
		self.name=name
		self.dest=dest
		self.copy_opp=copy_opp
		self.base_file=base_file
		self.needed=needed		#needed in every simulation
		if base_file=="":
			self.base_file=name
		if name.endswith(".inp")==True:
			self.index_file=False
		else:
			self.index_file=True

file_list=[]

file_list.append(file_type(name="genrate",dest="file",copy_opp=file_type().JUST_COPY))
file_list.append(file_type(name="sim.json",dest="archive",copy_opp=file_type().JUST_COPY))
file_list.append(file_type(name="fit_error_delta",dest="file",copy_opp=file_type().JUST_COPY))
file_list.append(file_type(name="fit_error_exp",dest="file",copy_opp=file_type().JUST_COPY))
file_list.append(file_type(name="fit_error_sim",dest="file",copy_opp=file_type().JUST_COPY))
file_list.append(file_type(name="fit_data",dest="file",copy_opp=file_type().JUST_COPY))
file_list.append(file_type(name="json.bib",copy_opp=file_type().JUST_COPY))
file_list.append(file_type(name="server.inp",copy_opp=file_type().JUST_COPY))

def get_file_info(file_name):
	match = re.match(r"([a-z_]+)([0-9]+)(.inp)", file_name, re.I)
	if match==None:
		match = re.match(r"([a-z_]+)([0-9]+)(.dat)", file_name, re.I)

	if match!=None:
		for i in range(0,len(file_list)):
			if file_list[i].name==match.groups()[0]:
				return file_list[i]
	
	for i in range(0,len(file_list)):
		if file_list[i].name==file_name:
			return file_list[i]
		
	return False


def merge_archives(src_archive,dest_archive,only_over_write):
	debug=False

	progress_window=progress_class()
	progress_window.show()
	progress_window.start()

	process_events()

	dest_path=os.path.dirname(dest_archive)
	template_archive=gpvdm_paths.get_inp_template_path()

	ls=zip_lsdir(src_archive)

	#copy files without checking ver

	for i in range(0,len(ls)):
		info=get_file_info(ls[i])
		if info!=False:
			if info.copy_opp==file_type().JUST_COPY:
				#print(ls[i])
				archive_copy_file(dest_archive,ls[i],src_archive,ls[i],dest=info.dest)
		elif ls[i].endswith(".m"):
			archive_copy_file(dest_archive,ls[i],src_archive,ls[i],dest="file")

		progress_window.set_fraction(float(i)/float(len(ls)))
		progress_window.set_text("Importing "+ls[i])
		process_events()

	#if you find a materials directory in the archive try to merge it
	for i in range(0,len(ls)):
		zip_dir_name=ls[i].split("/")
		if zip_dir_name[0]=="materials":
			dest=os.path.join(os.path.dirname(get_materials_path()))
			#print("Try to read",src_archive,ls[i],dest)
			extract_file_from_archive(dest,src_archive,ls[i])

		if zip_dir_name[0]=="sim":
			extract_file_from_archive(dest_path,src_archive,ls[i])

		if zip_dir_name[0]=="calibrate":
			extract_file_from_archive(dest_path,src_archive,ls[i])

	#search for scan directories
	scan_dirs=[]
	for i in range(0,len(ls)):
		if ls[i].endswith("gpvdm_gui_config.inp"):
			scan_dirs.append(os.path.dirname(ls[i]))

	#extract scan directories
	for i in range(0,len(ls)):
		for ii in range(0,len(scan_dirs)):
			if ls[i].startswith(scan_dirs[ii])==True:
				#print("Try to read",src_archive,ls[i])
				extract_file_from_archive(dest_path,src_archive,ls[i])

	progress_window.stop()

def import_archive(src_archive,dest_archive,only_over_write):
	src_dir=os.path.dirname(src_archive)
	dest_dir=os.path.dirname(dest_archive)

	if src_archive.endswith('.gpvdm')==False:
		print("I can only import from .gpvdm files")
		return

	if dest_archive.endswith('.gpvdm')==False:
		print("I can only import to .gpvdm files")
		return

	merge_archives(src_archive,dest_archive,only_over_write)

	import_scan_dirs(dest_dir,src_dir)

def import_scan_dirs(dest_dir,src_dir):
	scans=scans_io(src_dir)
	sim_dirs=scans.get_scan_dirs()
	for my_file in sim_dirs:
		dest=os.path.join(dest_dir,os.path.basename(my_file))

		copy_scan_dir(dest,my_file)

