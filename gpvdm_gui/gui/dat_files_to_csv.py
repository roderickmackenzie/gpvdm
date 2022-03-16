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

## @package dat_file
#  Load and dump a dat file into a dat class
#

import os
import shutil
import re
import hashlib
import glob
from util_zip import zip_get_data_file
from str2bool import str2bool
from triangle import triangle
from inp import inp_save_lines_to_file

def dat_files_to_csv(file_name,data):
	if file_name.endswith(".csv")==False:
		file_name=file_name+".csv"

	max=data[0].y_len
	for d in data:
		if d.y_len>max:
			max=d.y_len
	out=[]
	line=""
	for i in range(0,len(data)):
		line=line+data[i].key_text+" "+data[i].y_label+","+data[i].key_text+" "+data[i].data_label+","

	line=line[:-1]
	out.append(line)

	for i in range(0,max):
		line=""
		for ii in range(0,len(data)):
			y_text=""
			data_text=""
			if i<data[ii].y_len:
				y_text=str('{:.8e}'.format(float(data[ii].y_scale[i])))
				data_text=str('{:.8e}'.format(float(data[ii].data[0][0][i])))
			line=line+y_text+","+data_text+","
		line=line[:-1]
		out.append(line)

	inp_save_lines_to_file(file_name,out)


