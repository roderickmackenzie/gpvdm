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


## @package icon_widget
#  A widget to show the icon
#


import os

#qt
from PyQt5.QtWidgets import QMainWindow,QLabel, QFrame,QTextEdit, QAction,QApplication,QTableWidgetItem,QComboBox, QMessageBox, QDialog, QDialogButtonBox, QFileDialog
from PyQt5.QtWidgets import QGraphicsScene,QListWidgetItem,QListView,QLineEdit,QWidget,QHBoxLayout,QPushButton,QSizePolicy
from PyQt5.QtWidgets import QFileDialog
from PyQt5.uic import loadUi
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.QtCore import QPersistentModelIndex
from QComboBoxLang import QComboBoxLang
from PyQt5.QtGui import QIcon


#cal_path
from PyQt5.QtCore import pyqtSignal
from icon_lib import icon_get
from cal_path import subtract_paths

import decimal

import i18n
_ = i18n.language.gettext


class icon_widget(QWidget):

	changed = pyqtSignal()

	def __init__(self):
		QWidget.__init__(self)
		self.hbox=QHBoxLayout()
		self.edit=QLineEdit()
		self.button=QLabel()
		self.button.setFixedSize(32, 32)


		self.hbox.addWidget(self.button)

		self.hbox.addWidget(self.edit)

		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.edit.setStyleSheet("QLineEdit { border: none }");


		self.edit.textChanged.connect(self.text_changed)
		self.setLayout(self.hbox)

	def text_changed(self):
		self.changed.emit()

	def callback_button_click(self):
		from gpvdm_open import gpvdm_open
		self.path="/gpvdmroot/icons/"
		dialog=gpvdm_open(self.path,act_as_browser=False)

		ret=dialog.exec_()
		if ret==QDialog.Accepted:
			file_name=dialog.get_filename()
			rel_path=subtract_paths(self.path,file_name)
			rel_path=rel_path.replace("\\", "/")
			self.setText(rel_path)
			self.changed.emit()

	def setText(self,text):
		self.edit.blockSignals(True)
		self.edit.setText(text)
		icon=icon_get(text)
		if icon!=False:
			self.button.setPixmap(icon.pixmap(QSize(32, 32)))
		self.edit.blockSignals(False)
	
	def text(self):
		return self.edit.text()
