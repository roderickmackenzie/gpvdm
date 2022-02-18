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

## @package dat_file_math
#  Do math on the dat file class.
#

import os
import re
import copy

class dat_file_math():

	def pow(self,val):
		a=dat_file()
		a.copy(self)

		for z in range(0,len(self.z_scale)):
			for x in range(0,len(self.x_scale)):
				for y in range(0,len(self.y_scale)):
					a.data[z][x][y]=pow(self.data[z][x][y],val)
		return a		

	def intergrate(self):
		sum=0.0
		for y in range(0,len(self.y_scale)-1):
			dy=self.y_scale[y+1]-self.y_scale[y]
			sum=sum+self.data[0][0][y]*dy

		return sum

	def set_neg_to_zero(self):
		for y in range(0,len(self.y_scale)):
			if self.data[0][0][y]<0.0:
				self.data[0][0][y]=0.0

	def set_neg_to_last(self):
		last=0.0
		for y in range(0,len(self.y_scale)):
			if self.data[0][0][y]<0.0:
				self.data[0][0][y]=last
			else:
				last=self.data[0][0][y]

	def __sub__(self,val):
		a = copy.deepcopy(self)
		a.name="hello"
		if type(val)==float:
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=self.data[z][x][y]-val
		elif type(val)==type(self):
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=self.data[z][x][y]-val.data[z][x][y]
						#print("a",self.data[z][x][y],val.data[z][x][y],a.data[z][x][y])
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
		a = copy.deepcopy(self)
		if type(val)==float:
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=valself.data[z][x][y]
		elif type(val)==type(self):
			for z in range(0,len(self.z_scale)):
				for x in range(0,len(self.x_scale)):
					for y in range(0,len(self.y_scale)):
						a.data[z][x][y]=val.data[z][x][y]-self.data[z][x][y]
		
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


	def max_min(self,cur_min=None,cur_max=None,only_use_data=False):
		my_max=False
		my_min=False
		
		if only_use_data==False:
			if self.data_max!=None and self.data_min!=None:
				return [self.data_max,self.data_min]

		if self.x_len>0 and self.y_len>0 and self.z_len>0:
			my_max=self.data[0][0][0]
			my_min=self.data[0][0][0]

			for z in range(0,self.z_len):
				for x in range(0,self.x_len):
					for y in range(0,self.y_len):
						
						if self.data[z][x][y]>my_max:
							my_max=self.data[z][x][y]

						if self.data[z][x][y]<my_min:
							my_min=self.data[z][x][y]
		if cur_min!=None:
			if cur_min<my_min:
				my_min=cur_min

		if cur_max!=None:
			if cur_max>my_max:
				my_max=cur_max

		return [my_max,my_min]


	def dat_file_sub(self,one):
		if (self.x_len==one.x_len) and (self.y_len==one.y_len) and (self.z_len==one.z_len):
			for z in range(0,self.z_len):
				for x in range(0,self.x_len):
					for y in range(0,self.y_len):
						self.data[z][x][y]=self.data[z][x][y]-one.data[z][x][y]

	def dat_file_sub_float(self,val):
			for z in range(0,self.z_len):
				for x in range(0,self.x_len):
					for y in range(0,self.y_len):
						self.data[z][x][y]=self.data[z][x][y]-val
						
	def dat_file_mul(self,val):
			for z in range(0,self.z_len):
				for x in range(0,self.x_len):
					for y in range(0,self.y_len):
						self.data[z][x][y]*=val

	def abs(self):
			for z in range(0,self.z_len):
				for x in range(0,self.x_len):
					for y in range(0,self.y_len):
						self.data[z][x][y]=abs(self.data[z][x][y])


