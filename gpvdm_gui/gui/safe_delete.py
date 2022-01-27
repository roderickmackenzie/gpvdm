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


## @package util
#  General helper functions.
#



import os
import shutil
import sys
import hashlib
import glob
from cal_path import get_home_path
from win_lin import running_on_linux

def gpvdm_delete_file(path,allow_dir_removal=False):
	#Paranoia
	if os.path.samefile(str(get_home_path()),str(path))==True:
		sys.exit('I can not delete this dir')
		return

	#Paranoia
	if os.path.samefile(str(os.getcwd()),str(path))==True:
		sys.exit('I can not delete this dir2')
		return

	if os.path.isdir(path)==True:
		if allow_dir_removal==False:
			print("I am not allwed to delete:", path)
			return
		try:
			shutil.rmtree(path)
		except IOError:
			print("Could not delete the dir:", path)

	elif os.path.isfile(path)==True:
		print("Delete",path)
		try:
			os.remove(path)
		except IOError:
			print("Could not delete the file:", path)
	else:
		print("This should not be run")


