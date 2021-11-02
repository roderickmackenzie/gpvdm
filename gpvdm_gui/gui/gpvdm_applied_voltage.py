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


## @package gpvdm_applied_voltage
#  Select the voltage to apply to a contact
#


import os

from PyQt5.QtWidgets import QMessageBox, QDialog
from PyQt5.QtWidgets import QLineEdit,QWidget,QHBoxLayout,QPushButton
from PyQt5.QtCore import pyqtSignal
from QComboBoxLang import QComboBoxLang

#cal_path
from cal_path import subtract_paths
from cal_path import get_materials_path
from gpvdm_json import gpvdm_data

import i18n
_ = i18n.language.gettext


class gpvdm_applied_voltage(QWidget):

	changed = pyqtSignal()

	def __init__(self,file_box=True):
		QWidget.__init__(self)
		self.raw_value="ground"
		self.hbox=QHBoxLayout()
		self.edit=QLineEdit()
		self.combobox = QComboBoxLang()
		self.combobox.addItemLang("ground",_("Ground"))
		self.combobox.addItemLang("constant",_("Constant bias"))
		self.combobox.addItemLang("change",_("Change"))

		self.hbox.addWidget(self.combobox)
		self.hbox.addWidget(self.edit)

		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.edit.setStyleSheet("QLineEdit { border: none }");


		#self.button.clicked.connect(self.callback_button_click)
		self.combobox.currentIndexChanged.connect(self.callback_combobox)
		self.edit.textChanged.connect(self.callback_edit)
		self.setLayout(self.hbox)

	def update(self):
		self.edit.blockSignals(True)
		self.combobox.blockSignals(True)
		cb_value=self.contact.applied_voltage_type
		self.combobox.setValue_using_english(cb_value)
		if cb_value!="ground" and cb_value!="change":
			self.edit.setEnabled(True)
			self.edit.setText(str(self.contact.applied_voltage))
		else:
			self.edit.setEnabled(False)
			if cb_value=="ground":
				self.edit.setText("Gnd")
			elif cb_value=="change":
				self.edit.setText("Vsig")

		self.combobox.blockSignals(False)
		self.edit.blockSignals(False)


	def callback_edit(self):
		try:
			val=str(float(self.edit.text()))
		except:
			return
		self.find_contact()
		self.contact.applied_voltage=float(self.edit.text())
		self.changed.emit()

	def callback_combobox(self):
		self.find_contact()
		self.contact.applied_voltage_type=self.combobox.currentText_english()
		self.update()
		self.changed.emit()

	def updateValue(self,uid):
		self.uid=uid
		self.find_contact()
		print(self.uid,self.contact.applied_voltage_type)
		self.update()

	def find_contact(self):
		self.contact=None
		for c in gpvdm_data().epi.contacts.segments:
			if self.uid==c.id:
				self.contact=c

		
