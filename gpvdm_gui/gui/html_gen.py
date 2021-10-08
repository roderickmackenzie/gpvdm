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

## @package html_gen
#  Html page builder for gpvdm
#


import os

from cal_path import get_sim_path
from inp import inp_save_lines_to_file

class html_obj:
	def __init__(self):
		self.type=""
		self.data=[]

	def __str__(self):
		if self.type=="p":
			return "<p>"+self.data+"</p>"
		elif self.type=="h1":
			return "<h1>"+self.data+"</h1>"
		elif self.type=="h2":
			return "<h2>"+self.data+"</h2>"
		elif self.type=="h1":
			return "<h3>"+self.data+"</h3>"
		elif self.type=="tab":
			build="<table>\n"
			for l in self.data:
				build=build+"<tr>\n"
				for c in l:
					build=build+"<th>"+c+"</th>\n"
				build=build+"</tr>\n"

			build=build+"</table>\n"
			return build
		elif self.type=="collapsible":
			build="<button type=\"button\" class=\"collapsible\">"+self.data+"</button>\n"
			return build
		elif self.type=="div":
			return "<div class=\""+self.data+"\">"
		elif self.type=="end_div":
			return "</div>"
		elif self.type=="br":
			return "<br>"

class html_gen:

	def __init__(self):
		self.items=[]

	def add_p(self,data):
		a=html_obj()
		a.type="p"
		a.data=data
		self.items.append(a)

	def add_h1(self,data):
		a=html_obj()
		a.type="h1"
		a.data=data
		self.items.append(a)

	def add_h2(self,data):
		a=html_obj()
		a.type="h2"
		a.data=data
		self.items.append(a)

	def add_tab(self,data):
		a=html_obj()
		a.type="tab"
		a.data=data
		self.items.append(a)

	def add_collapsible(self,data):
		a=html_obj()
		a.type="collapsible"
		a.data=data
		self.items.append(a)

	def add_div(self,data):
		a=html_obj()
		a.type="div"
		a.data=data
		self.items.append(a)


	def end_div(self):
		a=html_obj()
		a.type="end_div"
		a.data=""
		self.items.append(a)

	def add_br(self):
		a=html_obj()
		a.type="br"
		a.data=""
		self.items.append(a)

	def gen_list(self):
		self.out=[]

		self.out.append("<!DOCTYPE html>")
		self.out.append("<head>")
		self.out.append("<style>")
		self.out.append("table {")
		#style for the table
		self.out.append("  font-family: arial, sans-serif;")
		self.out.append("  border-collapse: collapse;")
		self.out.append("  width: 50%;")
		self.out.append("}")
		self.out.append("")
		self.out.append("td, th {")
		self.out.append("  border: 1px solid #dddddd;")
		self.out.append("  text-align: left;")
		self.out.append("  padding: 8px;")
		self.out.append("}")
		self.out.append("")
		self.out.append("tr:nth-child(even) {")
		self.out.append("  background-color: #dddddd;")
		self.out.append("}")
		#collapsible
		self.out.append(".collapsible {")
		self.out.append("  background-color: #3333ff;")
		self.out.append("  color: white;")
		self.out.append("  cursor: pointer;")
		self.out.append("  padding: 18px;")
		self.out.append("  width: 25%;")
		self.out.append("  border: none;")
		self.out.append("  text-align: left;")
		self.out.append("  outline: none;")
		self.out.append("  font-size: 15px;")
		self.out.append("}")

		self.out.append(".active, .collapsible:hover {")
		self.out.append("  background-color: #555;")
		self.out.append("}")

		self.out.append(".content {")
		self.out.append("  padding: 0 18px;")
		self.out.append("  display: none;")
		self.out.append("  overflow: hidden;")
		self.out.append("  background-color: #f1f1f1;")
		self.out.append("  }")
		self.out.append("</style>")
		self.out.append("</head>")
		self.out.append("<body>")
		self.out.append("<body>")
		self.out.append("<html>")

		for i in self.items:
			self.out.append(str(i))


		self.out.append("<script>")
		self.out.append("var coll = document.getElementsByClassName(\"collapsible\");")
		self.out.append("var i;")
		self.out.append("")
		self.out.append("for (i = 0; i < coll.length; i++) {")
		self.out.append("  coll[i].addEventListener(\"click\", function() {")
		self.out.append("    this.classList.toggle(\"active\");")
		self.out.append("    var content = this.nextElementSibling;")
		self.out.append("    if (content.style.display === \"block\") {")
		self.out.append("      content.style.display = \"none\";")
		self.out.append("    } else {")
		self.out.append("      content.style.display = \"block\";")
		self.out.append("    }")
		self.out.append("  });")
		self.out.append("}")
		self.out.append("</script>")
		self.out.append("</body>")
		self.out.append("</html>")
	
	def save(self,file_name):
		inp_save_lines_to_file(file_name,self.out)

