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

def dat_file_to_gnuplot_header(dat_file):
	ret=[]
	ret.append("set title '"+dat_file.title+"'")
	ret.append("set ylabel '"+dat_file.data_label+" ("+dat_file.data_units+")'")
	ret.append("set xlabel '"+dat_file.y_label+" ("+dat_file.y_units+")'")
	ret.append("set key top left")
	ret.append("set colors classic")
	if dat_file.logdata==True:
		ret.append("set logscale y")
		ret.append("set format y \"%2.0t{/Symbol \\264}10^{%L}\"")
	else:
		ret.append("#set logscale y")
		ret.append("#set format y \"%2.0t{/Symbol \\264}10^{%L}\"")

	if dat_file.logy==True:
		ret.append("set logscale x")
	else:
		ret.append("#set logscale x")

	return ret

def dat_files_to_gnuplot(out_dir,data):
	os.mkdir(out_dir)
	data_dir=os.path.join(out_dir,"data")
	os.mkdir(data_dir)

	makefile=[]
	makefile.append("main:")
	makefile.append("	gnuplot plot.plot >plot.eps")
	makefile.append("	gs -dNOPAUSE -r600 -dEPSCrop -sDEVICE=jpeg -sOutputFile=plot.jpg plot.eps -c quit")
	makefile.append("	xdg-open plot.jpg")
	inp_save_lines_to_file(os.path.join(out_dir,"makefile"),makefile)

	plotfile=[]
	plotfile.append("set term postscript eps enhanced color solid \"Helvetica\" 25")
	plotfile.extend(dat_file_to_gnuplot_header(data[0]))

	plotfile.append("plot \\")

	for i in range(0,len(data)):
		d=data[i]
		d.save_as_txt(os.path.join(data_dir,str(i)+".txt"))
		file_path=os.path.join("data",str(i)+".txt")
		file_path=d.file_name
		line="'"+file_path+"' using ($1):($2) with lp title '"+d.key_text+"'"
		#print(i,len(data))
		if i<len(data)-1:
			line=line+",\\"

		plotfile.append(line)

	inp_save_lines_to_file(os.path.join(out_dir,"plot.plot"),plotfile)

def dat_files_to_gnuplot_files(out_dir,data):
	if os.path.isdir(out_dir)==False:
		os.mkdir(out_dir)
	data_dir=os.path.join(out_dir,"data")

	if os.path.isdir(data_dir)==False:
		os.mkdir(data_dir)

	makefile=[]
	makefile.append("main:")
	for i in range(0,len(data)):
		makefile.append("	gnuplot "+str(i)+".plot >"+str(i)+".eps")
		makefile.append("	gs -dNOPAUSE -r600 -dEPSCrop -sDEVICE=jpeg -sOutputFile="+str(i)+".jpg "+str(i)+".eps -c quit")
		makefile.append("")

	inp_save_lines_to_file(os.path.join(out_dir,"makefile"),makefile)

	for i in range(0,len(data)):
		plotfile=[]
		plotfile.append("set term postscript eps enhanced color solid \"Helvetica\" 25")
		plotfile.extend(dat_file_to_gnuplot_header(data[i]))

		d=data[i]
		d.save_as_txt(os.path.join(data_dir,str(i)+".txt"))
		file_path=os.path.join("data",str(i)+".txt")
		#file_path=d.file_name
		line="plot '"+file_path+"' using ($1):($2) with lp title '"+d.key_text+"'"

		plotfile.append(line)

		inp_save_lines_to_file(os.path.join(out_dir,str(i)+".plot"),plotfile)
