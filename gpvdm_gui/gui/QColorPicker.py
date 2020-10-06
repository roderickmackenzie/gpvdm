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

## @package QColorPicker
#  A color picker widget
#



import os
from plot_io import get_plot_file_info

#qt
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QColorDialog,QGraphicsScene,QListWidgetItem,QListView,QLineEdit,QWidget,QHBoxLayout,QPushButton,QLineEdit
from PyQt5.QtGui import QPixmap

#cal_path
from cal_path import get_ui_path

from PyQt5.QtCore import pyqtSignal

from help import help_window

import i18n
_ = i18n.language.gettext


class QColorPicker(QWidget):

	changed = pyqtSignal()
	
	def __init__(self,r,g,b):
		QWidget.__init__(self)
		self.r=r
		self.g=g
		self.b=b
		self.hbox=QHBoxLayout()
		self.edit=QLineEdit()
		self.button=QPushButton()
		self.button.setFixedSize(25, 25)
		self.button.setText("...")
		self.hbox.addWidget(self.edit)
		self.hbox.addWidget(self.button)

		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.update_color()

		
		self.button.clicked.connect(self.callback_button_click)

		self.setLayout(self.hbox)

	def update_color(self):
		rgb=(self.r*255,self.g*255,self.b*244)
		self.edit.setStyleSheet("QLineEdit { border: none;  background-color: rgb(%d,%d,%d)  }" % rgb);
		
	def callback_button_click(self):
		col = QColorDialog.getColor(Qt.white, self, options=QColorDialog.DontUseNativeDialog)
		#col.setOption(QColorDialog.DontUseNativeDialog)
		if col.isValid():
			self.r=col.red()/255
			self.g=col.green()/255
			self.b=col.blue()/255
			self.update_color()
			self.changed.emit()

	def get_value(self):
		return self.edit.text()
		
