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


## @package gpvdm_select_emission
#  A widget for the tab widget which allows the user to select material files.
#


import os

from PyQt5.QtWidgets import QMessageBox, QDialog
from PyQt5.QtWidgets import QLineEdit,QWidget,QHBoxLayout,QPushButton
from PyQt5.QtCore import pyqtSignal

#cal_path
from cal_path import subtract_paths
from cal_path import get_emission_path

import i18n
_ = i18n.language.gettext


class gpvdm_select_emission(QWidget):

	changed = pyqtSignal()

	def __init__(self,file_box=True):
		QWidget.__init__(self)
		self.hbox=QHBoxLayout()
		self.edit=QLineEdit()
		self.button=QPushButton()
		self.button.setFixedSize(25, 25)
		self.button.setText("...")

		if file_box==True:
			self.hbox.addWidget(self.button)

		self.hbox.addWidget(self.edit)

		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.edit.setStyleSheet("QLineEdit { border: none }");


		self.button.clicked.connect(self.callback_button_click)

		self.setLayout(self.hbox)

	def callback_button_click(self):
		from gpvdm_open import gpvdm_open
		dialog=gpvdm_open(get_emission_path(),act_as_browser=False)

		ret=dialog.exec_()
		if ret==QDialog.Accepted:
			file_name=dialog.get_filename()
			rel_path=subtract_paths(get_emission_path(),file_name)
			rel_path=rel_path.replace("\\", "/")
			self.setText(rel_path)
			self.changed.emit()

	def setText(self,text):
		self.edit.setText(text)
	
	def text(self):
		return self.edit.text()
		
