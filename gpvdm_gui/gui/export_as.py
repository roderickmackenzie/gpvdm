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

## @package export_as
#  Export the simulation as a latex file or jpg
#



import os
from inp import inp_load_file
from inp import inp_search_token_value
from token_lib import tokens
from util import pygtk_to_latex_subscript


import glob
from cal_path import get_sim_path
from util_latex import latex


def export_as(output):	#This needs rewriting using util_latex and inp.inp_to_latex but no time now
	tex=True
	dollar="$"
	col=" & "
	eol=" \\\\"
	ext= os.path.splitext(output)[1]
	line=""
	if (ext==".pdf") or (ext==".jpg") or (ext==".tex")  or (ext==".csv"):
		print(ext)
		if ext==".csv":
			tex=False
			dollar=""
			col="\t"
			eol=""

		lines=[]
		if tex==True:
			line=line+"\\documentclass{article}\n"
			line=line+"\\providecommand{\\e}[1]{\\ensuremath{\\times 10^{#1}}}\n"
			line=line+"\\begin{document}\n"
			line=line+"\\pagenumbering{gobble}\n"
			line=line+"\n"
		files=[]

		f_list=glob.iglob(os.path.join("./", "dos*.inp"))
		for in_file in f_list:
			files.append(in_file)
		print(files)


		files=["./device.inp","./ray.inp","./device_epitaxy.inp", "./stark.inp", "./materials/redf/fit.inp", "./materials/redf/patch.inp"]
		names=["Device","Ray","Device Epitaxy","Stark","Fit redf","Fit patch"]
		if tex==True:
			line=line+"\\begin{table}[H]\n"
			line=line+"\\begin{center}\n"
			line=line+"  \\begin{tabular}{lll}\n"
			line=line+"  \\hline\n"

		line=line+"  Parameter"+col+"label"+col+"unit "+eol+"\n"

		if tex==True:
			line=line+"  \\hline\n"

		config_lines=[]
		cur_file=0
		config_lines=inp_load_file("latex_export_info.inp")
		for cur_file in range(0,len(files)):


			if os.path.isfile(files[cur_file])==True:
				lines=inp_load_file(files[cur_file])
				t=tokens()

				for i in range(0,len(lines),2):
					dump_token=inp_search_token_value(config_lines, lines[i])
					if dump_token=="1":
						my_token=t.find(lines[i])
						if my_token!=False:
							if my_token.number_type=="e":
								number=""
								if tex==True:
									#print lines
									#print lines[i]
									number=latex.number_to_latex(dos_lines[ii][i+1])
								else:
									number=dos_lines[ii][i+1]
								line=line+my_token.info+col+dollar+number+dollar+col+dollar+pygtk_to_latex_subscript(my_token.units)+dollar+eol+"\n"

		if tex==True:
			line=line+"  \\hline\n"
			line=line+"\\end{tabular}\n"
			line=line+"\\end{center}\n"
			line=line+"\\caption{"+names[cur_file]+"}\n"
			line=line+"\\end{table}\n"
			line=line+"\n"

		if tex==True:
			line=line+"\\end{document}\n"

		text_file = open("doc.tex", "w")
		text_file.write(line)
		print(line)
		text_file.close()

		if (ext==".pdf"):
			os.system("latex -interaction=batchmode doc")
			os.system("dvipdf doc.dvi")
			os.system("mv doc.pdf "+output)

		if (ext==".jpg"):
			os.system("latex -interaction=batchmode doc")
			os.system("convert -trim -bordercolor White -border 20x10 +repage -density 300 doc.dvi doc.jpg")
			os.system("mv doc.jpg "+output)

		if (ext==".tex"):
			os.system("mv doc.tex "+output)

		if (ext==".csv"):
			os.system("mv doc.tex "+output)

		#cmd = 'tar -czvf '+output+' ./*.inp ./sim.gpvdm ./*.dat '
		#os.system(cmd)



