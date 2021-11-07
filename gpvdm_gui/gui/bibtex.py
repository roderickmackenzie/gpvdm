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

## @package bibtex
#  A bibtex parser for gpvdm
#

import os
from inp import inp
from json_base import json_base
import operator

class bib_item(json_base):
	def __init__(self):
		json_base.__init__(self,"bibtex")
		self.type=""
		self.token=""
		self.var_list=[]
		self.var_list.append(["author",""])
		self.var_list.append(["title",""])
		self.var_list.append(["journal",""])
		self.var_list.append(["volume",""])
		self.var_list.append(["number",""])
		self.var_list.append(["pages",""])
		self.var_list.append(["year",""])
		self.var_list.append(["DOI",""])
		self.var_list.append(["publisher",""])
		self.var_list.append(["address",""])
		self.var_list.append(["booktitle",""])
		self.var_list.append(["isbn",""])
		self.var_list.append(["unformatted",""])
		self.var_list_build()

		self.year_int=1970

	def bib_short_cite(self):
		build=""
		if self.title!="":
			build=build+self.title+", "

		if self.journal!="":
			build=build+self.journal+", "

		if self.volume!="":
			build=build+self.volume+", "

		if self.pages!="":
			build=build+self.pages+", "

		if self.year!="":
			build=build+self.year+", "

		build=build[:-2]

		return build

	def bib_decode(self,lines):
		l=lines[0][1:]
		if l[-1]==",":
			l=lines[0][:-1]

		l=l.split("{")
		self.type=l[0]
		self.token=l[1]
		if len(lines)<=2:
			return

		lines=lines[1:-1]
		lines[-1]=lines[-1]+","

		wait_for="="
		build=""
		for l in lines:
			ls=l.rstrip()
			for i in range(0,len(ls)):
				c=ls[i]
				if wait_for==",end":
					if c=="," and i==(len(ls)-1):
						build_strip=build.strip()
						if build_strip[0]=="\"":
							build_strip=build_strip[1:]
						if build_strip[-1]=="\"":
							build_strip=build_strip[:-1]

						text=build_strip
						if hasattr(self, token):
							if text.startswith("{") and text.endswith("}"):
								text=text[1:-1]
							setattr(self,token,text)

						wait_for="="
						build=""

						#print("token=",token)
						#print("text=",text)
					else:
						build=build+c

				elif wait_for=="=":
					if c=="=":
						token=build.strip()
						wait_for=",end"
						build=""
					else:
						build=build+c
					
		try:
			self.year_int=int(self.year)
		except:
			pass
		new_lines=[]


	def bib_get_as_str(self):
		build="@"+self.type+"{"+self.token+",\n"

		for var, orig_val in self.var_list:
			val=getattr(self, var)
			if val!="" and val!=False:
				build=build+" "+var+" = \""+val+"\",\n"

		build=build[:-2]
		build=build+"\n}"
		return build

	def bib_dump(self):
		print(self.bib_get_as_str())

	def get_text(self,html=True):
		text=""
		if self.title=="":
			return self.unformatted

		if html==True:
			text=text+self.clean_author(self.author)+", "
			if self.title!="":
				text=text+"\""+self.title.capitalize()+"\""+", "

			if self.journal!="":
				text=text+self.journal+", "

			if self.volume!="":
				text=text+self.volume+", "

			if self.pages!="":
				text=text+self.pages.replace("--","-")+", "

			if self.year!="":
				text=text+self.year+", "

			if text.endswith(", ")==True:
				text=text[:-2]

			text=text+"<br>\n"
			
			if self.DOI!="":
				text=text+"<b>doi link:</b> <a href=\"http://doi.org/"+self.DOI+"\"> http://doi.org/"+self.DOI+"</a>"
		else:
			text=text+self.author+", "+self.journal+", "+self.title+", "+self.volume+", "+self.pages+", "+self.year+" "+self.DOI

		return text

	def clean_author(self,author):
		au=author.split(" and ")
		n=0
		ret=""
		for a in au:
			name=a
			s=a.split(",")
			if len(s)>1:
				first_name=s[1].strip()
				if len(first_name)>1:
					first_name=first_name[0]
				first_name=first_name
				name=s[0]+" "+first_name
			if ret!="":
				ret=ret+", "
			ret=ret+name
		return ret

class bibtex:
	def __init__(self):
		self.refs=[]

	def load(self,file_name):
		self.file_name=file_name

		f=inp()
		if f.load(file_name)==False:
			return False

		bracket=0
		within=False
		ref_lines=[]
		for l in f.lines:
			if l.lstrip().startswith("@"):
				if bracket!=0:
					print("error parsing bibtex file")
				within=True
				ref_lines=[]

			bracket=bracket+l.count("{")
			bracket=bracket-l.count("}")

			if within==True:
				ref_lines.append(l)

			if bracket==0:
				if within==True:
					item=bib_item()
					item.save=self.save
					item.bib_decode(ref_lines)
					self.refs.append(item)

				within=False
		return True

	def sort(self):
		sorted_x = sorted(self.refs, key=operator.attrgetter('year_int'),reverse =True)
		self.refs=sorted_x

	def new(self):
		item=bib_item()
		item.type="article"
		item.save=self.save
		self.refs.append(item)
		return self.refs[-1]

	def dump(self):
		for r in self.refs:
			print(r.bib_get_as_str())

	def save(self):
		self.save_as(self.file_name)

	def save_as(self,file_name):
		lines=[]
		for r in self.refs:
			lines.append(r.bib_get_as_str()+"\n")
		f=inp()
		f.lines=lines
		f.save_as(file_name)

	def get_text(self,html=True,numerate=True):
		out=""
		n=1
		year=self.refs[0].year
		out=out+"<h2>"+year+"</h2>"
		for r in self.refs:
			if html==True:
				if year!=r.year:
					out=out+"<h1>"+r.year.replace("2000","Unknown year")+"</h1>"
				year=r.year
				text="<p>"
				if numerate==True:
					text=text+"["+str(n)+"] "
				text=text+r.get_text()+"</p>\n"
				out=out+text
			else:
				out=out+r.get_text()+"\n"
			n=n+1
		return out

	def get_text_of_token(self,token,html=False):

		for r in self.refs:
			if r.token==token:
				if html==True:
					return r.get_text()+"<br>\n"
				else:
					return r.get_text()+"\n"

		return False

	def get_ref(self,token):
		for r in self.refs:
			if r.token==token:
				return r

		return False

