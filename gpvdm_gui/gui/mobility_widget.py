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


## @package mobility_widget
#  A widget to adjust mobility
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
from cal_path import get_ui_path
from PyQt5.QtCore import pyqtSignal

import decimal

import i18n
_ = i18n.language.gettext


class mobility_widget(QWidget):

	changed = pyqtSignal()

	def __init__(self,electrons=True):
		QWidget.__init__(self)
		self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.raw_value="ground"
		self.hbox=QHBoxLayout()
		self.combobox = QComboBoxLang()
		self.combobox.setMaximumWidth( 100 )
		self.combobox.addItemLang("symmetric",_("Symmetric"))
		self.combobox.addItemLang("asymmetric",_("Asymmetric"))


		self.edit_z=QLineEdit()
		self.edit_z.setMaximumWidth( 90 )
		self.label_z=QLabel("z: ")
		self.label_z.setStyleSheet("QLabel { border: 0px; padding: 0px; }");
		self.label_z.setMaximumWidth( 10 )

		self.edit_x=QLineEdit()
		self.edit_x.setMaximumWidth( 90 )
		self.label_x=QLabel("x: ")
		self.label_x.setStyleSheet("QLabel { border: 0px; padding: 0px; }");
		self.label_x.setMaximumWidth( 10 )

		self.edit_y=QLineEdit()
		self.edit_y.setMaximumWidth( 90 )
		self.label_y=QLabel("y: ")
		self.label_y.setStyleSheet("QLabel { border: 0px; padding: 0px; }");
		self.label_y.setMaximumWidth( 10 )

		#self.button=QPushButton()
		#self.button.setFixedSize(25, 25)
		#self.button.setText("...")
		self.hbox.addWidget(self.label_z,Qt.AlignLeft)
		self.hbox.addWidget(self.edit_z,Qt.AlignLeft)
		self.hbox.addWidget(self.label_x,Qt.AlignLeft)
		self.hbox.addWidget(self.edit_x,Qt.AlignLeft)
		self.hbox.addWidget(self.label_y,Qt.AlignLeft)
		self.hbox.addWidget(self.edit_y,Qt.AlignLeft)
		self.hbox.addStretch()
		self.hbox.addWidget(self.combobox)

		self.hbox.setSpacing(0)
		#self.hbox.addWidget(self.button)

		self.edit_z.textChanged.connect(self.callback_edit)
		self.edit_x.textChanged.connect(self.callback_edit)
		self.edit_y.textChanged.connect(self.callback_edit)

		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.edit_z.setStyleSheet("QLineEdit { border: none }");
		self.edit_x.setStyleSheet("QLineEdit { border: none }");
		self.edit_y.setStyleSheet("QLineEdit { border: none }");


		#self.button.clicked.connect(self.callback_button_click)
		self.combobox.currentIndexChanged.connect(self.callback_combobox)
		#self.edit.textChanged.connect(self.callback_edit)
		#self.setStyleSheet("background-color:black;");
		self.setLayout(self.hbox)

	def update(self):
		if self.combobox.currentText_english()=="symmetric":
			self.edit_z.setVisible(False)
			self.label_z.setVisible(False)

			self.edit_x.setVisible(False)
			self.label_x.setVisible(False)

			self.label_y.setVisible(False)
			self.edit_y.setMaximumWidth( 120 )

		else:
			self.edit_z.setVisible(True)
			self.label_z.setVisible(True)

			self.edit_x.setVisible(True)
			self.label_x.setVisible(True)

			self.label_y.setVisible(True)
			self.edit_x.setMaximumWidth( 90 )
			self.edit_y.setMaximumWidth( 90 )
			self.edit_z.setMaximumWidth( 90 )

	def callback_edit(self):
		try:
			x=str(float(self.edit_z.text()))
			x=str(float(self.edit_x.text()))
			y=str(float(self.edit_y.text()))
			self.changed.emit()
		except:
			pass

	def callback_combobox(self):
		self.update()
		self.changed.emit()

	def from_si(self,value):
		return str(value)
		s=1.0/float(value)		#Ohms-1 m-1
		s=s/100					#Ohms-1 cm-1
		ret=("%.2e" % s).replace("e+0","e")
		return ret

	def to_si(self,value):
		return str(value)
		s=float(value)*100			#Ohms-1 cm-1
		s=1.0/s						#Ohms m
		return "%.2e" % s

	def set_values(self,values):
		self.combobox.setValue_using_english(values[0])

		try:
			self.edit_z.setText(self.from_si(values[1]))
			self.edit_x.setText(self.from_si(values[2]))
			self.edit_y.setText(self.from_si(values[3]))
		except:
			pass

		self.update()
	
	def get_values(self):
		return [self.combobox.currentText_english(),self.from_si(self.edit_z.text()),self.from_si(self.edit_x.text()),self.from_si(self.edit_y.text())]
		
