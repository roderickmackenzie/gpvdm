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

## @package epitaxy
#  The epitaxy class.
#

import os
import math
from inp import inp_save
from inp import inp_load_file
from util import isnumber

from cal_path import get_materials_path
from cal_path import get_default_material_path

from inp import inp_load_file
from inp import inp_search_token_array
from inp import inp_search_token_value
from inp import inp_remove_file

from inp import inp
from inp import inp_copy_file
from inp import inp_lsdir
from inp import inp

from cal_path import get_sim_path
from util_zip import archive_merge_file
from mesh import get_mesh
from inp import inp_update_token_value

from util import is_numbered_file
from contacts_io import contacts_io
from shape import shape

from gui_enable import gui_get
if gui_get()==True:
	from file_watch import get_watch
	from PyQt5.QtCore import pyqtSignal
	from PyQt5.QtWidgets import QWidget

class epi_layer(shape):
	def __init__(self):
		super().__init__()
		self.layer_type="other"
		self.interface_file="none"
		self.start=0.0
		self.end=0.0
		self.Gnp=0.0
		self.solve_optical_problem=True
		self.solve_thermal_problem=True

	def set_dy(self,data):
		if type(data)==float or type(data)==int:
			self.dy=float(data)
		if type(data)==str:
			try:
				self.dy=float(data)
			except:
				return False

		return True

	def cal_rgb(self):
		#print(self.r, self.g,self.b)
		if self.r==0.0 and self.g==0.8 and self.b==0.0:

			f=inp()
			if f.load(os.path.join(os.path.join(get_materials_path(),self.optical_material),"mat.inp"))==False:
				return

			ret=f.get_array("#red_green_blue")

			if ret!=False:
				self.r=float(ret[0])
				self.g=float(ret[1])
				self.b=float(ret[2])
				self.alpha=float(f.get_token( "#mat_alpha"))

