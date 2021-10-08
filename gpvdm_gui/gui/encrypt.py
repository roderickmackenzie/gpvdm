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

## @package encrypt
#  Encrypt data used for cluster communication.
#

from win_lin import running_on_linux
import hashlib
from inp import inp_get_token_value
enable_crypto=False
try:
	from Crypto.Cipher import AES
	enable_crypto=True
except:
	pass
iv=""
key=""

def encrypt_load():
	global key
	global iv
	if running_on_linux()==True:
		iv=inp_get_token_value("cluster.inp","#iv")
		key=inp_get_token_value("cluster.inp","#key")

def encrypt(data):
	return data
	global key
	global iv
	global enable_crypto
	if enable_crypto==True:
		ret=""
		m = hashlib.md5()
		m.update(key.encode('utf-8'))
		key_hash=m.digest()

		m = hashlib.md5()
		m.update(iv.encode('utf-8'))
		iv_hash=m.digest()

		encryptor = AES.new(key_hash, AES.MODE_CBC, IV=iv_hash)

		ret= encryptor.encrypt(bytes(data[0:512]))

		encryptor = AES.new(key_hash, AES.MODE_CBC, IV=iv_hash)

		ret=ret+encryptor.encrypt(bytes(data[512:len(data)]))

	return ret

def encrypt2(data):
	return data
	global key
	global iv
	global enable_crypto
	if enable_crypto==True:
		ret=""
		m = hashlib.md5()
		m.update(key.encode('utf-8'))
		key_hash=m.digest()

		m = hashlib.md5()
		m.update(iv.encode('utf-8'))
		iv_hash=m.digest()

		encryptor = AES.new(key_hash, AES.MODE_CBC, IV=iv_hash)

		ret= encryptor.encrypt(bytes(data[0:32]))

		encryptor = AES.new(key_hash, AES.MODE_CBC, IV=iv_hash)

		ret=ret+encryptor.encrypt(bytes(data[32:len(data)]))

	return ret

def decrypt(data):
	return data
	global key
	global iv
	global enable_crypto
	if enable_crypto==True:
		ret=""
		m = hashlib.md5()
		m.update(key.encode('utf-8'))
		key_hash=m.digest()

		m = hashlib.md5()
		m.update(iv.encode('utf-8'))
		iv_hash=m.digest()

		encryptor = AES.new(key_hash, AES.MODE_CBC, IV=iv_hash)

		ret= encryptor.decrypt(bytes(data))

	return ret
