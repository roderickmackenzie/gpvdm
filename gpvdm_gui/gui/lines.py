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

## @package lines
#  Read lines from a file into a x,y array.
#

import os
import shutil
import re
import hashlib
import glob
from util_zip import zip_get_data_file

class vec():
	x=0.0
	y=0.0
	z=0.0


def lines_read(out,file_name):
	
	found,lines=zip_get_data_file(file_name)

	if found==False:
		return False

	for i in range(0,len(lines)):
		if len(lines[i])>2:
			text=lines[i].split(" ")
			out.append(vec())
			out[-1].x=float(text[0])
			out[-1].y=float(text[1])
	
	return True
