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

## @package libcrypt
#  Utils for lock
#


import sys
import os

def libcrypt_decrypt(data,key="hello"):
	if data.startswith(b"gpvdmenc")==False:
		return data

	kpos=0
	kmax=len(key)
	key=bytearray(key.encode())

	if data.startswith(b"gpvdmenc")==True:
		data=list(data[8:])
		for i in range(0,len(data)):
			data[i]=data[i] ^ key[kpos]
			kpos=kpos+1
			if kpos>=kmax:
				kpos=0
		data=bytearray(data)

		if data.startswith(b"gpvdm")==False:
			return b"gpvdmenc"
		data=data[5:]

	return data

