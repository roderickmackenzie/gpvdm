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

## @package dat_file_math
#  Do math on the dat file class.
#

import os
import shutil
import re
from dat_file import dat_file

def dat_file_max_min(my_data,cur_min=None,cur_max=None):
	my_max=False
	my_min=False
	
	if my_data.data_max!=None and my_data.data_min!=None:
		return [my_data.data_max,my_data.data_min]

	if my_data.x_len>0 and my_data.y_len>0 and my_data.z_len>0:
		my_max=my_data.data[0][0][0]
		my_min=my_data.data[0][0][0]

		for z in range(0,my_data.z_len):
			for x in range(0,my_data.x_len):
				for y in range(0,my_data.y_len):
					
					if my_data.data[z][x][y]>my_max:
						my_max=my_data.data[z][x][y]

					if my_data.data[z][x][y]<my_min:
						my_min=my_data.data[z][x][y]
	if cur_min!=None:
		if cur_min<my_min:
			my_min=cur_min

	if cur_max!=None:
		if cur_max>my_max:
			my_max=cur_max

	return [my_max,my_min]


def dat_file_sub(my_data,one):
	if (my_data.x_len==one.x_len) and (my_data.y_len==one.y_len) and (my_data.z_len==one.z_len):
		for z in range(0,my_data.z_len):
			for x in range(0,my_data.x_len):
				for y in range(0,my_data.y_len):
					my_data.data[z][x][y]=my_data.data[z][x][y]-one.data[z][x][y]

def dat_file_sub_float(my_data,val):
		for z in range(0,my_data.z_len):
			for x in range(0,my_data.x_len):
				for y in range(0,my_data.y_len):
					my_data.data[z][x][y]=my_data.data[z][x][y]-val
					
def dat_file_mul(my_data,val):
		for z in range(0,my_data.z_len):
			for x in range(0,my_data.x_len):
				for y in range(0,my_data.y_len):
					my_data.data[z][x][y]*=val

def dat_file_abs(my_data):
		for z in range(0,my_data.z_len):
			for x in range(0,my_data.x_len):
				for y in range(0,my_data.y_len):
					my_data.data[z][x][y]=abs(my_data.data[z][x][y])
