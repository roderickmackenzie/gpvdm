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
import copy

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

	def __init__(self,name,segment_class=False,segment_example=None):
		self.include_name=True
		self.base_name=name
		self.var_list=[]
		self.segment_class=segment_class
		self.segments=[]
		self.loaded=False
		self.latex_allowed=[]
		self.latex_banned=[]
		self.triangles_loaded=False
		self.segments_name=["segments"]
		self.segment_name=["segment"]
		self.f=inp()
		self.segment_examples=[segment_example]

	def import_from_list(self,lines):
		lines="\n".join(lines)
		j_data=json.loads(lines)
		self.load_from_json(j_data)

	def find_enabled_segment(self):
		for sn in self.segments_name:
			for s in getattr(self, sn):
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
		for s in self.segments_name:
			setattr(self, s, [])
		
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

		for sn in self.segments_name:
			for s in getattr(self, sn):
				val=getattr(s, "id","not found")
				if val==want_id:
					return s
				elif isclass(s)==True:
					ret=s.find_object_by_id(want_id)
					if ret!=None:
						return ret
		return None

	def find_object_path_by_id(self,want_id,cur_path=""):
		#print(self.segments_name)
		for item in self.var_list:
			m=item[0]
			val=getattr(self, m)
			if m=="id":
				if val==want_id:
					return self,cur_path
			elif isclass(val)==True:
				ret,new_path=val.find_object_path_by_id(want_id,cur_path=cur_path+"."+m)
				if ret!=None:
					return ret,new_path

		for sn in self.segments_name:
			s_obj=getattr(self, sn)
			#print(sn,s_obj,want_id)
			for s_int in range(0,len(s_obj)):
				val=getattr(s_obj[s_int], "id","not found")
				if val==want_id:
					return s_obj[s_int], cur_path+"."+sn+"["+str(s_int)+"]"
				elif isclass(s_obj[s_int])==True:
					ret,new_path=s_obj[s_int].find_object_path_by_id(want_id,cur_path=cur_path+"."+sn+"["+str(s_int)+"]")
					if ret!=None:
						return ret,new_path
		return None,None

	def pop_object_by_id(self,want_id):
		items=[attr for attr in dir(self) if not callable(getattr(self, attr)) and not attr.startswith("__")]
		for item in items:
			val=getattr(self, item)
			if isclass(val)==True:
				if getattr(val, "pop_object_by_id", None)!=None:
					val.pop_object_by_id(want_id)
			elif type(val)==list:
				for i in range(0,len(val)):
					if isclass(val[i])==True:
						item_id=getattr(val[i], "id", None)
						if item_id==want_id:
							val.pop(i)
							return
						if getattr(val[i], "pop_object_by_id", None)!=None:
							val[i].pop_object_by_id(want_id)

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

		for sn in self.segments_name:
			for s in getattr(self, sn):
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

		for sn in self.segments_name:
			for item in getattr(self, sn):
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

	def set_val(self,path,val):
		pointer=self
		for m in path.split("/"):
			last=pointer
			if m.startswith("section"):
				n=int(m[7:])
				pointer=pointer.sections[n]
			elif m.startswith("layer"):
				n=int(m[5:])
				pointer=pointer.layers[n]
			else:
				pointer=getattr(pointer, m)

		setattr(last, m, val)

	def gen_json(self,include_bracket=True):
		out=[]


		if self.include_name==True:
			name_str="\""+self.base_name+"\":"
		else:
			name_str=""
		if include_bracket==True:
			out.append(name_str +" {")

		for item in self.var_list:
			m=item[0]
			val=getattr(self, m)
			if m.startswith("text_")==False:
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

		if self.segment_class==True:
			for sn,item_name in zip(self.segments_name,self.segment_name):
				out.append("\""+sn+"\":"+str(len(getattr(self,sn)))+",")
				i=0
				for s in getattr(self,sn):
					s.base_name=item_name+str(i)
					out.extend(s.gen_json())
					out[-1]=out[-1]+","
					i=i+1

		if out[-1].endswith(",")==True:
			out[-1]=out[-1][:-1]
		if include_bracket==True:
			out.append("\t}")

		return out

	def dump_as_latex(self,token_lib=None):
		ret=[]
		class ret_data():
			def __init__(self):
				self.text=""
				self.value=""
				self.units=""
				self.token=item

		for item in self.var_list:
			dump=True
			m=item[0]
			if "all" in self.latex_banned:
				dump=False
				if m in self.latex_allowed:
					dump=True

			if m in self.latex_banned:
				dump=False

			if dump==True:
				value=str(getattr(self, m))
				token_data=token_lib.find_json(m)
				if token_data!=False:

					if is_number(value)==True:
						r=ret_data()
						r.text=token_data.info
						r.value=latex().number_to_latex(value)
						r.units=pygtk_to_latex_subscript(token_data.units)
						r.token=m
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
						getattr(self,m).load_from_json(data[m])

		if self.segment_class==True:
			for sn,item_name,segment_example in zip(self.segments_name,self.segment_name,self.segment_examples):
				setattr(self,sn,[])
				if sn in data:
					segs=data[sn]
					for i in range(0,segs):
						a=copy.deepcopy(segment_example)
						simulation_name=item_name+str(i)
						a.load_from_json(data[simulation_name])
						getattr(self,sn).append(a)



	def reload(self):
		if self.f.load_json(self.file_name)!=False:
			self.load_from_json(self.f.json)
			self.last_time=self.f.time()
			self.loaded=True
			self.triangles_loaded=False

	def load(self,file_name):
		#print("gpvdm _load")
		self.f.set_file_name(file_name)
		self.file_name=file_name
		self.reload()
		if self.loaded==False:
			return False
		return True

	def random_id(self):
		return "id"+codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()

	def save_as(self,file_name,do_tab=True):
		self.file_name=file_name
		lines=self.gen_json()
		self.f.lines=lines
		if do_tab==True:
			self.f.tab()
		else:
			tot=""
			for i in range(0,len(self.f.lines)):
				self.f.lines[i]=self.f.lines[i].replace("\t","")
		self.f.save_as(file_name)
		self.last_time=self.f.time()

	def save(self,do_tab=True):
		if self.loaded==True:
			lines=self.gen_json()
			self.f.lines=lines
			if do_tab==True:
				self.f.tab()
			else:
				tot=""
				for i in range(0,len(self.f.lines)):
					self.f.lines[i]=self.f.lines[i].replace("\t","")
			self.f.save()
			self.last_time=self.f.time()

	def load_triagles(self):
		for l in self.epi.layers:
			l.load_triangles()
			for s in l.shapes:
				s.load_triangles()
				if s.triangles==None:
					s.shape_enabled=False

		for c in self.epi.contacts.segments:
			c.load_triangles()

		for obj in self.world.world_data.segments:
			obj.load_triangles()
			if obj.triangles==None:
				obj.shape_enabled=False

		self.triangles_loaded=True
