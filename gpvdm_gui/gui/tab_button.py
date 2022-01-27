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


## @package gpvdm_select
#  A widget for the tab widget which allows the user to select files.
#


import os

#qt
try:
	from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction,QApplication,QTableWidgetItem,QComboBox, QMessageBox, QDialog, QDialogButtonBox, QFileDialog
	from PyQt5.QtWidgets import QGraphicsScene,QListWidgetItem,QListView,QLineEdit,QWidget,QHBoxLayout,QPushButton
	from PyQt5.QtWidgets import QFileDialog
	from PyQt5.uic import loadUi
	from PyQt5.QtGui import QPixmap
	from PyQt5.QtCore import QSize, Qt, QTimer
	from PyQt5.QtCore import QPersistentModelIndex
	from QComboBoxLang import QComboBoxLang
	from PyQt5.QtGui import QIcon
except:
	pass

#cal_path
from cal_path import get_ui_path
from open_save_dlg import open_as_filter
from PyQt5.QtCore import pyqtSignal

import i18n
_ = i18n.language.gettext


class tab_button(QWidget):
	changed = pyqtSignal(str)

	def __init__(self):
		QWidget.__init__(self)
		self.data=None
		self.hbox=QHBoxLayout()
		self.edit=QLineEdit()
		self.button=QPushButton()
		self.button.setFixedSize(50, 25)
		self.button.setText("Edit")
		#self.hbox.addWidget(self.edit)
		self.hbox.addWidget(self.button)
		self.uid=None
		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.edit.setStyleSheet("QLineEdit { border: none }");

		self.button.clicked.connect(self.callback_button)
		self.setLayout(self.hbox)

	def callback_button(self):
		self.changed.emit(self.uid)
		
