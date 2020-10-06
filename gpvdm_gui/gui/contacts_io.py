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


class contact(shape):
	def __init__(self):
		super().__init__()
		self.name=""
		self.position=""
		self.applied_voltage_type="constant"
		self.applied_voltage="-2.0"
		self.contact_resistance_sq=0.0
		self.shunt_resistance_sq=0.0
		self.np=1e20
		self.charge_type="electron"
		self.physical_model="ohmic"

		#contacts_dump()
class contacts_io():

	def __init__(self):
		self.contacts=[]

	#def init_watch(self):
	#	if gui_get()==True:
	#		get_watch().add_call_back("contacts.inp",self.em)

	def em(self):
		self.load()
		self.changed.emit()

	def load(self):
		self.contacts=[]

		pos=0
		contact_file=inp()
		if contact_file.load(os.path.join(get_sim_path(),"contacts.inp"))!=False:
			contact_file.to_sections(start="#contact_position")

			for s in contact_file.sections:
				c=contact()

				c.position=s.contact_position

				c.applied_voltage_type=s.contact_applied_voltage_type

				c.applied_voltage=s.contact_applied_voltage

				c.np=s.contact_charge_density

				c.charge_type=s.contact_charge_type

				c.contact_resistance_sq=s.contact_resistance_sq

				c.shunt_resistance_sq=s.shunt_resistance_sq

				c.physical_model=s.physical_model

				c.ve0=s.contact_ve
				c.vh0=s.contact_vh

				c.shape_dos="none"
				c.load(s.contact_shape_file_name)
				#print(c.shape_enabled,c.name,s.contact_shape_file_name)
				self.contacts.append(c)

	def print():
		for s in self.contacts:
			print(s.x0, s.dx,s.depth,s.contact_applied_voltage,s.contact_applied_voltage_type)

	def clear():
		self.contacts=[]


	def gen_file(self):
		lines=[]
		lines.append("#sections")
		lines.append(str(len(self.contacts)))
		i=0
		for s in self.contacts:
			lines.append("#contact_position"+str(i))
			lines.append(str(s.position))
			lines.append("#contact_applied_voltage_type"+str(i))
			lines.append(str(s.applied_voltage_type))
			lines.append("#contact_applied_voltage"+str(i))
			lines.append(str(s.applied_voltage))
			lines.append("#contact_charge_density"+str(i))
			lines.append(str(s.np))
			lines.append("#contact_charge_type"+str(i))
			lines.append(str(s.charge_type))
			lines.append("#contact_shape_file_name"+str(i))
			lines.append(s.file_name)
			lines.append("#contact_resistance_sq"+str(i))
			lines.append(str(s.contact_resistance_sq))
			lines.append("#shunt_resistance_sq"+str(i))
			lines.append(str(s.shunt_resistance_sq))
			lines.append("#physical_model"+str(i))
			lines.append(s.physical_model)
			lines.append("#contact_ve"+str(i))
			lines.append(str(s.ve0))
			lines.append("#contact_vh"+str(i))
			lines.append(str(s.vh0))
			i=i+1

		lines.append("#ver")
		lines.append("1.3")
		lines.append("#end")

		return lines


	def dump(self):
		lines=self.gen_file()
		for s in lines:
			print(s)


	def save(self):
		lines=self.gen_file()
		inp_save(os.path.join(get_sim_path(),"contacts.inp"),lines)

		for c in self.contacts:
			c.save()

	def remove_by_id(self,ids):
		if type(ids)==str:
			ids=[ids]

		for c in self.contacts[:]:
			if c.id in ids:
				self.contacts.remove(c)

	def get_layers_with_contacts(self):
		layers=[]
		for c in self.contacts:
			if c.position not in layers:
				layers.append(c.position)

		return layers

	def insert(self,pos,shape_file_name):
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
		s.load(shape_file_name)
		s.name="new_contact"
		s.type="box"
		s.shape_dos="none"
		self.contacts.insert(pos,s)
		return self.contacts[pos]

