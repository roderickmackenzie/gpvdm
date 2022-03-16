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
from str2bool import str2bool

class lock():
	def __init__(self):
		self.lock_enabled=True
		self.registered=False
		self.error=""
		self.open_gl_working=True
		self.reg_client_ver="ver"
		self.website="www.gpvdm.com"
		self.port="/api"
		self.my_email="roderick.mackenzie@durham.ac.uk"
		self.question="Questions? Contact: "
		self.data_path=os.path.join(get_user_settings_dir(),"info.inp")

		self.data=json_base("lock")
		self.data.var_list.append(["uid",""])
		self.data.var_list.append(["no_key",""])
		self.data.var_list.append(["status",""])
		self.data.var_list.append(["gpvdm_next",False])
		self.data.var_list.append(["message",""])
		self.data.var_list.append(["update_available",False])
		self.data.var_list_build()
		self.data.locked={}
		if self.load()==True:
			if self.data.client_ver!=self.reg_client_ver:
				self.get_license()

	def check_license(self):
		if self.lock_enabled==False:
			return None

		command=multiplatform_exe_command(get_exe_command()+" --use")
		os.system(command)

	def report_bug(self,data):
		a=http_get()
		params = {'action':"crash_report",'ver_core': const_ver()+" "+self.reg_client_ver, 'uid': self.data.uid,'data':data}
		tx_string="http://"+self.website+self.port+"/debug?"+urllib.parse.urlencode(params)
		print(tx_string)
		lines=a.get(tx_string)

	def check_license_thread(self):
		if self.lock_enabled==False:
			return True

		p = Thread(target=self.check_license)
		p.daemon = True
		p.start()

	def register(self,user_data):
		if self.lock_enabled==False:
			return None

		reg_path=os.path.join(gpvdm_paths.get_tmp_path(),"reg.txt")
		user_data.save_as(reg_path,do_tab=False)

		command=multiplatform_exe_command(get_exe_command()+" --register")
		os.system(command)
		#print("done")
		#l.delete()

		l=inp()
		l.load(os.path.join(gpvdm_paths.get_tmp_path(),"ret.txt"))
		lines=l.get_token("#ret")

		if lines==False:
			return False

		if lines=="error:no_internet":
			self.error="no_internet"
			return False

		if lines=="error:error_server":
			self.error="no_internet"
			return False

		if lines=="error:too_old":
			self.error="too_old"
			return False

		self.data.uid=lines

		return True

	def html(self):
		text=""
		text=text+"UID:"+self.data.uid+"<br>"
		return text

	def get_license(self,key="none",uid=None):
		if self.lock_enabled==False:
			return None

		if uid==None:
			uid=self.data.uid

		command=multiplatform_exe_command(get_exe_command()+" --license")
		os.system(command)

		l=inp()
		l.load(os.path.join(gpvdm_paths.get_tmp_path(),"ret.txt"))
		lines=l.get_token("#ret")
		if lines==False:
			return False

		if lines=="error:too_old":
			self.error="too_old"
			return False

		if lines=="error:error":
			self.error="uid_not_found"
			return False

		self.load()

		self.registered=True
		return True

	def get_uid(self):
		return self.data.uid

	def is_gpvdm_next(self):
		return self.data.gpvdm_next


	def get_next_gui_action(self):
		if self.lock_enabled==False:
			return "ok"

		if self.registered==False:
			return "register"

		return "ok"

	def is_function_locked(self,id):
		if self.lock_enabled==False:
			return False

		for key in self.data.locked:
			if key==id:
				return True
		return False


	def load_new(self):
		if self.lock_enabled==False:
			return None

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

		#print(json_data)

		try:
			json_data=json.loads(json_data)
		except:
			return False

		self.data.import_raw_json(json_data)
		self.data.gpvdm_next=str2bool(self.data.gpvdm_next)
		self.data.update_available=str2bool(self.data.update_available)
		self.use_count=self.data.use_count

		self.registered=True
		return True

	def load(self):
		if self.lock_enabled==False:
			return None

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
		elif lines=="error:too_old":
			self.error="too_old"
			return False

		self.error=lines
		return False

	

my_lock=lock()

def get_lock():
	global my_lock
	return my_lock

