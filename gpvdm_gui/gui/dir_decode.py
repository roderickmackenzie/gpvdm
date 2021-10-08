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


## @package dir_decode
#  Functions to find out what type of directory we have
#



import os
import shutil
import re
import hashlib
import glob
from inp import inp
from inp import inp_get_token_value
import json

def get_dir_type(path):

	if os.path.isfile(path)==True:
		return "file"

	if os.path.isdir(path)==True:
		file_name=os.path.join(path,"data.json")
		f=inp()
		try:
			if f.load(file_name)!=False:
				json_data="\n".join(f.lines)
				decode=json.loads(json_data)
				return decode['item_type']
		except:
			pass

		if os.path.isfile(os.path.join(path,"wavelengths.dat"))==True:
			return "light"

		if os.path.isfile(os.path.join(path,"scan_config.inp"))==True:
			return "scan_dir"

		mat_file=os.path.join(path,"mat.inp")
		token=inp_get_token_value(os.path.join(path,"mat.inp"), "#gpvdm_file_type")
		if token=="backup_main":
			return "backup_main"
		elif token=="backup":
			return "backup"
		elif token=="cache":
			return "cache"
		elif token=="multi_plot_dir":
			return "multi_plot_dir"


		return "dir"
