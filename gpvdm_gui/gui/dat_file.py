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

## @package dat_file
#  Load and dump a dat file into a dat class
#

import os
import shutil
import re
import hashlib
import glob
from util_zip import zip_get_data_file
from inp import inp_load_file
from str2bool import str2bool
from triangle import triangle
from quiver import quiver
from json_circuit import json_component
from gl_base_object import gl_base_object
from util_text import is_number

#search first 40 lines for dims
def dat_file_load_info(output,lines):
	#print(lines[0])
	if len(lines)>1:
		if lines[0]=="#gobj":
			output.type="gobj"
			return True

		if lines[0]=="#gpvdm":
			max_lines=len(lines)
			if max_lines>40:
				max_lines=40

			found_xyz=False
			for i in range(0, max_lines):
				if (len(lines[i])>0):
					if (lines[i][0]!="#"):
						break
					else:
						command=lines[i].split(" ",1)
						if len(command)<2:
							command.append("")
						if (command[0]=="#x_mul"):
							output.x_mul=float(command[1])
						if (command[0]=="#y_mul"):
							output.y_mul=float(command[1])
						if (command[0]=="#z_mul"):
							output.z_mul=float(command[1])
						if (command[0]=="#x_offset"):
							output.x_offset=float(command[1])
						if (command[0]=="#y_offset"):
							output.y_offset=float(command[1])
						if (command[0]=="#z_offset"):
							output.z_offset=float(command[1])
						if (command[0]=="#data_mul"):
							output.data_mul=float(command[1])
						if (command[0]=="#x_label"):
							output.x_label=command[1]
						if (command[0]=="#y_label"):
							output.y_label=command[1]
						if (command[0]=="#z_label"):
							output.z_label=command[1]
						if (command[0]=="#data_label"):
							output.data_label=command[1]
						if (command[0]=="#x_units"):
							output.x_units=command[1]
						if (command[0]=="#y_units"):
							output.y_units=command[1]
						if (command[0]=="#z_units"):
							output.z_units=command[1]
						if (command[0]=="#data_units"):
							output.data_units=command[1]
						if (command[0]=="#logscale_x"):
							output.logx=str2bool(command[1])
						if (command[0]=="#logscale_y"):
							output.logy=str2bool(command[1])
						if (command[0]=="#logscale_z"):
							output.logz=str2bool(command[1])
						if (command[0]=="#logscale_data"):
							output.logdata=str2bool(command[1])
						if (command[0]=="#type"):
							output.type=command[1]
						if (command[0]=="#title"):
							output.title=command[1]
						if (command[0]=="#section_one"):
							output.section_one=command[1]
						if (command[0]=="#section_two"):
							output.section_two=command[1]
						if (command[0]=="#time"):
							output.time=float(command[1])
						if (command[0]=="#Vexternal"):
							output.Vexternal=float(command[1])
						if (command[0]=="#data_max"):
							output.data_max=float(command[1])
						if (command[0]=="#data_min"):
							output.data_min=float(command[1])
						if (command[0]=="#x"):
							output.x_len=int(command[1])
							found_xyz=True
						if (command[0]=="#y"):
							output.y_len=int(command[1])
							found_xyz=True
						if (command[0]=="#z"):
							output.z_len=int(command[1])
							found_xyz=True
						if (command[0]=="#rgb"):
							output.decode_rgb("#rgb "+command[1])

			if found_xyz==True and output.x_len != -1 and output.y_len != -1 and output.z_len != -1:
				return True
			else:
				return False
				
	return False

def guess_dim(lines):
	x=0
	y=0
	z=0
	data_started=False
	for i in range(0, len(lines)):
		temp=lines[i]
		temp=re.sub(' +',' ',temp)
		temp=re.sub("\t"," ",temp)
		temp=re.sub("\r","",temp)

		if len(temp)>0:
				s=temp.split()
				l=len(s)

				if l>0:
					if len(s[l-1])>0:
						if s[l-1][0]=="#":
							l=l-1
				if l==1:
					if is_number(s[0])==True:
						print("I can't do this file type yet",s,l)
						return False,False,False
				if l==2:
					if is_number(s[0])==True and is_number(s[1])==True:
						y=y+1
				if l==3:
					if is_number(s[0])==True and is_number(s[1])==True and is_number(s[2])==True:
						print("I can't do this file type yet",s,l)
						return False,False,False
	return 1,y,1

