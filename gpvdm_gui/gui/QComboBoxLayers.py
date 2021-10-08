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

## @package QComboBoxLayers
#  A combobox used to select layers
#
import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QSizePolicy,QComboBox
from PyQt5.QtCore import pyqtSignal
from gpvdm_json import gpvdm_data

class QComboBoxLayers(QComboBox):
	
	def __init__(self):
		QComboBox.__init__(self)
		data=gpvdm_data()
		for l in data.epi.layers:
			self.addItem(l.shape_name)

		self.addItem("none")
	
	def setValue(self,value):
		all_items  = [self.itemText(i) for i in range(self.count())]
		for i in range(0,len(all_items)):
			if all_items[i] == value:
				self.setCurrentIndex(i)
				break
	



