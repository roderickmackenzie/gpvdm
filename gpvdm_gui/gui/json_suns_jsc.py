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


## @package json_suns_jsc
#  Store the suns_voc json data
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_suns_jsc_simulation(json_base):

	def __init__(self):
		json_base.__init__(self,"suns_voc_segment")
		self.var_list=[]
		self.var_list.append(["english_name","Suns\\nVoc"])
		self.var_list.append(["icon","sunsjsc"])
		self.var_list.append(["sunstart",0.01])
		self.var_list.append(["sunstop",10.0])
		self.var_list.append(["sundp",0.01])
		self.var_list.append(["sundpmul",1.05])
		self.var_list_build()


class json_suns_jsc(json_base):

	def __init__(self):
		json_base.__init__(self,"suns_jsc",segment_class=True)
		self.segments.append(json_suns_jsc_simulation())

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_suns_jsc_simulation()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

