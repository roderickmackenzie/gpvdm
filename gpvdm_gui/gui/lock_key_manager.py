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

## @package trial
#  The trial window
#



import os

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QLineEdit,QComboBox,QHBoxLayout, QPushButton,QLabel, QDialog, QTabWidget,QTextBrowser, QVBoxLayout,QSizePolicy
from PyQt5.QtGui import QPainter,QIcon,QImage
from PyQt5.QtGui import QFont

from icon_lib import icon_get

from PyQt5.QtCore import QSize, Qt
import re

from error_dlg import error_dlg
from lock import lock

from lock import get_lock

import webbrowser
from license_key import license_key
from error_dlg import error_dlg
from msg_dlg import msg_dlg

class lock_key_manager(QDialog):

	def callback_trial(self):
		webbrowser.open("https://www.gpvdm.com/buy.php")
		#self.reject()

	def callback_close(self):
		self.reject()

	def callback_validate(self):
		val=get_lock().validate_key(self.keybox.text().strip())
		print(val)
		if val==True:
			self.accept()
			return
		elif get_lock().error=="notfound":
			error_dlg(self,_("Not a valid license number"))
		elif get_lock().error=="limreached":
			error_dlg(self,_("This key has been used too many times."))
		elif get_lock().error=="outoftime":
			error_dlg(self,_("This key has expired."))
		elif get_lock().error=="too_old":
			error_dlg(self,_("This version of gpvdm is too old to validate it's key, please download the latest version."))
		else:
			error_dlg(self,_("Can't access the internet"))

		self.reject()

	def callback_keybox_edit(self):
		if self.keybox.text()=="":
			self.buy.show()
			self.close_button.show()
			self.validate.hide()
		else:
			self.buy.hide()
			self.close_button.hide()
			self.validate.show()


	def __init__(self,show_text=True,override_text=False):
		QWidget.__init__(self)
		self.setWindowIcon(icon_get("icon"))
		self.setWindowTitle(_("Key manager (www.gpvdm.com)")) 
		self.setWindowFlags(Qt.WindowStaysOnTopHint)

		vbox=QVBoxLayout()

		if override_text!=False:
			text=override_text
		l=QLabel(text)
		l.setFont(QFont('SansSerif', 25))
		l.setWordWrap(True)
		vbox.addWidget(l)


		button_box=QHBoxLayout()

		l=QLabel("Key")
		l.setFont(QFont('SansSerif', 14))
		button_box.addWidget(l)

		self.keybox = QLineEdit()
		button_box.addWidget(self.keybox)
		self.keybox.textChanged.connect(self.callback_keybox_edit)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		button_box.addWidget(spacer)

		self.close_button=QPushButton("Close", self)
		self.close_button.clicked.connect(self.callback_close)
		button_box.addWidget(self.close_button)

		self.buy=QPushButton("Buy license key", self)
		self.buy.clicked.connect(self.callback_trial)
		button_box.addWidget(self.buy)

		self.validate=QPushButton("Validate key", self)
		self.validate.clicked.connect(self.callback_validate)
		button_box.addWidget(self.validate)
		self.validate.hide()

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)

		vbox.addWidget(button_box_widget)

		self.tab=QTabWidget()

		key_widget=QWidget()
		key_widget.setLayout(vbox)
		self.tab.addTab(key_widget,_("Key"))

		self.tab.setMinimumWidth(500)

		text=get_lock().html()


		main_vbox=QVBoxLayout()
		self.lock=QTextBrowser()
		self.lock.setText(text)
		self.tab.addTab(self.lock,_("Data"))


		main_vbox.addWidget(self.tab)
		
		self.setLayout(main_vbox)

		self.setMinimumWidth(600)

		
	def run(self):
		return self.exec_()


