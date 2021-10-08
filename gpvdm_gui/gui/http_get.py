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


## @package http_get
#  Get data from the web
#

import sys
import os
from urllib.parse import urlparse
from PyQt5.QtCore import pyqtSignal,QObject
import urllib.request
from time import sleep


class http_get(QObject):
	got_data = pyqtSignal(str,int)

	def __init__(self):
		QObject.__init__(self)

	def emit_status(self):
		self.got_data.emit(self.address,self.pos)


	def get(self,address):
		self.address=address
		self.data=b""

		#print(self.address)

		try:
			response=urllib.request.urlopen(self.address)
		except:
			print("fail")
			return False
		#print(type(response.headers['content-length']))
		self.data=bytearray(int(response.headers['content-length']))
		self.pos=0 
		while True:

			chunk = response.read(1024*32)
			if not chunk:
				break
			self.data[self.pos:self.pos+len(chunk)] = chunk
			self.pos=self.pos+len(chunk)
			#self.data=self.data+chunk
			self.emit_status()
			#sys.stdout.write('.')

		return self.data




