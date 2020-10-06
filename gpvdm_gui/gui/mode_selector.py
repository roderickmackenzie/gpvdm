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

## @package mode_selector
#  Select which optical mode to display
#

import os
from inp import inp_update_token_value
from inp import inp_get_token_value
import zipfile
import glob
from tab import tab_class

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu, QComboBox, QMenuBar, QLabel
from PyQt5.QtGui import QIcon
import operator

from cal_path import get_sim_path

class lam_file():
	def __init__(self):
		self.file_name=""
		self.lam=0.0
	def __str__(self):
		return str(self.lam)+"nm"

	def text(self):
		return str(self.lam)+"nm"
		

class mode_selector(QWidget):

	def __init__(self):
		QWidget.__init__(self)
		self.thefiles=[]
		self.dump_dir=os.path.join(get_sim_path(),"optical_output")

		self.layout=QVBoxLayout()
		label=QLabel(_("Wavelengths")+":")
		self.layout.addWidget(label)

		self.cb = QComboBox()
		self.layout.addWidget(self.cb)
		self.setLayout(self.layout)
		self.show()
		self.show_all=False

	def get_text(self):
		val=self.cb.currentText()
		if val=="all":
			return "all"
		for v in self.thefiles:
			if val==str(v):
				return v.lam
		return None

	def get_file_name(self):
		txt=self.cb.currentText()
		for i in range(0, len(self.thefiles)):
			if str(self.thefiles[i])==txt:
				return os.path.join(self.dump_dir,self.thefiles[i].file_name)

		return "none"


		
	def find_modes(self,path):
		result = []
		lines=[]
		path=os.path.join(self.dump_dir,"wavelengths.dat")

		if os.path.isfile(path)==True:

			f = open(path, "r")
			lines = f.readlines()
			f.close()
			
			for l in range(0, len(lines)):
				txt=lines[l].rstrip()
				if txt!="":
					a=lam_file()
					a.lam=txt
					a.file_name=""
					
					result.append(a)

		return result

	def update(self):
		self.dump_dir=os.path.join(get_sim_path(),"optical_output")
		self.cb.blockSignals(True)

		self.thefiles=self.find_modes(self.dump_dir)
		self.cb.clear()

		self.cb.addItem("all")

		for i in range(0, len(self.thefiles)):
			self.cb.addItem(str(self.thefiles[i]))

		self.cb.setCurrentIndex(0)
		self.cb.blockSignals(False)

