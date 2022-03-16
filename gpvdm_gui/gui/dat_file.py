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

## @package dat_file
#  Load and dump a dat file into a dat class
#

import os
import shutil
import re
import hashlib
import glob
import sys
from util_zip import zip_get_data_file
from inp import inp
from str2bool import str2bool
from triangle import triangle
from quiver import quiver
from util_text import is_number
from triangle import vec
from util_zip import zip_get_raw_data
import json
import numpy as np
from array import array
from dat_file_math import dat_file_math
from dat_file_trap_map import dat_file_trap_map
import codecs

#search first 40 lines for dims
def dat_file_load_info(output,lines):
	found_x=False
	found_y=False
	found_z=False

	if len(lines)>0:
		if lines[0].startswith("#gpvdm_csv"):
			line=lines[0][10:]

			j_data=json.loads(line)
			for j in j_data.keys():
				if j=="logscale_data":
					setattr(output, "logdata", str2bool(str(j_data[j])))
				elif j=="rgb":
					output.r=float(int(j_data[j][0:2], 16)/255)
					output.g=float(int(j_data[j][2:4], 16)/255)
					output.b=float(int(j_data[j][4:6], 16)/255)
				else:
					setattr(output, j, j_data[j])



		elif lines[0]=="#gpvdm":
			max_lines=len(lines)
			if max_lines>40:
				max_lines=40

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
							found_x=True
						if (command[0]=="#y"):
							output.y_len=int(command[1])
							found_y=True
						if (command[0]=="#z"):
							output.z_len=int(command[1])
							found_z=True
						if (command[0]=="#rgb"):
							output.decode_rgb("#rgb "+command[1])

		if found_x == True and found_y == True and found_z == True:
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