def col_name_to_pos(lines,col,known_col_sep):
	if known_col_sep==None:
		return col

	if type(col)==float:
		return col

	for i in range(0, len(lines)):
		s,label=decode_line(lines[i],known_col_sep=known_col_sep)
		if col in s:
			return s.index(col)

	return False




def decode_line(line,known_col_sep=None):
	label=False
	line=re.sub(' +',' ',line)

	if known_col_sep!=None:
		s=line.split(known_col_sep)
		return s,False

	line=re.sub('\t',' ',line)

	#check for labels at the end of the line
	if len(line)>0:
		if line[0]!="#":
			if line.count("#")>0:
				label=line.split("#")[1]
				line=line.split("#")[0]
				labels=True

	line=line.replace(', ', ' ')	#remove comman in csv files
	line=line.replace(',', '.')		#Remove European commas
	s=line.split()
		
	return s,label

			

def read_data_2d(x_scale,y_scale,z,file_name):
	if file_name==None:
		return False
	
	found,lines=zip_get_data_file(file_name)
	if found==True:
		x_max=0
		y_max=0
		y_pos=0
		z_store=[]
		for i in range(0, len(lines)):
			if len(lines[i])>0:
				if lines[i][0]!="#" and lines[i]!="\n":
					temp=lines[i]
					temp=re.sub(' +',' ',temp)
					temp=re.sub('\t',' ',temp)
					temp=temp.rstrip()
					sline=temp.split(" ")

					if len(sline)==3:
						if x_max==0:
							y_scale.append(float(lines[i].split(" ")[1]))
						if y_pos==0:
							x_scale.append(float(lines[i].split(" ")[0]))

						z_store.append(float(lines[i].split(" ")[2]))
					y_pos=y_pos+1

					if x_max==0:
						y_max=y_max+1

			if lines[i]=="":
				x_max=x_max+1
				y_pos=0

		if  lines[len(lines)-1]!="\n":
			x_max=x_max+1

		x_max=len(x_scale)
		y_max=len(y_scale)

		pos=0
		for x in range(0, x_max):
			z.append([])
			for y in range(0, y_max):
				z[x].append(z_store[pos])
				pos=pos+1
		return True
	else:
		return False

def dat_file_print(dat):
	print("valid_data",dat.valid_data)
	print("grid",dat.grid)
	print("show_pointer",dat.show_pointer)
	print("logy",dat.logy)
	print("logx",dat.logx)
	print("logz",dat.logz)
	print("logdata",dat.logdata)
	print("label_data",dat.label_data)
	print("invert_y",dat.invert_y)
	print("normalize",dat.normalize)
	print("norm_to_peak_of_all_data",dat.norm_to_peak_of_all_data)
	print("subtract_first_point",dat.subtract_first_point)
	print("add_min",dat.add_min)
	print("legend_pos",dat.legend_pos)
	print("ymax",dat.ymax)
	print("ymin",dat.ymin)
	print("x_label",dat.x_label)
	print("y_label",dat.y_label)
	print("z_label",dat.z_label)
	print("data_label",dat.data_label)
	print("x_units",dat.x_units)
	print("y_units",dat.y_units)
	print("z_units",dat.z_units)
	print("rgb",dat.rgb)
	print("data_units",dat.data_units)
	print("x_mul",dat.x_mul)
	print("y_mul",dat.y_mul)
	print("z_mul",dat.z_mul)
	print("data_mul",dat.data_mul)
	print("key_units",dat.key_units)
	print("file0",dat.file0)
	print("tag0",dat.tag0)
	print("file1",dat.file1)
	print("tag1",dat.tag1)
	print("file2",dat.file2)
	print("tag2",dat.tag2)
	print("example_file0",dat.example_file0)
	print("example_file1",dat.example_file1)
	print("example_file2",dat.example_file2)
	print("time",dat.time)
	print("Vexternal",dat.Vexternal)
	print("file_name",dat.file_name)
	print("other_file",dat.other_file)
	print("title",dat.title)
	print("type",dat.type)
	print("section_one",dat.section_one)
	print("section_two",dat.section_two)

	print("x_start",dat.x_start)
	print("x_stop",dat.x_stop)
	print("x_points",dat.x_points)
	print("y_start",dat.y_start)
	print("y_stop",dat.y_stop)
	print("y_points",dat.y_points)
	print("x_len",dat.x_len)
	print("y_len",dat.y_len)
	print("z_len",dat.z_len)
	
	print("x_scale",dat.x_scale)
	print("y_scale",dat.y_scale)
	print("z_scale",dat.z_scale)
	print("data",dat.data)
	print("labels",dat.labels)



