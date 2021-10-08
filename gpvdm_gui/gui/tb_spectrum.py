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

## @package tb_spectrum
#  Select which solar spectrum to use.
#


import os
import glob
from tab import tab_class

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QTabWidget,QSystemTrayIcon,QMenu, QComboBox, QMenuBar, QLabel
from PyQt5.QtGui import QIcon

from cal_path import find_light_source
from gpvdm_json import gpvdm_data

class tb_spectrum(QComboBox):

	def __init__(self):
		QWidget.__init__(self)
		self.update()

	def set_value(self,val):
		self.setCurrentIndex(self.findText(val))

	def get_value(self):
		return self.currentText()

	def update(self):
		data=gpvdm_data()
		self.blockSignals(True)
		models=find_light_source()
		for i in range(0, len(models)):
			self.addItem(models[i])

		used_model=data.light.sun

	
		self.blockSignals(False)
