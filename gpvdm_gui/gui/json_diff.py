# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2012-2017 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#
#   https://www.gpvdm.com
#   Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
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
# 


## @package json_diff
# Diff two json objects
#


import sys
import os
import shutil
import json
import inspect
from str2bool import str2bool
from util_text import is_number
from inp import inp

def json_diff(obj0,obj1,path=""):
	ret=[]
	for obj in obj0:
		if type(obj0[obj])==dict:
			if path!="":
				new_path=path+"."+obj
			else:
				new_path=obj

			my_ret=json_diff(obj0[obj],obj1,path=new_path)
			ret.extend(my_ret)
		else:
			cur_path=path+"."+obj
			pointer=obj1
			#print(cur_path)
			for seg in cur_path.split("."):
				#print(pointer,seg)
				pointer=pointer[seg]
			if pointer!=obj0[obj]:
				ret.append(cur_path)

	return ret


