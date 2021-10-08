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


## @package json_circuit
#  Store the cv domain json data
#

import json
from json_base import json_base


class json_component(json_base):

	def __init__(self):
		json_base.__init__(self,"component")
		self.var_list=[]
		self.var_list.append(["name","resistor"])
		self.var_list.append(["x0",2])
		self.var_list.append(["y0",2])
		self.var_list.append(["x1",3])
		self.var_list.append(["y1",3])
		self.var_list.append(["R",10.0])
		self.var_list.append(["C",0.0])
		self.var_list.append(["L",0.0])
		self.var_list.append(["nid",1.0])
		self.var_list.append(["I0",1e-12])
		self.var_list.append(["layer","none"])
		self.var_list.append(["Dphotoneff",1.0])
		self.var_list_build()

		self.dir="north"
		self.lines=[]

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

class json_circuit_diagram(json_base):

	def __init__(self):
		json_base.__init__(self,"circuit_diagram",segment_class=True)

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_component()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

class json_circuit(json_base):

	def __init__(self):
		json_base.__init__(self,"circuit")
		self.var_list=[]
		self.var_list.append(["enabled",False])
		self.var_list.append(["circuit_diagram",json_circuit_diagram()])
		self.var_list_build()


