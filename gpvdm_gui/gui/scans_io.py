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

## @package scans_io
#  IO functions for the scanning simulation parameters.
#


import sys
import os


from progress_class import progress_class
from process_events import process_events
from server import server_break
from numpy import std

from error_dlg import error_dlg

import i18n
_ = i18n.language.gettext

from yes_no_cancel_dlg import yes_no_cancel_dlg

import zipfile
from util_zip import archive_add_dir
from inp import inp
from inp import inp_lsdir
from inp import inp_issequential_file
from scan_io import scan_io
from safe_delete import gpvdm_delete_file

class scans_io:

	def __init__(self,path):
		self.path=path

	def get_scans(self):
		scan_dirs=[]
		f=inp(file_path=os.path.join(self.path,"sim.gpvdm"))
		ls=f.lsdir()
		#print(ls)
		for scan_file in ls:
			
			if inp_issequential_file(scan_file,"scan")==True:
				s=scan_io()
				s.load(os.path.join(self.path,scan_file))
				scan_dirs.append(s)


		return scan_dirs

	def find_path_by_name(self,scan_name):
		ret=[]
		scans=self.get_scans()
		for s in scans:
			if s.human_name==scan_name:
				return s.config_file
		return False

	def get_scan_dirs(self):
		ret=[]
		scans=self.get_scans()
		for s in scans:
			ret.append(s.scan_dir)
		return ret

	def get_new_scan_file(self):
		f=inp()
		for i in range(0,100):
			file_name="scan"+str(i)+".inp"
			if f.isfile(os.path.join(self.path,file_name))==False:
				return file_name

		return False

	def delete(self,scan_name):
		config_file=self.find_path_by_name(scan_name)
		if config_file!=False:
			config_path=os.path.join(self.path,config_file)
			f=inp()
			f.set_file_name(config_path)
			f.delete()

		scan_dir=os.path.join(self.path,scan_name)
		if os.path.isdir(scan_dir)==True:
			gpvdm_delete_file(scan_dir,allow_dir_removal=True)

	def new(self,scan_name):
		config_file_new=self.get_new_scan_file()
		scan=scan_io()
		scan.set_path(os.path.join(self.path,scan_name))
		scan.human_name=scan_name
		scan.config_file=config_file_new
		scan.make_dir()
		scan.save()

	def delete_scan_dirs(self):
		scans=self.get_scan_dirs()

		for s in scans:
			#print("Deleteing ",my_file)
			gpvdm_delete_file(s,allow_dir_removal=True)

	def clean_all(self):
		print("stub")

	def clone(self,new_name,old_name):
		if self.find_path_by_name(new_name)!=False:
			return False
		config_file_old=self.find_path_by_name(old_name)
		config_file_new=self.get_new_scan_file()
		f=inp()
		f.load(os.path.join(self.path,config_file_old))
		f.set_token("#scan_name",new_name)
		f.save_as(os.path.join(self.path,config_file_new))
		return True

