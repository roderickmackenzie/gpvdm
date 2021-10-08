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


## @package inp_encrypt
#  Used for writing and reading encrypted files
#

#import sys
import os
import shutil
from util_zip import replace_file_in_zip_archive

import hashlib
aes_support=True
try:
	import pyaes
except:
	aes_support=False

def decrypt_file(out_file,in_file,key):
	if aes_support==False:
		print("no encoding")
		return False
	iv="857585291b631481e586f3023b03c548a95ea6f369bdda171a6e0bc28ae929ee"

	bs=32

	f = open(in_file, mode='rb')
	data = f.read()
	f.close()

	m = hashlib.md5()
	m.update(key.encode('utf-8'))
	key_hash=m.digest()

	m = hashlib.md5()
	m.update(iv.encode('utf-8'))
	iv_hash=m.digest()

	if data.startswith(b"gpvdmenc")==True:
		data=list(data[8:])

		aes = pyaes.AESModeOfOperationCBC (key_hash,iv=iv_hash)
		#encryptor = AES.new(key_hash, AES.MODE_CBC, IV=iv_hash)

		#ret= encryptor.decrypt(bytes(data))
		ret=b''
		while len(data)!=0:
			ret += aes.decrypt(bytes(data[:16]))
			data = data[16:]	

		#ret = aes.decrypt (bytes(data))

		if ret.startswith(b"gpvdm")==False:
			return False

		num=""
		pos=0
		ret=ret[5:]
		while(chr(ret[pos])!="#"):
			num=num+chr(ret[pos]) 
			pos=pos+1

		pos=pos+1
		try:
			data_len=int(num)
		except:
			return False

		f = open(out_file, mode='wb')
		f.write(ret[pos:pos+data_len])
		f.close()

		return True
	else:
		return False

def encrypt_file(out_file,in_file,key):

	iv="857585291b631481e586f3023b03c548a95ea6f369bdda171a6e0bc28ae929ee"

	bs=32

	f = open(in_file, mode='rb')
	data = f.read()
	f.close()

	print(len(data))

	m = hashlib.md5()
	m.update(key.encode('utf-8'))
	key_hash=m.digest()

	m = hashlib.md5()
	m.update(iv.encode('utf-8'))
	iv_hash=m.digest()

	#encryptor = AES.new(key_hash, AES.MODE_CBC, IV=iv_hash)
	aes = pyaes.AESModeOfOperationCBC (key_hash,iv=iv_hash)


	head="gpvdm"+str(len(data))+"#"
	data=bytes(head,'utf-8')+data
	padding=(int((len(data))/16.0)+1)*16-len(data)
	data+=b"\0" * padding

	ret=b''
	while len(data)!=0:
		ret += aes.encrypt(bytes(data[:16]))
		data = data[16:]	

	ret=b"gpvdmenc"+ret
	f = open(out_file, mode='wb')
	f.write(ret)
	f.close()

	return True

