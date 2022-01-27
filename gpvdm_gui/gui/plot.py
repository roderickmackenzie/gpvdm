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

## @package plot
#  These are mainly used by the scan widget.
#  This needs rewriting
#

import os
from token_lib import tokens
from util_zip import zip_get_data_file
from dat_file import dat_file_load_info

def plot_load_info(plot_token,file_name_in):
	if get_plot_file_info(plot_token,file_name_in)==False:
		return False

	return True


def get_plot_file_info(output,file_name):
	found,lines=zip_get_data_file(file_name)
	#print(file_name)
	if found==False:
		print("can't find file",file_name)
		return False

	for i in range(0, len(lines)):
		lines[i]=lines[i].rstrip()

	return dat_file_load_info(output,lines)

def check_info_file(search_string):
	files=["sim_info.dat"]
	if files.count(search_string)> 0:
		return True
	else:
		return False


def plot_populate_plot_token(plot_token,file_name):
	if file_name!=None:
		ret=plot_load_info(plot_token,file_name)
		print("ret====",ret)
		if ret==True:
			return True

	#check to see if I have been provided with a token

	if plot_token!=None:
		my_token_lib=tokens()
		result0=my_token_lib.find(plot_token.tag0)
		result1=my_token_lib.find(plot_token.tag1)
		print("one=",plot_token.tag0,result0)
		print("two=",plot_token.tag1,result1)
		if result0!=False:
			plot_token.x_label=result0.info
			plot_token.x_units=result0.units
			plot_token.x_mul=result0.number_mul

			plot_token.data_label=result1.info
			plot_token.data_units=result1.units
			plot_token.data_mul=result1.number_mul

			plot_token.title=result0.info+" "+result1.info

			print("Found tokens",plot_token.tag0,plot_token.tag1)
			return True
		else:
			print("Tokens not found",plot_token.tag0,plot_token.tag1)

	return False