class epitaxy():

	def __init__(self):
		#QWidget.__init__(self)
		self.layers=[]
		self.callbacks=[]
		self.contacts=contacts_io()
		self.loaded=False

	def dump_tree(self):
		for i in range(0,len(self.layers)):
			l=self.layers[i]
			print("layer:"+str(i)+" "+str(l.name)+","+l.dos_file)
			for s in l.shapes:
				print(s.name,s.shape_dos)

	def dump(self):
		lines=self.gen_output()
		for l in lines:
			print(l)

	def get_new_material_name(self):
		count=0
		while(1):
			found=False
			name="newlayer"+str(count)
			for l in self.layers:
				if name==l.name:
					found=True
					break

			if found==False:
				break
			count=count+1

		return name

	def get_all_electrical_files(self):
		ret=[]
		for l in self.layers:
			ret.extend(l.get_sub_files())

			#interface files
			if l.interface_file!="none":
				ret.append(l.interface_file+".inp")

		for c in self.contacts.contacts:
			ret.extend(c.get_sub_files())

		#print(ret)
		return ret

	def clean_unused_files(self):
		files=inp_lsdir("sim.gpvdm")
		tab=self.get_all_electrical_files()

		for i in range(0,len(files)):
			disk_file=files[i]
			#dos
			if is_numbered_file(disk_file,"dos")==True:
				if disk_file not in tab:
					inp_remove_file(disk_file)

			#electrical
			if is_numbered_file(disk_file,"electrical")==True:
				if disk_file not in tab:
					inp_remove_file(disk_file)

			#pl
			if is_numbered_file(disk_file,"pl")==True:
				if disk_file not in tab:
					inp_remove_file(disk_file)

			#shape files
			if is_numbered_file(disk_file,"shape")==True:
				if disk_file not in tab:
					inp_remove_file(disk_file)


			if is_numbered_file(disk_file,"lumo")==True:
				if disk_file not in tab:
					inp_remove_file(disk_file)

			#homo files
			if is_numbered_file(disk_file,"homo")==True:
				if disk_file not in tab:
					inp_remove_file(disk_file)


	def layer_to_index(self,index):
		if type(index)==int:
			return index
		else:
			for i in range(0,len(self.layers)):
				if self.layers[i].name==index:
					return i
		return -1

	def new_shape_file(self,layer):
		new_filename=self.gen_new_electrical_file("shape")

		mesh=get_mesh()
		my_shape=shape()
		my_shape.load(os.path.join(get_sim_path(),new_filename)+".inp")

		if layer!=None:
			my_shape.dy=layer.dy

		my_shape.dx=mesh.get_xlen()
		my_shape.dz=mesh.get_zlen()
		my_shape.shape_electrical=self.gen_new_electrical_file("electrical")
		my_shape.shape_nx=1
		my_shape.shape_ny=1
		my_shape.shape_nz=1
		my_shape.name="New shape"
		my_shape.optical_material="blends/p3htpcbm"
		my_shape.save()

		return my_shape

	def add_new_layer(self,pos=-1):
		if pos!=-1:
			pos=self.layer_to_index(pos)

		a=epi_layer()
		a.dy=100e-9

		a.pl_file="none"
		file_name=self.new_shape_file(None).file_name
		a.load(os.path.join(get_sim_path(),file_name))
		a.name=self.get_new_material_name()
		a.interface_file=self.gen_new_electrical_file("interface")
		a.save()

		a.shapes=[]
		a.lumo_file="none"
		a.homo_file="none"
		a.dos_file="other"

		a.r=1.0
		a.g=0
		a.b=0
		a.alpha=0.5
		if pos==-1:
			self.layers.append(a)
		else:
			self.layers.insert(pos, a)
		return a

	def remove_by_id(self,ids):
		if type(ids)==str:
			ids=[ids]

		for l in self.layers[:]:
			for s in l.shapes[:]:
				if s.id in ids:
					l.shapes.remove(s)

			if l.id in ids:
				self.layers.remove(l)


	def move_up(self,pos):
		pos=self.layer_to_index(pos)
		if pos<1:
			return

		self.layers.insert(pos-1, self.layers.pop(pos))

	def move_down(self,pos):
		pos=self.layer_to_index(pos)
		#print(pos)
		if pos>len(self.layers)-1 or pos<0:
			return

		self.layers.insert(pos+1, self.layers.pop(pos))

	def gen_output(self):
		lines=[]
		lines.append("#layers")
		lines.append(str(len(epi.layers)))

		layer=0
		for i in range(0,len(epi.layers)):
			lines.append("#layer_type"+str(layer))
			lines.append(str(epi.layers[i].layer_type))
			lines.append("#layer_dos_file"+str(layer))
			lines.append(epi.layers[i].dos_file)
			lines.append("#layer_pl_file"+str(layer))
			lines.append(epi.layers[i].pl_file)
			lines.append("#layer_base_shape"+str(layer))
			lines.append(epi.layers[i].file_name)
			lines.append("#layer_shape"+str(layer))
			if len(epi.layers[i].shapes)==0:
				lines.append("none")
			else:
				build=""
				for s in epi.layers[i].shapes:
					build=build+s.file_name+","
				build=build[:-1]
				lines.append(build)

			lines.append("#layer_lumo"+str(layer))
			lines.append(epi.layers[i].lumo_file)
			lines.append("#layer_homo"+str(layer))
			lines.append(epi.layers[i].homo_file)
			lines.append("#layer_interface"+str(layer))
			lines.append(epi.layers[i].interface_file)
			lines.append("#solve_optical_problem"+str(layer))
			lines.append(str(epi.layers[i].solve_optical_problem))
			lines.append("#solve_thermal_problem"+str(layer))
			lines.append(str(epi.layers[i].solve_thermal_problem))

			layer=layer+1

		lines.append("#ver")
		lines.append("1.41")
		lines.append("#end")
		return lines

	def save(self):
		lines=self.gen_output()
		inp_save(os.path.join(get_sim_path(),"epitaxy.inp"),lines,id="epitaxy")
		for l in epi.layers:
			l.save()

		ymesh=get_mesh().y
		ymesh.do_remesh(self.ylen_active())

	def update_layer_type(self,layer,data):
		l=self.layers[layer]

		if data=="active" and l.dos_file.startswith("dos")==False:

			l.dos_file=self.new_electrical_file("dos")

			mat_dir=os.path.join(get_materials_path(),l.optical_material)

			new_dos_file=l.dos_file+".inp"

			if inp().isfile(new_dos_file)==False:
				dos_path_generic=os.path.join(get_default_material_path(),"dos.inp")
				inp_copy_file(new_dos_file,dos_path_generic)

				dos_path_material=os.path.join(mat_dir,"dos.inp")
				if os.path.isfile(dos_path_material)==True:
					archive_merge_file(os.path.join(get_sim_path(),"sim.gpvdm"),os.path.join(mat_dir,"sim.gpvdm"),new_dos_file,"dos.inp")

		if data=="active" and l.pl_file.startswith("pl")==False:
			l.pl_file=self.gen_new_electrical_file("pl")

		if data=="active" and l.lumo_file.startswith("lumo")==False:
			l.lumo_file=self.gen_new_electrical_file("lumo")

		if data=="active" and l.homo_file.startswith("homo")==False:
			l.homo_file=self.gen_new_electrical_file("homo")


		if l.shape_electrical=="none" or inp().isfile(l.shape_electrical+".inp")==False:
			l.shape_electrical=self.gen_new_electrical_file("electrical")

		l.layer_type=data

		if data!="active":
			l.dos_file="none"
			l.pl_file="none"
			l.lumo_file="none"
			l.homo_file="none"


		self.clean_unused_files()

		#self.dump()

	def gen_new_electrical_file(self,prefix):
		new_file_name=self.new_electrical_file(prefix)
		full_new_file_name=new_file_name+".inp"
		db_file=os.path.join(get_default_material_path(),prefix+".inp")

		if inp().isfile(full_new_file_name)==False:
			inp_copy_file(full_new_file_name,db_file)

		return new_file_name

	def find_shape_by_id(self,id):

		for c in self.contacts.contacts:
			if c.id==id:
				return c

		for l in self.layers:
			if l.id==id:
				return l

			for s in l.shapes:
				if s.id==id:
					return s
		return None

	def find_layer_by_id(self,id):

		nl=0
		for c in self.contacts.contacts:
			if c.id==id:
				if c.position=="top":
					return 0

				if c.position=="bottom":
					return len(self.layers)-1

		for l in self.layers:

			if l.id==id:
				return nl

			for s in l.shapes:
				if s.id==id:
					return nl

			nl=nl+1

		return None

	def find_shape_by_file_name(self,shape_file):
		if shape_file.endswith(".inp"):
			shape_file=shape_file[:-4]
		for c in self.contacts.contacts:
			if c.file_name==shape_file:
				return c

		for l in self.layers:
			if l.file_name==shape_file:
				return l

			for s in l.shapes:
				if s.file_name==shape_file:
					return s
		return None

	def get_top_contact_layer(self):
		for l in range(0,len(self.layers)):
			if self.layers[l].layer_type=="contact": 
				return l

		return -1

	def get_btm_contact_layer(self):
		found=0
		for l in range(0,len(self.layers)):
			if self.layers[l].layer_type=="contact":
				if found==1:
					return l
				found=found+1

		return -1

	def del_dos_shape(self,shape_file):
		s=self.find_shape_by_file_name(shape_file)
		s.shape_dos="none"
		inp_update_token_value(s.file_name,"#shape_dos",s.shape_dos)

	def del_electrical_shape(self,shape_file):
		s=self.find_shape_by_file_name(shape_file)
		s.shape_dos="none"
		inp_update_token_value(s.file_name,"#shape_electrical",s.shape_dos)

	def add_new_dos_to_shape(self,shape_file):
		s=self.find_shape_by_file_name(shape_file)

		if s.shape_dos!="none":
			return s.shape_dos

		new_file=self.new_electrical_file("dos")

		s.shape_dos=new_file

		inp_update_token_value(s.file_name,"#shape_dos",s.shape_dos)

		new_dos_file=new_file+".inp"

		if inp().isfile(new_dos_file)==False:
			dos_path_generic=os.path.join(get_default_material_path(),"dos.inp")
			inp_copy_file(new_dos_file,dos_path_generic)

		return new_file

	def add_new_electrical_to_shape(self,shape_file):
		s=self.find_shape_by_file_name(shape_file)

		if s.shape_dos!="none":
			return s.shape_dos

		new_file=self.new_electrical_file("electrical")

		s.shape_dos=new_file

		inp_update_token_value(s.file_name,"#shape_electrical",s.shape_electrical)

		new_electrical_file=new_file+".inp"

		if inp().isfile(new_electrical_file)==False:
			electrical_path_generic=os.path.join(get_default_material_path(),"electrical.inp")
			inp_copy_file(new_electrical_file,electrical_path_generic)

		return new_file

	def new_electrical_file(self,prefix):
		files=self.get_all_electrical_files()

		for i in range(0,20):
			name=prefix+str(i)+".inp"

			if name not in files:
				return prefix+str(i)

	def get_all_sub_shapes(self,id):
		ret=[]
		for l in self.layers:
			if l.id==id:
				ret.append(l)

				for s in l.shapes:
					ret.append(s)
					#return ret


		#if layer==0:
		#	for c in self.contacts.contacts:
		#		if c.position=="top":
		#			ret.append(c)

		#ret.extend(self.layers[layer].shapes)
		return ret

	def ylen(self):
		tot=0
		for a in self.layers:
			tot=tot+a.dy

		return tot

	def ylen_active(self):
		tot=0
		for a in self.layers:
			if a.dos_file.startswith("dos")==True:
				tot=tot+a.dy

		return tot

	def get_opticaly_active_ylen(self):
		tot=0
		for a in epi.layers:
			if a.solve_optical_problem==True:
				tot=tot+a.dy

		return tot

	def get_layer_by_cordinate(self,y):
		tot=0
		for i in range(0,len(self.layers)):
			tot=tot+self.layers[i].dy
			#print(tot, y,i)
			if tot>=y or math.isclose(tot, y, rel_tol=1e-10):
				return i

		return -1

	def device_mask(self,x,y,z):
		#print(y)
		mesh=get_mesh()
		#1D case
		if mesh.x.points==1 and mesh.z.points==1:
			return True

		#check for contact
		layer=self.get_layer_by_cordinate(y)
		#print(y,layer)
		l=self.layers[layer]

		if l.layer_type=="contact" and layer==0:
			for c in self.contacts.contacts:
				if c.position=="top":
					if x>=c.x0 and x<=c.x0+c.dx:
						return True
			return False

		if l.layer_type=="contact" and layer==len(self.layers)-1:
			for c in self.contacts.contacts:
				if c.position=="bottom":
					if x>=c.x0 and x<=c.x0+c.dx:
						return True
			return False

		return True

	def reload_shapes(self):
		for a in self.layers:
			for s in a.shapes:
				#print(s.file_name)
				s.load(s.file_name)

	def get_shapes_between_x(self,x0,x1):
		shapes=[]
		for layer in self.layers:
			for s in layer.shapes:
				for pos in s.expand_xyz0(layer):
					if pos.x>x0 and pos.x<x1:
						shapes.append(s)
		return shapes

	def add_callback(self,fn):
		self.callbacks.append(fn)


	def load(self,path):
		self.layers=[]
		f=inp()

		y_pos=0.0
		if f.load(os.path.join(path,"epitaxy.inp"))!=False:
			number_of_layers=int(f.get_next_val())
			f.to_sections(start="#layer_type")

			for s in f.sections:
				a=epi_layer()


				a.layer_type=s.layer_type

				a.dos_file=s.layer_dos_file

				if a.dos_file.startswith("dos")==True:
					a.layer_type="active"

				a.pl_file=s.layer_pl_file

				a.file_name=s.layer_base_shape
				if a.file_name!="none":
					a.load(os.path.join(get_sim_path(),a.file_name))
				else:
					a.name=s.layer_name


				a.cal_rgb()

				#shape
				temp=s.layer_shape		#value
				if temp=="none":
					a.shapes=[]
				else:
					files=temp.split(",")
					for sh in files:
						my_shape=shape()
						my_shape.load(sh)
						a.shapes.append(my_shape)

				#lumo
				a.lumo_file=s.layer_lumo

				#lumo
				a.homo_file=s.layer_homo

				#interface_file
				a.interface_file=s.layer_interface


				a.solve_optical_problem=s.solve_optical_problem

				a.solve_thermal_problem=s.solve_thermal_problem

				a.start=y_pos

				y_pos=y_pos+a.dy

				a.end=y_pos
				self.layers.append(a)

			self.contacts.load()
			self.loaded=True
			#self.dump_tree()

			changed=False
			for l in self.layers:
				if l.interface_file=="none":
					l.interface_file=self.gen_new_electrical_file("interface")
					changed=True
			if changed==True:
				self.save()

	def find_layer_index_from_file_name(self,input_file):
		if input_file.endswith(".inp")==True:
			input_file=input_file[:-4]

		for i in range(0,len(self.layers)):
			l=self.layers[i]

			for s in l.shapes:
				if s.file_name==input_file:
					return i

			if l.dos_file==input_file:
				return i

			if l.homo_file==input_file:
				return i

			if l.lumo_file==input_file:
				return i

		return False

	def get_layer_end(self,l):

		pos=0.0
		for i in range(0, l+1):
			pos=pos+self.layers[i].dy

		return pos

	def get_layer_start(self,l):

		pos=0.0
		for i in range(0, l):
			pos=pos+self.layers[i].dy

		return pos

	def get_device_start(self):
		if get_mesh().y.circuit_model==True:
			return 0.0

		pos=0.0
		for i in range(0, len(self.layers)):
			if self.layers[i].dos_file.startswith("dos")==True:
				return pos

			pos=pos+self.layers[i].dy

		return None

	def get_next_dos_layer(self,layer):
		layer=layer+1
		for i in range(layer,len(self.layers)):
			if self.layers[i].dos_file.startswith("dos")==True:
				return i

		return False

epi=epitaxy()





def epitaxy_get_layer(i):
	global epi
	return epi.layers[i]

def get_epi():
	global epi
	return epi

def epitaxy_get_epi():
	global epi
	return epi.layers

def epitaxy_dos_file_to_layer_name(dos_file):
	global epi
	i=epi.find_layer_index_from_file_name(dos_file)

	if i!=False:
		return epi.layers[i].name

	return False

def epitaxy_get_dos_files():
	global epi
	dos_file=[]
	for i in range(0,len(epi.layers)):
		if epi.layers[i].dos_file.startswith("dos")==True:
			dos_file.append(epi.layers[i].dos_file)

	return dos_file


def epitaxy_get_layers():
	global epi
	return len(epi.layers)

def epitaxy_get_dy(i):
	global epi
	return epi.layers[i].dy

def epitaxy_get_pl_file(i):
	global epi
	return epi.layers[i].pl_file

def epitaxy_get_dos_file(i):
	global epi
	return epi.layers[i].dos_file


def epitaxy_get_name(i):
	global epi
	return epi.layers[i].name


