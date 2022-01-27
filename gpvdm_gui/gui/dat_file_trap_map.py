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

class dat_file_trap_map():

	def load_trap_map(self,data):
		pos=0
		self.data=None
		self.Ec=[[[[0.0 for band in range(self.srh_bands)] for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]
		self.Ev=[[[[0.0 for band in range(self.srh_bands)] for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]
		self.nt=[[[[0.0 for band in range(self.srh_bands)] for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]
		self.pt=[[[[0.0 for band in range(self.srh_bands)] for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]

		self.Ec_f=[[[0.0 for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]
		self.Ev_f=[[[0.0 for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]
		self.nf=[[[0.0 for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]
		self.pf=[[[0.0 for y in range(self.y_len)] for x in range(self.x_len)] for z in range(self.z_len)]

		for z in range(0,self.z_len):
			self.z_scale[z]=data[pos]
			pos=pos+1

		for x in range(0,self.x_len):
			self.x_scale[x]=data[pos]
			pos=pos+1

		for y in range(0,self.y_len):
			self.y_scale[y]=data[pos]
			pos=pos+1

		self.data_min=1e6
		self.data_max=-1e6
		self.Ev_min=1e6
		self.Ec_max=-1e6

		for z in range(0,self.z_len):
			for x in range(0,self.x_len):
				for y in range(0,self.y_len):
					self.Ec_f[z][x][y]=data[pos]
					if self.Ec_max<data[pos]:
						self.Ec_max=data[pos]
					pos=pos+1

					self.nf[z][x][y]=data[pos]
					if self.data_min>data[pos]:
						self.data_min=data[pos]
					elif self.data_max<data[pos]:
						self.data_max=data[pos]
					pos=pos+1

					for band in range(0,self.srh_bands):
						self.Ec[z][x][y][band]=data[pos]
						if self.Ec_max<data[pos]:
							self.Ec_max=data[pos]
						pos=pos+1

						self.nt[z][x][y][band]=data[pos]
						if self.data_min>data[pos]:
							self.data_min=data[pos]
						elif self.data_max<data[pos]:
							self.data_max=data[pos]
						pos=pos+1

					self.Ev_f[z][x][y]=data[pos]
					if self.Ev_min>data[pos]:
						self.Ev_min=data[pos]
					pos=pos+1

					self.pf[z][x][y]=data[pos]
					if self.data_min>data[pos]:
						self.data_min=data[pos]
					elif self.data_max<data[pos]:
						self.data_max=data[pos]

					pos=pos+1

					for band in range(0,self.srh_bands):
						self.Ev[z][x][y][band]=data[pos]
						if self.Ev_min>data[pos]:
							self.Ev_min=data[pos]
						pos=pos+1

						self.pt[z][x][y][band]=data[pos]
						if self.data_min>data[pos]:
							self.data_min=data[pos]
						elif self.data_max<data[pos]:
							self.data_max=data[pos]

						pos=pos+1


