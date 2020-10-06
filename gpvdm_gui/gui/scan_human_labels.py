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

## @package scan_human_labels
#  A class to turn the .inp into human labels
#
#import sys
import os
from inp import inp_load_file
from token_lib import tokens
from materials_io import find_materials
from cal_path import get_materials_path
from util_zip import zip_lsdir
from cal_path import get_sim_path
from inp import inp_get_token_value

from epitaxy import epitaxy_dos_file_to_layer_name


from util import is_numbered_file
from epitaxy import get_epi
from mesh import get_mesh
from cal_path import subtract_paths

import re

#import shutil
class scan_item:
	human_label=""
	token=""
	filename=""

class scan_human_labels():

	def __init__(self):
		self.list=[]
		self.my_token_lib=tokens().get_lib()
		self.epi=get_epi()
		self.mesh=get_mesh()
		#print("init!!!!!!!!!!!!!!!!!!!!!!!!!")

	def clear(self):
		self.list=[]
		#print("WTF!!!!!!!!!!!!!")

	def add_item(self,file_name,token,human_label):
		item=scan_item()
		human_label=human_label.replace("<sub>","")
		human_label=human_label.replace("</sub>","")
		item.human_label=human_label
		item.filename=file_name
		item.token=token
		self.list.append(item)

	def populate_from_known_tokens(self):
		for item in self.my_token_lib:
			if item.file_name!="":
				self.add_item(item.file_name,item.token,os.path.join(os.path.splitext(item.file_name)[0],item.info))

	def populate_from_files(self):
		name=os.path.join(get_sim_path(),"sim.gpvdm")
		if os.path.isfile(name)==True:
			file_list=zip_lsdir(name)
		
			for i in range(0,len(file_list)):
				#print(file_list[i])
				if is_numbered_file(file_list[i],"dos")==True:
					#print("oops>>>",file_list[i],epitaxy_dos_file_to_layer_name(file_list[i]))
					name=epitaxy_dos_file_to_layer_name(file_list[i])
					if name!=False:
						self.populate_from_file(file_list[i],human_name=os.path.join("epitaxy",name,"dos"))

				if is_numbered_file(file_list[i],"lumo")==True:
					index=self.epi.find_layer_index_from_file_name(file_list[i])
					if index!=False:
						name=self.epi.layers[index].name
						self.populate_from_file(file_list[i],human_name=os.path.join("epitaxy",name,"lumo"))

				if is_numbered_file(file_list[i],"homo")==True:
					index=self.epi.find_layer_index_from_file_name(file_list[i])
					if index!=False:
						name=self.epi.layers[index].name
						self.populate_from_file(file_list[i],human_name=os.path.join("epitaxy",name,"homo"))

				if is_numbered_file(file_list[i],"jv")==True:
					name=inp_get_token_value(os.path.join(get_sim_path(),file_list[i]),"#sim_menu_name")
					name=name.split("@")[0]
					self.populate_from_file(file_list[i],human_name=os.path.join("jv",name))

				if is_numbered_file(file_list[i],"laser")==True:
					name=inp_get_token_value(os.path.join(get_sim_path(),file_list[i]),"#laser_name")
					self.populate_from_file(file_list[i],human_name=os.path.join("laser",name))

				if is_numbered_file(file_list[i],"time_mesh_config")==True:
					number=file_list[i][len("time_mesh_config"):-4]
					name=inp_get_token_value(os.path.join(get_sim_path(),"pulse"+number+".inp"),"#sim_menu_name")
					name=name.split("@")[0]
					self.populate_from_file(file_list[i],human_name=os.path.join("time_domain",name))

		for i in range(0,len(self.epi.layers)):
			l=self.epi.layers[i]
			self.add_item("epitaxy.inp","#layer_material_file"+str(i),os.path.join("epitaxy",str(l.name),_("Material type")))
			self.add_item(l.file_name+".inp","#shape_dy",os.path.join("epitaxy",str(l.name),_("dy")))
			for s in l.shapes:
				if s.shape_dos!="none":
					print(s.shape_dos)
					self.populate_from_file(s.shape_dos,human_name=os.path.join("epitaxy",str(l.name),s.name,"DoS"))


		contacts=self.epi.contacts.contacts

		for i in range(0,len(contacts)):
			self.add_item("contacts.inp","#contact_charge_density"+str(i),os.path.join("contacts",str(contacts[i].name),_("Charge density")))
			self.add_item("contacts.inp","#contact_applied_voltage"+str(i),os.path.join("contacts",str(contacts[i].name),_("Voltage")))
			self.add_item("contacts.inp","#shunt_resistance_sq"+str(i),os.path.join("contacts",str(contacts[i].name),_("Shunt resistance")))

		for i in range(0,len(self.mesh.y.layers)):
			self.add_item("mesh_y.inp","#mesh_layer_length"+str(i),os.path.join("mesh","y",_("Layer")+" "+str(i),_("dy")))
			self.add_item("mesh_y.inp","#mesh_layer_points"+str(i),os.path.join("mesh","y",_("Layer")+" "+str(i),_("Points")))
			self.add_item("mesh_y.inp","#mesh_layer_mul"+str(i),os.path.join("mesh","y",_("Layer")+" "+str(i),_("Multiplier")))
			self.add_item("mesh_y.inp","#mesh_layer_left_right"+str(i),os.path.join("mesh","y",_("Layer")+" "+str(i),_("Left or Right")))

		#scan_item_save("out.dat")
		#self.dump()

	def save(self,file_name):
		f = open(file_name,'w')
		f.write(str(len(self.list))+"\n")
		for i in range(0,len(self.list)):
			f.write(self.list[i].human_label+"\n")
			f.write(self.list[i].filename+"\n")
			f.write(self.list[i].token+"\n")
		f.close()

	def dump(self):
		for item in self.list:
			print(item.filename,item.token,item.human_label)

	def remove_file(self,file_name):
		new_list=[]
		for i in range(0,len(self.list)):
			if 	self.list[i].filename!=file_name:
				new_list.append(self.list[i])

		self.list=new_list


	def get_file_from_human_label(self,name):
		for item in self.list:
			if item.human_label==name:
				return item.filename

		return "notknown"

	def get_token_from_human_label(self,value):
		for item in self.list:
			if item.human_label==value:
				return item.token

		return "notknown"

	def get_human_label(self,filename,token):
		for item in self.list:
			if item.filename==filename and item.token==token:
				return item.human_label

		return "notknown"


	def get_index_from_human_label(self,item):
		for i in range(0,len(self.list)):
			if self.list[i].human_label==item:
				return i

		return -1

	def populate_from_file(self,filename,human_name=""):
		lines=[]
		if filename.endswith(".inp")==False:
			filename=filename+".inp"

		lines=inp_load_file(filename)
		if human_name=="":
			human_name=filename

		my_token_lib=tokens()

		for i in range(0, len(lines)):
			token=lines[i]
			if len(token)>0:
				if token[0]=="#":
					result=my_token_lib.find(token)
					
					if result!=False:
						#print(filename,token,result,self.get_human_label(filename,token))
						if self.get_human_label(filename,token)=="notknown":
							numbered=""
							if bool(re.match(".+_\d+$",token.strip()))==True:	#search for function_0
								numbered=re.search("\d+$",token).group(0)
							self.add_item(filename,token,os.path.join(human_name,numbered,result.info))


	def ls_dir(self,path):
		for h in self.list:
			print(subtract_paths(os.path.normpath(path),os.path.normpath(os.path.dirname("/"+h.human_label))),h.human_label)
			if os.path.normpath(path)==os.path.normpath(os.path.dirname(h.human_label)):
				print(h.human_label)

scan_labels=scan_human_labels()

def get_scan_human_labels():
	global scan_labels
	return scan_labels

