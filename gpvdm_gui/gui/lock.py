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

## @package lock

import os
import re
import time
from http_get import http_get
import urllib.parse

from inp import inp_save
from inp import inp_search_token_value
from inp import inp_load_file
from inp import inp
from str2bool import str2bool

from win_lin import running_on_linux
if running_on_linux()==False:
	import winreg

import platform

from cal_path import get_user_settings_dir
from cal_path import get_exe_path
from cal_path import multiplatform_exe_command
import time

import getpass


class lock():
	def __init__(self):
		self.registered=False
		self.uid=""
		self.error=""
		self.open_gl_working=True
		self.reg_client_ver="ver"
		self.client_ver_from_lock=""
		self.status="no_key"
		self.locked=[]
		self.not_locked=[]
		self.update_available=False
		self.website="www.gpvdm.com"
		self.port="/api"
		self.my_email="roderick.mackenzie@durham.ac.uk"

		#date_time = "01.01.2022"
		#pattern = "%d.%m.%Y"
		#change_time = int(time.mktime(time.strptime(date_time, pattern)))
		#now_time = time.time()

		#decode0 = time.gmtime(change_time)
		#decode1 = time.gmtime(now_time)

		#delta=time.mktime(decode1)-time.mktime(decode0)
		#if delta>0:
		#	self.my_email="r.c.i.mackenzie at googlemail.com"

		self.question="Questions? Contact: "
		self.data_path=os.path.join(get_user_settings_dir(),"info.inp")

	def get_uid(self):
		return self.uid

	def is_function_locked(self,id):
		return False

	def is_function_not_locked(self,id):
		return False


	def get_reg_key(self,token):
		if running_on_linux()==False:
			try:
				registry_key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, "Software\\gpvdm", 0, winreg.KEY_READ)
				value, regtype = winreg.QueryValueEx(registry_key, token)
				winreg.CloseKey(registry_key)
				return value
			except WindowsError:
				pass

		return False

	def is_registered(self):
		return True

	def is_expired(self):
		return False

	def is_trial(self):
		return False

	def is_gpvdm_next(self):
		if os.path.isfile(os.path.join(get_user_settings_dir(),"settings2.inp"))==True:
			return True
		return False

my_lock=lock()

def get_lock():
	global my_lock
	return my_lock

def get_email():
	global my_lock
	return my_lock.my_email

