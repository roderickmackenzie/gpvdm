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

## @package token_val_lib
#  A cache containing tokens which need to be read quickly
#

import i18n
_ = i18n.language.gettext
from inp import inp_update_token_value

class token_data():
	def __init__(self):
		self.file=""
		self.token=""
		self.value=""
		self.edited=False

	def save(self):
		inp_update_token_value(self.file,self.token,self.value)		

	def __eq__(self,f):
		if f.file==self.file and f.token==self.token:
			return True

		return False

def token_data_lib():
	def __init__(self):
		self.data=[]

	def get_token_data(self,file_name,token):
		new_token=token_data()
		new_token.file=file_name
		new_token.token=token

		for t in self.data:
			if t==new_token:
				return t.value

		self.data.append()
