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

## @package util_latex
#  Latex helper routines
#

import os

class latex:
	def __init__(self):
		self.lines=[]

	def tab_start(self,labels):
		self.lines.append("\\begin{table}[H]\n")
		self.lines.append("\\begin{center}\n")
		a="\\begin{tabular}{|"
		for l in labels:
			a=a+"l|"
		a=a+"}\n"
		self.lines.append(a)

		self.lines.append("  \\hline\n")
		a="  "
		for l in labels:
			a=a+l+"&\t"
		a=a[:-2]
		a=a+"\\\\ \n"
		self.lines.append(a)

		self.lines.append("  \\hline\n")

	def tab_add_row(self,values):
		a=""
		for v in values:
			a=a+v+" &"
		a=a[:-1]
		a=a+"\\\\  \n"
		self.lines.append(a)

	def document_start(self):

		self.lines.append("\\documentclass{article}\n")
		self.lines.append("\\providecommand{\\e}[1]{\\ensuremath{\\times 10^{#1}}}\n")
		self.lines.append("\\begin{document}\n")
		self.lines.append("\\pagenumbering{gobble}\n")
		self.lines.append("\n")


	def latex_document_end(self):
		self.lines.append("\\end{document}\n")

	def tab_end(self,caption=None):
		self.lines.append("  \\hline\n")
		self.lines.append("\\end{tabular}\n")
		self.lines.append("\\end{center}\n")
		if caption!=None:
			self.lines.append("\\caption{"+caption+"}\n")
		self.lines.append("\\end{table}\n")
		self.lines.append("\n")

	def save(self,file_name):
		self.file_name=file_name
		dir_name=os.path.dirname(file_name)
		if os.path.isdir(dir_name)==False:
			os.mkdir(dir_name)

		out_file=open(file_name,"w")
		out_file.write("\n".join(self.lines))
		out_file.close()

	def make_jpg(self,ext=".jpg",makefile=False):
		#ext= os.path.splitext(self.file_name)[1]
		input_no_ext=os.path.splitext(self.file_name)[0]
		output_dir=os.path.dirname(self.file_name)
		if (ext==".pdf"):
			os.system("latex -interaction=batchmode --output-directory "+output_dir+" "+input_no_ext)
			os.system("dvipdf "+input_no_ext+".dvi")
			#os.system("mv "+input_no_ext+".pdf "+output)

		if (ext==".jpg"): 
			command0="latex -interaction=batchmode --output-directory "+output_dir+" "+input_no_ext
			command1="convert -trim -bordercolor White -border 20x10 +repage -density 300 "+input_no_ext+".dvi "+input_no_ext+".jpg"
			os.system(command0)
			os.system(command1)

		if makefile==True:
			out_file=open(os.path.join(output_dir,"makefile"),"w")
			out_file.write("main:\n")
			out_file.write("\tlatex -interaction=batchmode "+os.path.basename(input_no_ext)+"||true\n")
			out_file.write("\tconvert -trim -bordercolor White -border 20x10 +repage -density 300 "+os.path.basename(input_no_ext)+".dvi "+os.path.basename(input_no_ext)+".jpg\n")
			out_file.close()

	def number_to_latex(self,data):
		if type(data)==str:
			data=float(data)

		if data>0.01 and data<1000.0:
			return "{:.3f}".format(data)

		ret="%1.1e" % data
		if (ret.count('e')!=0):
			a,b=ret.split('e')
			b=b.replace("+","")
			ret=a+"\\e{"+b+"}"

		return ret

	def numbers_to_latex(self,data):
		out=""
		number=False
		open_ten=False
		for i in range(0,len(data)):
			if str.isdigit(data[i])==True and number==False:
				out=out+""#$
				number=True

			if number==True:
				add=data[i]

				if number==True:
					if data[i]=="e":
						add="\\times10^{"
						open_ten=True
					if str.isdigit(data[i])==True:
						add=data[i]
					else:
						if data[i]!="e" and data[i]!="-" and data[i]!="+" and data[i]!=".":
							number=False
							add=""+data[i] #$
							if open_ten==True:
								add="}"+data[i] #$
								open_ten=False
				out=out+add
			else:
				out=out+data[i]
		if open_ten==True:
			out=out+"}"#$
			number=False

		if number==True:
			out=out+"" #$

		return out

def str_to_latex(in_string):
	out_string=in_string.replace("_","\\_")
	return out_string



