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

## @package register
#  Registration window
#

import os

from icon_lib import icon_get

from error_dlg import error_dlg

from http_get import http_get
import urllib.parse
from cal_path import get_user_settings_dir
from win_lin import running_on_linux
from os.path import expanduser
import time
from i18n import get_full_language
from lock_util import lock_load

from cal_path import get_exe_path
from threading import Thread

from cal_path import get_exe_command
from const_ver import const_ver

if running_on_linux()==False:
	import winreg

import platform
from i18n import get_full_language
from inp import inp
from cal_path import multiplatform_exe_command
from cal_path import get_user_settings_dir
from cal_path import gpvdm_paths
import json
from json_base import json_base

class lock():
	def __init__(self):
		self.registered=False

		self.error=""
		self.open_gl_working=True
		self.reg_client_ver="ver"
		self.locked=[]
		self.not_locked=[]
		self.update_available=False
		self.website="www.gpvdm.com"
		self.port="/api"
		self.my_email="roderick.mackenzie@durham.ac.uk"
		self.question="Questions? Contact: "
		self.data_path=os.path.join(get_user_settings_dir(),"info.inp")

		self.data=json_base("lock")
		self.data.var_list.append(["uid",""])
		self.data.var_list.append(["no_key",""])
		self.data.var_list_build()

		if self.load()==True:
			if self.data.client_ver!=self.reg_client_ver:
				self.get_license()

	def check_license(self):
		command=multiplatform_exe_command(get_exe_command()+" --use")
		os.system(command)

	def report_bug(self,data):
		#Transmit debug info
		a=http_get()
		params = {'action':"crash_report",'ver_core': const_ver()+" "+self.reg_client_ver, 'uid': self.get_uid(),'data':data.replace("\n"," ")}
		tx_string="http://"+self.website+self.port+"/debug?"+urllib.parse.urlencode(params)
		lines=a.get(tx_string)

	def check_license_thread(self):
		p = Thread(target=self.check_license)
		p.daemon = True
		p.start()

	def register(self,user_data):
		reg_path=os.path.join(gpvdm_paths.get_tmp_path(),"reg.txt")
		user_data.save_as(reg_path,do_tab=False)

		command=multiplatform_exe_command(get_exe_command()+" --register")
		os.system(command)
		print("done")
		#l.delete()

		l=inp()
		l.load(os.path.join(gpvdm_paths.get_tmp_path(),"ret.txt"))
		lines=l.get_token("#ret")

		if lines==False:
			return False

		if lines=="no_internet":
			self.error="no_internet"
			return False

		if lines=="tooold":
			self.error="too_old"
			return False

		self.data.uid=lines

		return True

	def html(self):
		text=""
		text=text+"UID:"+self.data.uid+"<br>"
		return text

	def get_license(self,key="none",uid=None):
		if uid==None:
			uid=self.data.uid

		command=multiplatform_exe_command(get_exe_command()+" --license")
		os.system(command)

		l=inp()
		l.load(os.path.join(gpvdm_paths.get_tmp_path(),"ret.txt"))
		lines=l.get_token("#ret")
		if lines==False:
			return False

		if lines=="tooold":
			self.error="too_old"
			return False

		if lines=="error":
			self.error="uid_not_found"
			return False

		self.load()

		self.registered=True
		return True

	def get_uid(self):
		return self.data.uid

	def is_gpvdm_next(self):
		if os.path.isfile(os.path.join(get_user_settings_dir(),"settings2.inp"))==True:
			return True
		return False


	def get_next_gui_action(self):
		if self.registered==False:
			return "register"

		return "ok"


	def is_function_locked(self,id):
		return True
		if id in self.locked:
			return True
		return False

	def is_function_not_locked(self,id):
		if id in self.not_locked:
			return True
		return False

	def load_new(self):
		if self.get_reg_key("new_install")=="true":
			print("fresh install.....")
			return False

		lines=[]

		if os.path.isfile(os.path.join(get_user_settings_dir(),"settings2.inp"))==False:
			return False

		self.reg_client_ver=self.get_reg_key("ver")
		if self.reg_client_ver==False:
			self.reg_client_ver="linux"

		lines=lock_load(self.data_path)

		if lines==False:
			return False


		json_data="\n".join(lines)
		json_data=json.loads(json_data)
		self.data.import_raw_json(json_data)	

		if self.data.ver=="2.0":
			return False

		self.use_count=self.data.use_count
		self.locked=self.data.locked.split(";")
		self.not_locked=self.data.not_locked.split(";")

		self.registered=True
		return True

	def load(self):

		if self.load_new()==True:
			return

		value=self.get_reg_key("uid")
		if value!=False:
			self.data.uid=value
			#print("I found a uid in the registry.")
		return

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


	def is_expired(self):
		if self.data.status=="expired":
			return True
		return False

	def is_trial(self):
		if self.data.status=="no_key":
			return False

		if self.data.status=="full_version":
			return False

		return True

	def validate_key(self,key):
		command=multiplatform_exe_command(get_exe_command()+" --validate "+key)
		os.system(command)

		l=inp()
		l.load(os.path.join(gpvdm_paths.get_tmp_path(),"ret.txt"))
		lines=l.get_token("#ret")

		if lines==False:
			self.error="no_internet"
			return False

		if lines=="ok":
			self.load()
			return True
		elif lines=="tooold":
			self.error="too_old"
			return False

		self.error=lines
		return False

	

my_lock=lock()

def get_lock():
	global my_lock
	return my_lock

def get_email():
	return "roderick.mackenzie@nottingham.ac.uk"
