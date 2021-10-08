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

## @package fx_selector
#  Select which wavelenght to display
#

import os
import zipfile
import glob
from tab import tab_class

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu, QComboBox, QMenuBar, QLabel
from PyQt5.QtGui import QIcon
import operator

from cal_path import get_sim_path
from inp import inp

class lam_file():
	def __init__(self):
		self.file=""
		self.lam=0.0
		self.layer=0

	def __str__(self):
		return str(self.lam*1e9)+"nm "#layer "+str(self.layer)

	def text(self):
		return str(self.lam*1e9)+"nm "#layer "+str(self.layer)
		

class fx_selector(QWidget):

	def __init__(self):
		QWidget.__init__(self)
		self.thefiles=[]
		self.dump_dir=os.path.join(get_sim_path(),"ray_trace")

		self.layout=QVBoxLayout()
		label=QLabel(_("Wavelengths")+":")
		self.layout.addWidget(label)

		self.cb = QComboBox()
		self.layout.addWidget(self.cb)
		self.setLayout(self.layout)
		self.show()
		self.show_all=False

	def get_file_name(self):
		txt=self.cb.currentText()
		for i in range(0, len(self.thefiles)):
			if str(self.thefiles[i])==txt:
				return os.path.join(self.dump_dir,self.thefiles[i].file_name)

		return "none"


		
	def find_modes(self,path):
		result = []
		lines=[]
		dump_dir=os.path.join(get_sim_path(),"ray_trace")
		path=os.path.join(dump_dir,"wavelengths.dat")

		if os.path.isfile(path)==True:

			f = open(path, "r")
			lines = f.readlines()
			f.close()
			
			for l in range(0, len(lines)):
				txt=lines[l].rstrip()
				if txt!="":
					result.append(txt)

		
		if os.path.isdir(dump_dir)==True:
			files=os.listdir(dump_dir)
			files.sort()
			for f in files:
				sub_dir=os.path.join(dump_dir,f)
				if os.path.isdir(sub_dir)==True:
					json_file_name=os.path.join(sub_dir,"data.json")
					json_file=inp()
					json_file.load_json(json_file_name)
					sub_files=os.listdir(sub_dir)
					for sf in sub_files:
						if sf.startswith("ray_"):
							a=lam_file()
							a.layer=0#f[10:11]
							a.lam=json_file.json["wavelength"]
							a.file_name=os.path.join(sub_dir,sf)
							
							result.append(a)

		return result

	def update(self):
		self.cb.blockSignals(True)

		self.thefiles=self.find_modes(self.dump_dir)

		if len(self.thefiles)==0:
			self.setEnabled(False)
		else:
			self.setEnabled(True)

		self.cb.clear()

		for i in range(0, len(self.thefiles)):
			path=os.path.join(self.dump_dir,self.thefiles[i].file_name)
			if os.path.isfile(path):
				self.cb.addItem(str(self.thefiles[i]))
		self.cb.setCurrentIndex(0)
		self.cb.blockSignals(False)

