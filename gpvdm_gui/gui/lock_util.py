# -*- coding: utf-8 -*-
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

## @package lock_util
#  Utils for lock
#

import sys
import os

def lock_load(file_name):
	if os.path.isfile(file_name)==False:
		return False

	key="ahja"


	kpos=0
	kmax=len(key)
	key=bytearray(key.encode())
	f = open(file_name, mode='rb')
	data = f.read()
	f.close()

	if data.startswith(b"gpvdmenc")==True:
		data=list(data[8:])
		for i in range(0,len(data)):
			data[i]=data[i] ^ key[kpos]
			kpos=kpos+1
			if kpos>=kmax:
				kpos=0
		data=bytearray(data)

		if data.startswith(b"gpvdm")==False:
			return False
		data=data[5:]
		ret=data.decode("utf-8", "strict").split("\n")
	else:
		data=data.decode('utf-8')
		ret=data.split("\n")

	return ret

	return
