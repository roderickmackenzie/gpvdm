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

## @package contacts_io
#  Back end to deal with contacts.
#

import os

from inp import inp
from inp import inp_save

from str2bool import str2bool
from cal_path import get_sim_path
from shape import shape

from gui_enable import gui_get
from json_base import json_base

class contact(shape):
	def __init__(self):
		super().__init__()
		self.var_list.append(["position","top"])
		self.var_list.append(["applied_voltage_type","constant"])
		self.var_list.append(["applied_voltage",-2.0])
		self.var_list.append(["contact_resistance_sq",0.0])
		self.var_list.append(["shunt_resistance_sq",0.0])
		self.var_list.append(["np",1e20])
		self.var_list.append(["charge_type","electron"])
		self.var_list.append(["physical_model","ohmic"])
		self.var_list.append(["ve0",1e5])
		self.var_list.append(["vh0",1e5])
		self.var_list_build()
		self.latex_banned=["all"]
		self.latex_allowed=["np"]

class contacts_io(json_base):

	def __init__(self):
		self.this_is_the_contact_class=None
		json_base.__init__(self,"contacts",segment_class=True)

	def em(self):
		self.load()
		self.changed.emit()

	def load_json(self,json):
		self.segments=[]
		if "ncontacts" in json:
			ncontacts=json['ncontacts']
			for n in range(0,ncontacts):
				contact_name="contact"+str(n)
				c=contact()
				c.decode_from_json(json[contact_name])

				self.segments.append(c)

		if "segments" in json:
			ncontacts=json['segments']
			for n in range(0,ncontacts):
				contact_name="segment"+str(n)
				c=contact()
				c.decode_from_json(json[contact_name])

				self.segments.append(c)

	def print():
		for s in self.segments:
			print(s.x0, s.dx,s.depth,s.contact_applied_voltage,s.contact_applied_voltage_type)

	def clear():
		self.segments=[]

	def remove_by_id(self,ids):
		if type(ids)==str:
			ids=[ids]

		for c in self.segments[:]:
			if c.id in ids:
				self.segments.remove(c)

	def get_layers_with_contacts(self):
		layers=[]
		for c in self.segments:
			if c.position not in layers:
				layers.append(c.position)

		return layers

	def gen_json(self):
		out=[]
		n=0
		out.append("\"contacts\": {")
		out.append("\"segments\":"+str(len(self.segments))+",")
		for c in self.segments:
			c.include_name=False
			gen=c.gen_json()
			gen[0]="\"segment"+str(n)+"\": {"
			gen[len(gen)-1]="},"
			out.extend(gen)
			n=n+1
		out[len(out)-1]="}"
		out.append("}")
		return out

	def insert(self,pos):
		s=contact()
		s.position="top"
		s._applied_voltage="ground"
		s.np=1e25
		s.physical_model="ohmic"
		s.contact_resistance_sq="0.0"
		s.shunt_resistance_sq="1e7"
		s.ve0=1e7
		s.vh0=1e7
		s.charge_type="electron"
		s.name="new_contact"
		s.type="box"
		self.segments.insert(pos,s)
		return self.segments[pos]