class dat_file(dat_file_math,dat_file_trap_map):

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
		self.srh_bands=None


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
		self.cols=""
		self.bin=False
		self.id="id"+codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()
		self.error=""

	def guess_dim_csv(self,data):
		if self.cols=="yd":
			self.x_len=1
			self.z_len=1
			if len(data)==0:
				self.y_len=0
			else:
				self.y_len=len(data)


	def import_data(self,file_name,x_col=0,y_col=1,skip_lines=0,known_col_sep=None):
		"""This is an import filter for xy data"""
		if self.have_i_loaded_this(file_name)==True:
			return True

		#print("1")
		f=inp()
		if f.load(os.path.join(file_name))==False:
			self.error="Problem loading file"
			return False

		if f.lines==False:
			self.error="Problem loading file"
			return False

		if len(f.lines)<skip_lines:
			self.error="Not enough lines"
			return False

		x_col=col_name_to_pos(f.lines,x_col,known_col_sep)
		y_col=col_name_to_pos(f.lines,y_col,known_col_sep)

		lines=f.lines[skip_lines:]

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
					number_ok=False
					try:
						float(s[x_col])
						float(s[y_col])
						number_ok=True
					except:
						pass

					if number_ok==True:
						if max(x_col,y_col)<l:
							duplicate=False
							if float(s[x_col]) in self.y_scale:
								duplicate=True

							if duplicate==False:
								self.y_scale.append(float(s[x_col]))
								self.data[0][0].append(float(s[y_col]))

		self.x_len=1
		self.y_len=len(self.data[0][0])
		self.z_len=1

		#print("3")
		return True

	def rgb_to_hex(self):
		if self.r==None:
			return None

		return format(int(self.r*255), '02x')+format(int(self.g*255), '02x')+format(int(self.b*255), '02x')

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

	def load_only_info(self,file_name):
		self.file_name=file_name
		data=zip_get_raw_data(file_name,bytes=2000)
		if data==False:
			return False
		self.header_end=len(data)
		for i in range(0,len(data)-1):
			if chr(data[i])=="}":
				if chr(data[i+1])=="*":
					self.header_end=i+1
					break

		lines=data[:self.header_end].decode('utf-8')
		lines=[str.strip() for str in lines.splitlines()]
		dat_file_load_info(self,lines)

		return True

	def load_xyzd_csv(self):
		y=0
		x=0
		z=0
		pos=0

		if self.bin==False:
			data = np.loadtxt(self.file_name)
		else:
			data=zip_get_raw_data(self.file_name)
			float_array = array('f',data[self.header_end+2:])
			data=float_array.tolist()

		if self.y_len==-1:		#If it's a file with an unknown length
			dim=0				#This sorts out the case where numpy gives [] instead of [[]]
			d=data
			if len(data)!=0:
				for i in range(0,4):
					#print(type(d),dim)
					if type(d)==np.float64:
						break
					d=d[0]
					dim=dim+1
				if dim==1:
					data=[data]
			self.guess_dim_csv(data)

		self.init_mem()
		if self.cols=="zxyEd":
			self.load_trap_map(data)
			return

		if self.bin==True:
			for z in range(0,self.z_len):
				self.z_scale[z]=data[pos]
				pos=pos+1

			for x in range(0,self.x_len):
				self.x_scale[x]=data[pos]
				pos=pos+1

			for y in range(0,self.y_len):
				self.y_scale[y]=data[pos]
				pos=pos+1

			for z in range(0,self.z_len):
				for x in range(0,self.x_len):
					for y in range(0,self.y_len):
						self.data[z][x][y]=data[pos]
						pos=pos+1
			return

		if self.cols=="yd":
			for y in range(0,self.y_len):
					line=data[pos]
					self.data[z][x][y]=float(line[1])
					self.y_scale[y]=float(line[0])
					pos=pos+1

		elif self.cols=="xzd":
			self.y_scale[0]=self.y_offset
			for z in range(0,self.z_len):
				for x in range(0,self.x_len):
					line=data[pos]
					self.data[z][x][y]=float(line[2])
					if z==0:
						self.x_scale[x]=float(line[0])
					pos=pos+1
				self.z_scale[z]=float(line[1])

		elif self.cols=="yzd":
			self.x_scale[0]=self.x_offset
			for z in range(0,self.z_len):
				for y in range(0,self.y_len):
					line=data[pos]
					self.data[z][x][y]=float(line[2])
					if z==0:
						self.y_scale[y]=float(line[0])
					pos=pos+1
				self.z_scale[z]=float(line[1])

		elif self.cols=="xyd":
			self.z_scale[0]=self.z_offset
			for x in range(0,self.x_len):
				for y in range(0,self.y_len):
					line=data[pos]
					self.data[z][x][y]=float(line[2])
					if x==0:
						self.y_scale[y]=float(line[1])
					pos=pos+1
				self.x_scale[x]=float(line[0])

	def load(self,file_name,guess=True):
		self.file_name=file_name

		if file_name==None:
			self.valid_data=False
			return False

		if self.have_i_loaded_this(file_name)==True:
			return True

		self.load_only_info(file_name)
		#if it has cols in it then it's a new data format csv file
		if len(self.cols)>0:
			self.load_xyzd_csv()
			return
		found,lines=zip_get_data_file(file_name)
		if found==False:
			return False


		self.x_scale=[]
		self.y_scale=[]
		self.z_scale=[]
		self.data=[]

		#print(file_name)
		if dat_file_load_info(self,lines)==False:
			if guess==True:
				self.x_len, self.y_len, self.z_len = guess_dim(lines)
			else:
				return False
			if self.x_len==False:
				self.valid_data=False
				print("No idea what to do with this file!",file_name)
				return False

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
		if len(self.z_scale)==1:
			self.z_scale[0]=self.z_offset

		data_started=False
		#print(self.file_name)
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
			#print(line,s,self.z_len,self.x_len,self.y_len,self.file_name)
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

					elif l==3:
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
							#print(s,"*",line)
							self.data[z][x][y]=float(s[1])
						a0=s[0]
						a1=0.0
						a2=0.0


					if line_found==True:
						if l==2:
							if x==0 and z==0:
								self.y_scale[y]=float(a0)+self.y_offset

						elif l==3:
							if x==0 and z==0:
								self.y_scale[y]=float(a1)+self.y_offset
								
							if z==0 and y==0:
								self.x_scale[x]=float(a0)

						elif l==4:
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

		if self.rgb_to_hex()!=None:
			lines.append("#rgb "+str(self.rgb_to_hex()))

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
