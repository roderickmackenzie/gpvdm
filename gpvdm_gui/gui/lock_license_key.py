# -*- coding: utf-8 -*-
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

## @package register
#  Registration window
#


import os

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QLineEdit,QComboBox,QHBoxLayout,QPushButton,QLabel,QDialog,QVBoxLayout,QSizePolicy
from PyQt5.QtGui import QPainter,QIcon,QImage
from PyQt5.QtGui import QFont

from icon_lib import icon_get

from PyQt5.QtCore import QSize, Qt

import re

from error_dlg import error_dlg

from lock import get_lock

class license_key(QDialog):

	def callback_ok(self):
		print("boom")
		#get_lock().register(email=self.email0.text(),name=self.name.text())
		#get_lock().get_license()

		self.accept()

	def __init__(self):
		QWidget.__init__(self)
		self.setWindowIcon(icon_get("icon"))
		self.setWindowTitle(_("Registration window (www.gpvdm.com)")) 
		self.setWindowFlags(Qt.WindowStaysOnTopHint)

		vbox=QVBoxLayout()

		l=QLabel(_("Enter the license key below:"))
		l.setFont(QFont('SansSerif', 14))
		vbox.addWidget(l)

		hbox_widget=QWidget()
		hbox=QHBoxLayout()
		hbox_widget.setLayout(hbox)
		l=QLabel("<b>"+_("Key")+"</b>:")
		l.setFont(QFont('SansSerif', 14))
		hbox.addWidget(l)
		self.name = QLineEdit()
		hbox.addWidget(self.name)
		vbox.addWidget(hbox_widget)

		button_box=QHBoxLayout()

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		button_box.addWidget(spacer)

		self.register=QPushButton("Register", self)
		self.register.clicked.connect(self.callback_ok)
		button_box.addWidget(self.register)

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)
		vbox.addWidget(button_box_widget)

		self.setLayout(vbox)

		self.setMinimumWidth(400)

		self.name.setText("key")
		
		
	def run(self):
		return self.exec_()


