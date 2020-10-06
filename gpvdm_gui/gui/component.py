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

## @package component
#  An electrical component
#


import os
import math

class component():
	def __init__(self):
		self.name="resistor"
		self.x0=2
		self.y0=2
		self.x1=2
		self.y1=3
		self.z2=2
		self.z2=3

		self.dir="north"
		self.lines=[]

		self.R=10.0
		self.C=0.0
		self.L=0.0
		self.nid=1.0
		self.J0=0.0

	def __str__(self):
		return str(self.x0)+" "+str(self.y0)+" "+str(self.x1)+" "+str(self.y1)+" "+self.name

	def __eq__(self,a):
		if self.x0==a.x0:
			if self.y0==a.y0:
				if self.x1==a.x1:
					if self.y1==a.y1:
						return True

		if self.x0==a.x1:
			if self.y0==a.y1:
				if self.x1==a.x0:
					if self.y1==a.y0:
						return True
		return False

	def get_direction(self):
		if self.x0==self.x1:
			if self.y1>self.y0:
				return "up"
			else:
				return "down"

		if self.y0==self.y1:
			if self.x1>self.x0:
				return "right"
			else:
				return "left"
