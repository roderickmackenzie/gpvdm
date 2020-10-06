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


## @package gpvdm_select
#  A widget for the tab widget which allows the user to select files.
#


import os

#qt
from PyQt5.QtWidgets import QMainWindow,QLabel, QFrame,QTextEdit, QAction,QApplication,QTableWidgetItem,QComboBox, QMessageBox, QDialog, QDialogButtonBox, QFileDialog
from PyQt5.QtWidgets import QGraphicsScene,QListWidgetItem,QListView,QLineEdit,QWidget,QHBoxLayout,QPushButton
from PyQt5.QtWidgets import QFileDialog
from PyQt5.uic import loadUi
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.QtCore import QPersistentModelIndex
from QComboBoxLang import QComboBoxLang
from PyQt5.QtGui import QIcon


#cal_path
from cal_path import get_ui_path
from PyQt5.QtCore import pyqtSignal

from epitaxy import get_epi
from inp import inp
from dos_io import gen_fermi_from_np
from dos_io import gen_np_from_fermi
import decimal

import i18n
_ = i18n.language.gettext


class energy_to_charge(QWidget):

	changed = pyqtSignal()

	def __init__(self):
		QWidget.__init__(self)
		self.position="top"
		self.charge_type="electron"
		self.hbox=QHBoxLayout()
		self.edit_m3=QLineEdit()
		self.edit_m3.setMaximumWidth( 60 )
		self.label_m3=QLabel("m<sup>-3</sup> /")
		self.label_m3.setStyleSheet("QLabel { border: 0px; padding: 0px; }");
		self.label_m3.setMaximumWidth( 30 )

		self.edit_eV=QLineEdit()
		self.edit_eV.setMaximumWidth( 50 )
		self.label_eV=QLabel("eV")
		self.label_eV.setStyleSheet("QLabel { border: 0px; padding: 0px; }");
		self.label_eV.setMaximumWidth( 20 )

		#self.button=QPushButton()
		#self.button.setFixedSize(25, 25)
		#self.button.setText("...")
		self.hbox.addWidget(self.edit_m3,Qt.AlignLeft)
		self.hbox.addWidget(self.label_m3,Qt.AlignLeft)
		self.hbox.addWidget(self.edit_eV,Qt.AlignLeft)
		self.hbox.addWidget(self.label_eV,Qt.AlignLeft)
		self.hbox.setSpacing(0)
		#self.hbox.addWidget(self.button)

		self.edit_m3.textChanged.connect(self.callback_m3_changed)
		self.edit_eV.textChanged.connect(self.callback_eV_changed)

		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.edit_m3.setStyleSheet("QLineEdit { border: none }");

		#self.button.clicked.connect(self.callback_button_click)

		self.setLayout(self.hbox)

	def cal_ev(self):
		for l in get_epi().layers:
			file_name=l.dos_file+".inp"
			if file_name.startswith("dos"):
				f=inp()
				f.load(file_name)
				if self.charge_type=="electron":
					eV=gen_fermi_from_np(float(self.edit_m3.text()),float(f.get_token("#Nc")),300.0)
				else:
					eV=gen_fermi_from_np(float(self.edit_m3.text()),float(f.get_token("#Nv")),300.0)
		return eV

	def cal_m3(self):
		for l in get_epi().layers:
			file_name=l.dos_file+".inp"
			if file_name.startswith("dos"):
				f=inp()
				f.load(file_name)
				if self.charge_type=="electron":
					n=gen_np_from_fermi(float(self.edit_eV.text()),float(f.get_token("#Nc")),300.0)
				else:
					n=gen_np_from_fermi(float(self.edit_eV.text()),float(f.get_token("#Nv")),300.0)
		return n


	

	def callback_eV_changed(self):
		n=self.cal_m3()
		self.edit_m3.blockSignals(True)
		self.edit_m3.setText('%.0e' % n)
		self.edit_m3.blockSignals(False)

		self.changed.emit()

	def callback_m3_changed(self):
		try:
			ev=self.cal_ev()
			self.edit_eV.blockSignals(True)
			self.edit_eV.setText('%.2f' % ev)
			self.edit_eV.blockSignals(False)
		except:
			pass
		self.changed.emit()

	def setText(self,text):
		val=float(text)
		text='%.2e' % val
		text=str(decimal.Decimal(text).normalize()).lower().replace('+', '')
		self.edit_m3.setText(text)
		#self.edit_eV.setText(text)

	def text(self):
		return self.edit_m3.text()
		
