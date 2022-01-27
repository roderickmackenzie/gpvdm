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

## @package used_files_menu
#  A used files menu
#


import os
from dat_file import dat_file
import glob
from cal_path import get_user_settings_dir
from util_zip import read_lines_from_file
from inp import inp_save_lines_to_file

def used_files_load():
	path=os.path.join(get_user_settings_dir(),"used_files.inp")
	if os.path.isfile(path)==True:
		lines=read_lines_from_file(path)
		return lines
	return []

def used_files_add(file_name):
	files=used_files_load()
	path=os.path.join(get_user_settings_dir(),"used_files.inp")
	if file_name in files:
		return
	files.insert(0,file_name)
	if len(files)>10:
		files=files[:10]

	inp_save_lines_to_file(path,files)

