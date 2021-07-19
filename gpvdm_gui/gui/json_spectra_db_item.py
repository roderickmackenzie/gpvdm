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


## @package json_spectra_db_item
#  Store the shape information
#


from json_base import json_base
from json_import_config import json_import_config

class json_spectra_db_item(json_base):

	def __init__(self):
		json_base.__init__(self,"spectra_db_item")
		self.var_list=[]
		self.var_list.append(["item_type","spectra"])
		self.var_list.append(["status","public"])
		self.var_list.append(["changelog",""])
		self.var_list.append(["spectra_import",json_import_config(name="spectra_import")])
		self.var_list_build()
		self.include_name=False
