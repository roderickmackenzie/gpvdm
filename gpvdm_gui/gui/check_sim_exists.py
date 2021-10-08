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

## @package check_sim_exists
#  Check the simulation has not been removed.
#

import os
from win_lin import running_on_linux
from threading import Thread
import platform
from gpvdm_http import get_data_from_web
import hashlib
from sim_warnings import sim_warnings
import i18n
from i18n import get_full_language

_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget

from PyQt5.QtCore import QTimer

import time

class check_sim_exists(QWidget):
	sim_gone = pyqtSignal()

	def __init__(self):
		QWidget.__init__(self)
		self.sim_dir=""
			
	def foo(self,n):
		count=0
		while(1):
			if self.sim_dir!="":
				if os.path.isdir(self.sim_dir)==False:
					count=count+1
				else:
					count=0

			if count>5:
				self.sim_dir=""
				self.sim_gone.emit()
				count=0

			time.sleep(1)

	def set_dir(self,sim_dir):
		self.sim_dir=sim_dir

	def start_thread(self):
		p = Thread(target=self.foo, args=(10,))
		p.daemon = True
		p.start()

		


