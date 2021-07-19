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


## @package json_base
#  Used to store json_base
#


import sys
import os
import shutil
import json
import inspect
from str2bool import str2bool
from util_text import is_number
from util_latex import latex
from util import pygtk_to_latex_subscript
from inp import inp
import codecs

def del_keys(dic, del_key):
    dict_foo = dic.copy()
    for field in dict_foo.keys():
        if field == del_key:
            del dic[field]
        if type(dict_foo[field]) == dict:
            del_keys(dic[field], del_key)
    return dic

def isclass(object):
	if "gen_json" in dir(object):
		return True
	return False

class json_base():

	def __init__(self,name,segment_class=False):
		self.include_name=True
		self.base_name=name
		self.var_list=[]
		self.segment_class=segment_class
		self.segments=[]
		self.loaded=False
		self.latex_allowed=[]
		self.latex_banned=[]


		self.f=inp()

	def import_from_list(self,lines):
		lines="\n".join(lines)
		j_data=json.loads(lines)
		self.load_from_json(j_data)

	def find_enabled_segment(self):
		for s in self.segments:
			if s.enabled==True:
				return s

	def import_raw_json(self,json_data):
		self.var_list=[]
		for token, val in json_data.items():
			self.var_list.append([token,val])
		self.var_list_build()

	def var_list_build(self):
		for l in self.var_list:
			setattr(self, l[0], l[1])

	def find_object_by_id(self,want_id):
		for item in self.var_list:
			m=item[0]
			val=getattr(self, m)
			if m=="id":
				if val==want_id:
					return self
			elif isclass(val)==True:
				ret=val.find_object_by_id(want_id)
				if ret!=None:
					return ret

		for s in self.segments:
			val=getattr(s, "id",None)
			if val==want_id:
				return s
			elif isclass(val)==True:
				ret=val.find_object_by_id(want_id)
				if ret!=None:
					return ret
		return None

	def fix_identical_uids(self,found):
		for item in self.var_list:
			m=item[0]
			val=getattr(self, m)
			if m=="id":
				if val in found:
					print("warning updated ID",val)
					self.id=self.random_id()
				found.append(val)
			elif isclass(val)==True:
				val.fix_identical_uids(found)

		for s in self.segments:
			val=getattr(s, "id",None)
			if val!=None:
				if val in found:
					print("warning updated ID",val)
					s.id=self.random_id()
				found.append(val)
			elif isclass(val)==True:
				val.fix_identical_uids(found)

		print(found)

	def update_random_ids(self):
		for item in self.var_list:
			m=item[0]
			val=getattr(self, m)
			if m=="id":
				#print(self)
				setattr(self, m, self.random_id()	)
			elif isclass(val)==True:
				try:
					val.update_random_ids()
				except:
					pass

	def gen_json(self,include_bracket=True):
		out=[]

		if self.segment_class==False:

			if self.include_name==True:
				name_str="\""+self.base_name+"\":"
			else:
				name_str=""
			if include_bracket==True:
				out.append(name_str +" {")

			for item in self.var_list:
				m=item[0]
				val=getattr(self, m)
				if type(val)==str:
					val=val.replace("\n","\\n")
					out.append("\t\""+m+"\":\""+val+"\",")
				elif type(val)==int:
					out.append("\t\""+m+"\":"+str(val)+",")
				elif type(val)==float:
					out.append("\t\""+m+"\":"+str(val)+",")
				elif type(val)==bool:
					out.append("\t\""+m+"\":\""+str(val)+"\",")
				elif isclass(val)==True:
					#print(type(val))
					out.extend(val.gen_json())
					out[-1]=out[-1]+","
				else:
					print(m,type(val))
			if out[-1].endswith(",")==True:
				out[-1]=out[-1][:-1]
			if include_bracket==True:
				out.append("\t}")
		else:
			out.append("\""+self.base_name+"\": {")
			out.append("\"segments\":"+str(len(self.segments))+",")
			i=0
			for s in self.segments:
				s.base_name="segment"+str(i)
				out.extend(s.gen_json())
				out[-1]=out[-1]+","
				i=i+1
			out[-1]=out[-1][:-1]
			out.append("}")

		return out

	def dump_as_latex(self,token_lib=None):
		ret=[]
		class ret_data():
			def __init__(self):
				self.text=""
				self.value=""
				self.units=""

		for item in self.var_list:
			dump=True
			m=item[0]
			if "all" in self.latex_banned:
				dump=False
				if m in self.latex_allowed:
					dump=True

			if dump==True:
				value=str(getattr(self, m))
				token_data=token_lib.find_json(m)
				if token_data!=False:

					if is_number(value)==True:
						r=ret_data()
						r.text=token_data.info
						r.value=latex().number_to_latex(value)
						r.units=pygtk_to_latex_subscript(token_data.units)
						ret.append(r)

		return ret

	def load_from_json(self,data):
		for item in self.var_list:
			m=item[0]
			val=item[1]
			#print(data)
			#print(data[m],type(getattr(self,m)),type(val))
			decoded=False
			if m=="time_domain":
				if 'pulse' in data:
					self.time_domain.load_from_json(data['pulse'])
					decoded=True
			elif m=="fx_domain":
				if "is" in data:
					self.fx_domain.load_from_json(data['is'])
					decoded=True
			
			if decoded==False:
				if m in data:
					if type(val)==float:
						try:
							clean_val=float(data[m])
						except:
							clean_val=0.0
						setattr(self, m, clean_val)
					elif type(val)==int:
						try:
							clean_val=int(data[m])
						except:
							clean_val=0
						setattr(self, m, clean_val)
					elif type(val)==bool:
						setattr(self, m, str2bool(data[m]))
					elif type(val)==type(data[m]):
						setattr(self, m, data[m])
					elif type(val)==str:
						setattr(self, m, str(data[m]))
					elif isclass(getattr(self,m))==True:
						#print(data[m])
						getattr(self,m).load_from_json(data[m])


	def reload(self):
		if self.f.load_json(self.file_name)!=False:
			self.load_from_json(self.f.json)
			self.last_time=self.f.time()
			self.loaded=True

	def load(self,file_name):
		self.f.set_file_name(file_name)
		self.file_name=file_name
		self.reload()
		if self.loaded==False:
			return False
		return True

	def random_id(self):
		return "id"+codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()

	def save_as(self,file_name):
		self.file_name=file_name
		lines=self.gen_json()
		self.f.lines=lines
		self.f.tab()
		self.f.save_as(file_name)
		self.last_time=self.f.time()

	def save(self):
		if self.loaded==True:
			lines=self.gen_json()
			self.f.lines=lines
			self.f.tab()
			self.f.save()
			self.last_time=self.f.time()

