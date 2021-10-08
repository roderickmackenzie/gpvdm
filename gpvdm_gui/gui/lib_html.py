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

## @package html_latex
#  html helper routines
#

import os

class lib_html:
	def __init__(self):
		self.lines=[]

	def tab_start(self,labels):
		self.lines.append("<table>\n")
		self.lines.append("<tr>\n")
		for l in labels:
			self.lines.append(" <th>"+l+"</th>\n")
		self.lines.append("</tr>\n")

	def tab_add_row(self,values):
		self.lines.append("<tr>\n")
		for v in values:
			self.lines.append("<td>"+v+"</td>\n")
		self.lines.append("</tr>\n")

	def document_start(self):
		self.lines.append("<html>\n")
		self.lines.append("<head>\n")
		self.lines.append("<style>\n")
		self.lines.append("table {\n")
		self.lines.append("  font-family: arial, sans-serif;\n")
		self.lines.append("  border-collapse: collapse;\n")
		self.lines.append("  width: 100%;\n")
		self.lines.append("}\n")
		self.lines.append("\n")
		self.lines.append("td, th {\n")
		self.lines.append("  border: 1px solid #dddddd;\n")
		self.lines.append("  text-align: left;\n")
		self.lines.append("  padding: 8px;\n")
		self.lines.append("}\n")
		self.lines.append("\n")
		self.lines.append("tr:nth-child(even) {\n")
		self.lines.append("  background-color: #dddddd;\n")
		self.lines.append("}\n")
		self.lines.append("</style>\n")
		self.lines.append("</head>\n")
		self.lines.append("<body>\n")


	def document_end(self):
		self.lines.append("</body>\n")
		self.lines.append("</html>\n")

	def tab_end(self):
  		self.lines.append("</table>\n")

	def save(self,file_name):
		self.file_name=file_name
		dir_name=os.path.dirname(file_name)
		if os.path.isdir(dir_name)==False:
			os.mkdir(dir_name)

		out_file=open(file_name,"w")
		out_file.write("\n".join(self.lines))
		out_file.close()


	def number_to_html(self,data):
		if type(data)==str:
			data=float(data)

		if data>0.01 and data<1000.0:
			return "{:.3f}".format(data)

		ret="%1.1e" % data
		if (ret.count('e')!=0):
			a,b=ret.split('e')
			b=b.replace("+","")
			ret=a+"x10<sup>"+b+"</sup>"

		return ret




