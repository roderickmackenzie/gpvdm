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

## @package materials_io
#  io functions related to the materials data base.
#

import os
import zipfile
import shutil

from cal_path import get_materials_path
from cal_path import get_base_material_path
from dir_decode import get_dir_type
from inp import inp
from json_material_db_item import json_material_db_item
 
def find_db_items(mat_path=get_materials_path(),file_type="material",from_src=None):
	ret=[]

	for root, dirs, files in os.walk(mat_path):
		dir_info=get_dir_type(root)
		if dir_info!=None:
			if dir_info.type==file_type:
				
				add=True
				if from_src!=None:
					a=json_material_db_item()
					f.load(os.path.join(root,"data.json"))
					if f.mat_src!=from_src:
						add=False

				if add==True:
					s=os.path.relpath(root, mat_path)
					s=s.replace("\\","/")
					ret.append(s)

	return ret

