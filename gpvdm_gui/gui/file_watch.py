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


## @package inp
#  Used for writing and reading .inp files from .gpvdm archives
#

#import sys
import os
import shutil
from PyQt5.QtCore import QTimer

import zipfile
from cal_path import get_sim_path
from inp import inp
from datetime import datetime
import re

class file_data():
	def __init__(self):
		self.file_name=""
		self.time=0
		self.call_backs=[]

	def __eq__(self,other):
		if self.file_name==other:
			return True
		return False

	def __str__(self):
		return self.file_name+" "+str(self.time)+" "+str(len(self.call_backs))

class file_watch():
	def __init__(self):
		self.files=[]
		self.disabled=True
		self.running=False
		self.last_run=-1

	def reset(self):
		#print("watches clear")
		self.files=[]
		self.disabled=True
		self.timer=QTimer()		
		self.timer.timeout.connect(self.check_dir)
		self.timer.start(300)

	
	def dump(self):
		print("files to watch:")
		for f in self.files:
			print(f)

	def check_files(self):
		if self.disabled==True:
			return

		for i in range(0,len(self.files)):
			f=inp()
			f.set_file_name(os.path.join(get_sim_path(),self.files[i].file_name))
			file_time=f.time()

			if self.files[i].time!=file_time:

				try:
					for c in self.files[i].call_backs:
						c()
						print("callback hook a")
				except:
					pass

				self.files[i].time=file_time
		#self.dump()

	def check_dir(self):
		if self.running==False:
			self.running=True
			if os.path.isdir(get_sim_path())==True:
				self.check_files()

		self.running=False

	def rebase(self):
		#print("rebase>>")
		self.disabled=False
		for i in range(0,len(self.files)):
			f=inp()
			f.set_file_name(os.path.join(get_sim_path(),self.files[i].file_name))

			self.files[i].time=f.time()

	def add_call_back(self,file_name,function):
		if file_name not in self.files:
			a=file_data()
			a.file_name=file_name

			f=inp()
			f.set_file_name(os.path.join(get_sim_path(),file_name))
			file_time=f.time()
			a.time=file_time

			self.files.append(a)

		for i in range(0,len(self.files)):
			if self.files[i]==file_name:
				if function not in self.files[i].call_backs:
					self.files[i].call_backs.append(function)
					break

			



watch=file_watch()

def get_watch():
	global watch
	return watch
