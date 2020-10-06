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

## @package plot_io
#  Back end for the plot files
#


import os
from inp import inp_load_file
from inp import inp_search_token_value
from util_zip import zip_get_data_file
from str2bool import str2bool
from inp import inp_save_lines_to_file

def plot_load_info(plot_token,file_name_in):
	#print "whoo",file_name_in
	#file_name=os.path.splitext(file_name_in)[0]+".dat"
	if get_plot_file_info(plot_token,file_name_in)==False:
		return False

	config_file=os.path.splitext(file_name_in)[0]+".oplot"
	ret=plot_load_oplot_file(plot_token,config_file)

	return True

def plot_load_oplot_file(plot_token,file_name):
	lines=inp_load_file(file_name)
	if lines!=False:
		plot_token.logy=str2bool(inp_search_token_value(lines, "#logy"))
		plot_token.logx=str2bool(inp_search_token_value(lines, "#logx"))
		plot_token.logz=str2bool(inp_search_token_value(lines, "#logz"))
		plot_token.grid=str2bool(inp_search_token_value(lines, "#grid"))
		plot_token.invert_y=str2bool(inp_search_token_value(lines, "#invert_y"))
		plot_token.normalize=str2bool(inp_search_token_value(lines, "#normalize"))
		plot_token.norm_to_peak_of_all_data=str2bool(inp_search_token_value(lines, "#norm_to_peak_of_all_data"))
		plot_token.subtract_first_point=str2bool(inp_search_token_value(lines, "#subtract_first_point"))
		plot_token.add_min=str2bool(inp_search_token_value(lines, "#add_min"))
		plot_token.file0=inp_search_token_value(lines, "#file0")
		plot_token.file1=inp_search_token_value(lines, "#file1")
		plot_token.file2=inp_search_token_value(lines, "#file2")
		plot_token.tag0=inp_search_token_value(lines, "#tag0")
		plot_token.tag1=inp_search_token_value(lines, "#tag1")
		plot_token.tag2=inp_search_token_value(lines, "#tag2")
		plot_token.legend_pos=inp_search_token_value(lines, "#legend_pos")
		plot_token.key_units=inp_search_token_value(lines, "#key_units")
		plot_token.label_data=str2bool(inp_search_token_value(lines, "#label_data"))
		plot_token.type=inp_search_token_value(lines, "#type")
		plot_token.x_label=inp_search_token_value(lines, "#x_label")
		plot_token.y_label=inp_search_token_value(lines, "#y_label")
		plot_token.z_label=inp_search_token_value(lines, "#z_label")
		plot_token.data_label=inp_search_token_value(lines, "#data_label")
		plot_token.x_units=inp_search_token_value(lines, "#x_units")
		plot_token.y_units=inp_search_token_value(lines, "#y_units")
		plot_token.y_units=inp_search_token_value(lines, "#z_units")
		plot_token.data_units=inp_search_token_value(lines, "#data_units")
		plot_token.x_mul=float(inp_search_token_value(lines, "#x_mul"))
		plot_token.y_mul=float(inp_search_token_value(lines, "#y_mul"))
		plot_token.z_mul=float(inp_search_token_value(lines, "#z_mul"))
		plot_token.data_mul=float(inp_search_token_value(lines, "#data_mul"))
		plot_token.key_units=inp_search_token_value(lines, "#key_units")
		plot_token.x_start=float(inp_search_token_value(lines, "#x_start"))
		plot_token.x_stop=float(inp_search_token_value(lines, "#x_stop"))
		plot_token.x_points=float(inp_search_token_value(lines, "#x_points"))
		plot_token.y_start=float(inp_search_token_value(lines, "#y_start"))
		plot_token.y_stop=float(inp_search_token_value(lines, "#y_stop"))
		plot_token.y_points=float(inp_search_token_value(lines, "#y_points"))
		plot_token.time=float(inp_search_token_value(lines, "#time"))
		plot_token.Vexternal=float(inp_search_token_value(lines, "#Vexternal"))

		return True
	return False


def get_plot_file_info(output,file_name):
	found,lines=zip_get_data_file(file_name)
	#print(file_name)
	if found==False:
		print("can't find file",file_name)
		return False

	for i in range(0, len(lines)):
		lines[i]=lines[i].rstrip()


	if len(lines)>1:
		if lines[0]=="#gpvdm":
			for i in range(0, len(lines)):
				if (len(lines[i])>0):
					if (lines[i][0]!="#"):
						break
					else:
						command=lines[i].split(" ",1)
						if len(command)<2:
							command.append("")
						if (command[0]=="#x_mul"):
							output.x_mul=float(command[1])
						if (command[0]=="#y_mul"):
							output.y_mul=float(command[1])
						if (command[0]=="#z_mul"):
							output.z_mul=float(command[1])
						if (command[0]=="#x_label"):
							output.x_label=command[1]
						if (command[0]=="#y_label"):
							output.y_label=command[1]
						if (command[0]=="#z_label"):
							output.z_label=command[1]
						if (command[0]=="#data_label"):
							output.data_label=command[1]
						if (command[0]=="#x_units"):
							output.x_units=command[1]
						if (command[0]=="#y_units"):
							output.y_units=command[1]
						if (command[0]=="#z_units"):
							output.z_units=command[1]
						if (command[0]=="#data_units"):
							output.data_units=command[1]
						if (command[0]=="#logscale_x"):
							output.logx=bool(int(command[1]))
						if (command[0]=="#logscale_y"):
							output.logy=bool(int(command[1]))
						if (command[0]=="#logscale_z"):
							output.logz=bool(int(command[1]))
						if (command[0]=="#type"):
							output.type=command[1]
						if (command[0]=="#title"):
							output.title=command[1]
						if (command[0]=="#section_one"):
							output.section_one=command[1]
						if (command[0]=="#section_two"):
							output.section_two=command[1]
						if (command[0]=="#time"):
							output.time=float(command[1])
						if (command[0]=="#Vexternal"):
							output.Vexternal=float(command[1])
						if (command[0]=="#x"):
							output.x_len=int(command[1])
						if (command[0]=="#y"):
							output.y_len=int(command[1])
						if (command[0]=="#z"):
							output.z_len=int(command[1])

			return True

	return False
