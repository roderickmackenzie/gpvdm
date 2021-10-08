#!/usr/bin/env python
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

## @package make_man
#  Build the man pages related to the output files.
#


import glob
import operator
from plot_io import plot_load_info
from dat_file import dat_file

def make_man():
	file_paths = []  # List which will store all of the full filepaths.
	out=""
	files=glob.glob("*.dat")
	section_one=[]
	section_two=[]
	text_list=[]
	for filename in files:

		if filename.endswith(".dat")==True:

			temp=dat_file()
			ret=plot_load_info(temp,filename)
			if ret==True:

				section_one.append(temp.section_one)
				section_two.append(temp.section_two)
				out=""
				filename=filename.replace("_","\\_")
				out=out+"\\textbf{"+filename+"}:"+ temp.title+"\\newline\n"
				out=out+"x-axis:"+temp.x_label+"($"+temp.x_units+"$)\\newline\n"
				out=out+"y-axis:"+temp.data_label+"($"+temp.data_units+"$)\\newline\n"
				out=out+"\\newline\n"
				text_list.append(out)


				file_paths.append(filename)

	section_one, section_two, text_list = zip(*sorted(zip(section_one, section_two,text_list),key = operator.itemgetter(1, 2)))



	text_file = open("./man_src/files.tex", "w")
	last_section_one=""
	last_section_two=""
	for i in range(0,len(text_list)):
		if section_one[i]!="":
			if last_section_one!=section_one[i]:
				last_section_one=section_one[i]
				text_file.write("\subsubsection{"+section_one[i]+"}\n")

			if last_section_two!=section_two[i]:
				last_section_two=section_two[i]
				text_file.write("\paragraph{"+section_two[i]+"}\n")

			text_file.write(text_list[i])
	text_file.close()


