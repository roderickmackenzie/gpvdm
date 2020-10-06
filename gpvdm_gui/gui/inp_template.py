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

## @package inp_template
#  Manipulate the inp template file
#


import os
import re
from cal_path import gpvdm_paths
from inp import inp

def inp_template_numeric_cpy(dest_path,dest_number,template_name):
	src=os.path.join(gpvdm_paths.get_inp_template_path(),template_name)
	for file_name in os.listdir(src):
		new_file_name=re.sub('\d', str(dest_number), file_name)
		full_in_file=os.path.join(src,file_name)
		full_out_file=os.path.join(dest_path,new_file_name)
		f=inp()
		f.load(full_in_file)
		f.save(full_out_file)
		print("copy:",full_out_file,full_in_file)




