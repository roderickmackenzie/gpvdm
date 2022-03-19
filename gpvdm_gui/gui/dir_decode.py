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
from file_store import file_store

import json

def get_dir_type(path):
	ret=file_store()
	ret.file_name=os.path.basename(path)
	ret.display_name=os.path.basename(path)
	if os.path.isfile(path)==True:
		ret.isdir=False
		ret.type="file"
		return ret

	if os.path.isdir(path)==True:
		ret.isdir=True
		ret.icon="folder"
		ret.type="dir"
		file_name=os.path.join(path,"data.json")
		f=inp()

		if f.load(file_name)!=False:
			try:
				json_data="\n".join(f.lines)
				decode=json.loads(json_data)
				ret.type=decode['item_type']
			except:
				pass

			try:
				ret.icon=decode['icon']
			except:
				pass

			try:
				ret.hidden=decode['hidden']
			except:
				pass

			return ret


		if os.path.isfile(os.path.join(path,"wavelengths.dat"))==True:
			ret.type="light"
			return ret

		if os.path.isfile(os.path.join(path,"scan_config.inp"))==True:
			ret.type="scan_dir"
			return ret

		mat_file=os.path.join(path,"mat.inp")
		token=inp_get_token_value(os.path.join(path,"mat.inp"), "#gpvdm_file_type")
		if token=="backup":
			ret.type="backup"
			return ret
		elif token=="multi_plot_dir":
			ret.type="multi_plot_dir"
			return ret

		return ret

	return None
	