class dat_file():
	def __init__(self):
		self.valid_data=False
		self.grid=False
		self.show_pointer=False
		self.logy=False
		self.logx=False
		self.logz=False
		self.logdata=False
		self.label_data=False
		self.invert_y=False
		self.normalize=False
		self.norm_to_peak_of_all_data=False
		self.subtract_first_point=False
		self.add_min=False
		self.legend_pos="lower right"
		self.ymax=-1
		self.ymin=-1
		self.xmax=-1
		self.xmin=-1
		self.zmax=-1
		self.zmin=-1
		self.x_label=""
		self.y_label=""
		self.z_label=""
		self.data_label=""
		self.x_units=""
		self.y_units=""
		self.z_units=""
		self.data_units=""
		self.plot_type=""		#wireframe/heat etc...
		self.plotted=False
		self.r=None
		self.g=None
		self.b=None


		self.x_mul=1.0
		self.y_mul=1.0
		self.z_mul=1.0

		self.x_offset=0.0
		self.y_offset=0.0
		self.z_offset=0.0

		self.data_mul=1.0
		self.key_units=""
		self.key_text=""
		self.file0=""
		self.tag0=""
		self.file1=""
		self.tag1=""
		self.file2=""
		self.tag2=""
		self.example_file0=""
		self.example_file1=""
		self.example_file2=""
		self.time=0.0
		self.Vexternal=0.0
		self.file_name=""
		self.other_file=""
		self.title=""
		self.type="xy"
		self.section_one=""
		self.section_two=""

		self.x_start=0
		self.x_stop=1
		self.x_points=25
		self.y_start=0
		self.y_stop=1
		self.y_points=25
		self.x_len=0
		self.y_len=0
		self.z_len=0
		
		self.data_max=None
		self.data_min=None

		self.x_scale=[]
		self.y_scale=[]
		self.z_scale=[]
		self.data=[]
		self.labels=[]
		self.file_age=0
		self.new_read=True
		self.file_name=None

	def import_data(self,file_name,x_col=0,y_col=1,skip_lines=0,known_col_sep=None):
		"""This is an import filter for xy data"""
		if self.have_i_loaded_this(file_name)==True:
			return True

		lines=[]
		print("1")
		lines=inp_load_file(file_name)
		if lines==False:
			return False

		if len(lines)<skip_lines:
			return False

		x_col=col_name_to_pos(lines,x_col,known_col_sep)
		y_col=col_name_to_pos(lines,y_col,known_col_sep)

		lines=lines[skip_lines:]

		print("2")

		self.x_scale=[]
		self.y_scale=[]
		self.z_scale=[]
		self.data=[]
		data_started=False
		self.data=[[[0.0 for k in range(0)] for j in range(1)] for i in range(1)]

		for i in range(0, len(lines)):
			s,label=decode_line(lines[i],known_col_sep=known_col_sep)
			#print(s)
			l=len(s)
			if l>0:

				if data_started==False:
					if is_number(s[0])==True:
						data_started=True

				if s[0]=="#end":
					break

				if data_started==True:
					if max(x_col,y_col)<l:
						duplicate=False
						for c in range(0,len(self.y_scale)):
							if self.y_scale[c]==float(s[x_col]):
								duplicate=True
								break

						if duplicate==False:
							self.y_scale.append(float(s[x_col]))
							self.data[0][0].append(float(s[y_col]))

		self.x_len=1
		self.y_len=len(self.data[0][0])
		self.z_len=1

		print("3")
		return True

	def rgb(self):
		if self.r==None:
			return None

		return format(int(self.r*255), '02x')+format(int(self.g*255), '02x')+format(int(self.b*255), '02x')

	def pow(self,val):
		a=dat_file()
		a.copy(self)

		for z in range(0,len(self.z_scale)):
			for x in range(0,len(self.x_scale)):
				for y in range(0,len(self.y_scale)):
					a.data[z][x][y]=pow(self.data[z][x][y],val)
		return a		

	def __sub__(self,val):
		a=dat_file()
		a.copy(self)

		for z in range(0,len(self.z_scale)):
			for x in range(0,len(self.x_scale)):
				for y in range(0,len(self.y_scale)):
					a.data[z][x][y]=self.data[z][x][y]-val
		return a

	def __add__(self,val):
		a=dat_file()
		a.copy(self)
		if type(val)==float:
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=self.data[z][x][y]+val
		else:
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=self.data[z][x][y]+val.data[z][x][y]

		return a

	def __truediv__(self,in_data):
		a=dat_file()
		a.copy(self)

		for z in range(0,len(self.z_scale)):
			for x in range(0,len(self.x_scale)):
				for y in range(0,len(self.y_scale)):
					a.data[z][x][y]=self.data[z][x][y]/in_data.data[z][x][y]
		return a

	def __rsub__(self,val):
		a=dat_file()
		a.copy(self)

		for z in range(0,len(self.z_scale)):
			for x in range(0,len(self.x_scale)):
				for y in range(0,len(self.y_scale)):
					a.data[z][x][y]=val-self.data[z][x][y]
		return a

	def set_float(self,val):
		for z in range(0,len(self.z_scale)):
			for x in range(0,len(self.x_scale)):
				for y in range(0,len(self.y_scale)):
					self.data[z][x][y]=val

	def chop_y(self,y0,y1):
		if y0==0 and y1==0:
			return

		self.y_scale=self.y_scale[y0:y1]
		self.y_len=len(self.y_scale)

		for z in range(0,len(self.z_scale)):
			for x in range(0,len(self.x_scale)):
				self.data[z][x]=self.data[z][x][y0:y1]
				#for y in range(0,len(self.y_scale)):
				#	self.data[z][x][y]=val

	def __mul__(self,in_data):
		a=dat_file()
		a.copy(self)

		if type(in_data)==float:
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=in_data*self.data[z][x][y]
		else:
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=in_data.data[z][x][y]*self.data[z][x][y]

		return a

	def __rmul__(self, in_data):
		return self.__mul__(in_data)

	def copy(self,in_data):
		self.x_len=in_data.x_len
		self.y_len=in_data.y_len
		self.z_len=in_data.z_len

		self.init_mem()

		for i in range(0,len(self.x_scale)):
			self.x_scale[i]=in_data.x_scale[i]

		for i in range(0,len(self.y_scale)):
			self.y_scale[i]=in_data.y_scale[i]

		for i in range(0,len(self.z_scale)):
			self.z_scale[i]=in_data.z_scale[i]

		self.y_mul=in_data.y_mul
		self.y_units=in_data.y_units
		self.data_mul=in_data.data_mul
		self.data_units=in_data.data_units

	def init_mem(self):
		self.data=[[[0.0 for k in range(self.y_len)] for j in range(self.x_len)] for i in range(self.z_len)]
				
		self.x_scale= [0.0]*self.x_len
		self.y_scale= [0.0]*self.y_len
		self.z_scale= [0.0]*self.z_len
		self.valid_data=True

	def decode_gobj_lines(self,lines):
		nobj=0
		for line in lines:
			o=gl_base_object()
			l=line.split(";;")

			#print(l)
			if len(l)>1:
				o.type=l[0]
				o.id=["obj"+str(nobj)]
				o.xyz.x=float(l[1])
				o.xyz.y=float(l[2])
				o.xyz.z=float(l[3])

				o.dxyz.x=float(l[4])
				o.dxyz.y=float(l[5])
				o.dxyz.z=float(l[6])

				o.r=float(l[7])
				o.g=float(l[8])
				o.b=float(l[9])

				o.alpha=float(l[10])

				o.selected=str2bool(l[11])
				o.selectable=str2bool(l[12])
				o.moveable=str2bool(l[13])
				o.allow_cut_view=str2bool(l[14])

				o.text=l[15]
				o.origonal_object=str2bool(l[16])

				self.data.append(o)
				nobj=nobj+1

	def decode_circuit_lines(self,lines):
		build=[]
		self.data=[]
		for line in lines:
			s,label=decode_line(line)
			l=len(s)
			if l>0:
				if s[0].startswith("#")==False:
					c=component()
					c.z0=float(s[0])
					c.x0=float(s[1])
					c.y0=float(s[2])
					c.z1=float(s[3])
					c.x1=float(s[4])
					c.y1=float(s[5])
					c.name=s[6]

					self.data.append(c)

		return True

	def decode_poly_lines(self,lines):
		build=[]
		self.data=[]
		for line in lines:
			s,label=decode_line(line)
			l=len(s)
			if l>0:
				if s[0].startswith("#")==False:
					s=list(map(float, s))
					build.append(s)

			if len(build)!=0 and len(s)==0:

				#This is to account for the forth xyz vector required by gnuplot and to remove it if it exists.
				if len(build)==4:
					if build[len(build)-1]==build[0]:
						build=build[:-1]

				if len(build)==3 and len(build[0])==3:
					t=triangle()
					t.xyz0.z=build[0][0]
					t.xyz0.x=build[0][1]
					t.xyz0.y=build[0][2]

					t.xyz1.z=build[1][0]
					t.xyz1.x=build[1][1]
					t.xyz1.y=build[1][2]

					t.xyz2.z=build[2][0]
					t.xyz2.x=build[2][1]
					t.xyz2.y=build[2][2]
					t.points=3
					self.data.append(t)

				elif len(build)==2 and len(build[0])==3:
					t=triangle()
					t.xyz0.z=build[0][0]
					t.xyz0.x=build[0][1]
					t.xyz0.y=build[0][2]

					t.xyz1.z=build[1][0]
					t.xyz1.x=build[1][1]
					t.xyz1.y=build[1][2]

					self.data.append(t)
					t.points=2
				else:
					self.data.append(build)
				build=[]

		self.valid_data=True
		return True

	def cal_min_max(self):
		self.ymax=0.0
		self.ymin=0.0
		self.xmax=0.0
		self.xmin=0.0
		self.zmax=0.0
		self.zmin=0.0

		if self.type=="quiver":
			for d in self.data:
				if d.x>self.xmax:
					self.xmax=d.x

				if d.y>self.ymax:
					self.ymax=d.y

				if d.z>self.zmax:
					self.zmax=d.z

	def decode_quiver_lines(self,lines):
		build=[]
		self.data=[]

		for line in lines:
			s,label=decode_line(line)
			l=len(s)
			if l>0:
				if s[0].startswith("#")==False:
					s=list(map(float, s))
					q=quiver()
					q.x=s[0]
					q.y=s[1]
					q.z=s[2]
					q.dx=s[3]
					q.dy=s[4]
					q.dz=s[5]
					q.mag=s[6]


					self.data.append(q)

		self.cal_min_max()
		self.valid_data=True
		return True

	def have_i_loaded_this(self,file_name):
		if os.path.isfile(file_name)==True:
			age=os.path.getmtime(file_name)

			if age==self.file_age:
				self.new_read=False
				return True
			else:
				self.new_read=True
				self.file_age=age

		return False

	def load(self,file_name,guess=True):
		self.file_name=file_name

		if file_name==None:
			self.valid_data=False
			return False

		if self.have_i_loaded_this(file_name)==True:
			return True

		found,lines=zip_get_data_file(file_name)
		if found==False:
			return False


		self.x_scale=[]
		self.y_scale=[]
		self.z_scale=[]
		self.data=[]

		if dat_file_load_info(self,lines)==False:
			#print("no dims")
			if guess==True:
				self.x_len, self.y_len, self.z_len = guess_dim(lines)
			else:
				return False
			if self.x_len==False:
				self.valid_data=False
				print("No idea what to do with this file!",file_name)
				return False

		if self.type=="gobj":
			return self.decode_gobj_lines(lines)

		if self.type=="poly":
			return self.decode_poly_lines(lines)

		if self.type=="circuit":
			return self.decode_circuit_lines(lines)

		if self.type=="quiver":
			return self.decode_quiver_lines(lines)

		return self.decode_zxy_lines(lines)

	def decode_zxy_lines(self,lines):
		self.init_mem()

		self.labels=[]

		data_started=False

		x=0
		y=0
		z=0
		dim=0
		label=""
		labels=False
		#print(file_name)

		for line in lines:
			s,label=decode_line(line)
			l=len(s)

			if l>0:
								

				if data_started==False:
					if is_number(s[0])==True:
						data_started=True

				if s[0]=="#end":
					break

				if data_started==True:

					if line.count("nan")>0:
						#print("Warning nan found in data file",file_name)
						return False

					line_found=False
					if l==4:
						line_found=True
						a0=s[0]
						a1=s[1]
						a2=s[2]
						self.data[z][x][y]=float(s[3])

					if l==3:
						line_found=True
						if self.type=="rgb":
							r=float(int(s[2][0:2], 16)/255)
							g=float(int(s[2][2:4], 16)/255)
							b=float(int(s[2][4:6], 16)/255)
							self.data[z][x][y]=[r,g,b]
						else:
							self.data[z][x][y]=float(s[2])
						a0=s[0]
						a1=s[1]
						a2=0.0

					elif l==2:
						#print(s,self.z_len,self.x_len,self.y_len)
						line_found=True
						if self.type=="rgb":
							r=float(int(s[1][0:2], 16)/255)
							g=float(int(s[1][2:4], 16)/255)
							b=float(int(s[1][4:6], 16)/255)
							self.data[z][x][y]=[r,g,b]
						else:
							self.data[z][x][y]=float(s[1])
						a0=s[0]
						a1=0.0
						a2=0.0

	#				else:
	#					print("skip")

					if line_found==True:
						if l==2:
							if x==0 and z==0:
								self.y_scale[y]=float(a0)+self.y_offset

						if l==3:
							if x==0 and z==0:
								self.y_scale[y]=float(a1)+self.y_offset
								
							if z==0 and y==0:
								self.x_scale[x]=float(a0)

						if l==4:
							if x==0 and y==0:
								self.z_scale[z]=float(a0)

							if z==0 and y==0:
								self.x_scale[x]=float(a1)

							if x==0 and z==0:
								self.y_scale[y]=float(a2)+self.y_offset
						#if z==y:
						#	self.z_scale[y]=float(a0)
						if label!=False:
							self.labels.append(label)
						y=y+1
						if y==self.y_len:
							y=0
							x=x+1
						if x==self.x_len:
							x=0
							z=z+1

				if s[0]=="#data":
					data_started=True

		if data_started==False:
			return False

		#print(self.data)

		return True

	def save_as_csv(self,file_name):
		if file_name.endswith(".csv")==False:
			file_name=file_name+".csv"

		lines=[]

		lines.append(self.y_label+","+self.data_label)

		for i in range(0,self.y_len):
			y_text=str('{:.8e}'.format(float(self.y_scale[i])))
			data_text=str('{:.8e}'.format(float(self.data[0][0][i])))
			lines.append(y_text+","+data_text)

		dump=""
		for item in lines:
			dump=dump+item+"\n"
			
		f=open(file_name, mode='w')
		lines = f.write(dump)
		f.close()

	def save_as_txt(self,file_name):
		if file_name.endswith(".txt")==False:
			file_name=file_name+".txt"

		lines=[]

		for i in range(0,self.y_len):
			y_text=str('{:.8e}'.format(float(self.y_scale[i])))
			data_text=str('{:.8e}'.format(float(self.data[0][0][i])))
			lines.append(y_text+" "+data_text)

		dump=""
		for item in lines:
			dump=dump+item+"\n"
			
		f=open(file_name, mode='w')
		lines = f.write(dump)
		f.close()

	def save(self,file_name):
		a = open(file_name, "w")
		a.write("\n".join(self.gen_output_data()))
		a.close()

	def decode_rgb(self,line):
		if line.startswith("#rgb"):
			#print(line)
			line=line.split()
			if len(line)==2:
				rgb=line[1]
				self.r=float(int(rgb[0:2], 16)/255)
				self.g=float(int(rgb[2:4], 16)/255)
				self.b=float(int(rgb[4:6], 16)/255)

	def __str__(self):
		return "\n".join(self.gen_output_data())

	def gen_output_data(self):
		lines=[]
		lines.append("#gpvdm")
		lines.append("#title "+str(self.title))
		lines.append("#type "+str(self.type))
		lines.append("#x_mul "+str(self.x_mul))
		lines.append("#y_mul "+str(self.y_mul))
		lines.append("#z_mul "+str(self.z_mul))
		lines.append("#data_mul "+str(self.data_mul))
		if self.x_label!="":
			lines.append("#x_label "+str(self.x_label))

		if self.y_label!="":
			lines.append("#y_label "+str(self.y_label))

		if self.z_label!="":
			lines.append("#z_label "+str(self.z_label))

		if self.data_label!="":
			lines.append("#data_label "+str(self.data_label))

		if self.x_units!="":
			lines.append("#x_units "+str(self.x_units))

		if self.y_units!="":
			lines.append("#y_units "+str(self.y_units))

		if self.z_units!="":
			lines.append("#y_units "+str(self.z_units))

		if self.rgb()!=None:
			lines.append("#rgb "+str(self.rgb()))

		if self.data_units!="":
			lines.append("#data_units "+str(self.data_units))

		if self.logy!=False:
			lines.append("#logscale_y "+str(self.logy))

		if self.logx!=False:
			lines.append("#logscale_x "+str(self.logx))

		if self.logz!=False:
			lines.append("#logscale_z "+str(self.logz))

		if self.logdata!=False:
			lines.append("#logscale_data "+str(self.logdata))

		lines.append("#time "+str(self.time))
		lines.append("#Vexternal "+str(self.Vexternal))
		lines.append("#x "+str(self.x_len))
		lines.append("#y "+str(self.y_len))
		lines.append("#z "+str(self.z_len))

		lines.append("#begin")

		if self.type=="poly":
			for d in self.data:
				lines.append(str(d.xyz0.z)+" "+str(d.xyz0.x)+" "+str(d.xyz0.y))
				lines.append(str(d.xyz1.z)+" "+str(d.xyz1.x)+" "+str(d.xyz1.y))
				lines.append(str(d.xyz2.z)+" "+str(d.xyz2.x)+" "+str(d.xyz2.y))
				lines.append(str(d.xyz0.z)+" "+str(d.xyz0.x)+" "+str(d.xyz0.y))

				lines.append("")
		else:
			for i in range(0,self.y_len):
				y_text=str('{:.8e}'.format(float(self.y_scale[i])))
				data_text=str('{:.8e}'.format(float(self.data[0][0][i])))
				lines.append(y_text+" "+data_text)

		lines.append("#end")

		return lines
