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

## @package lock_trial
#  The trial window
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
from lock import lock

from lock import get_lock

import webbrowser
from lock_license_key import license_key
from error_dlg import error_dlg
from msg_dlg import msg_dlg

class lock_trial(QDialog):

	def callback_trial(self):
		webbrowser.open("https://www.gpvdm.com/buy.php")
		self.reject()

	def callback_close(self):
		self.reject()

	def callback_validate(self):
		val=get_lock().validate_key(self.keybox.text().strip())
		if val==True:
			self.accept()
			return
		elif get_lock().error=="notfound":
			error_dlg(self,_("Not a valid license number"))
		elif get_lock().error=="key_not_found":
			error_dlg(self,_("Key not found"))
		elif get_lock().error=="user_not_found":
			error_dlg(self,_("User not found"))
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


	def __init__(self,show_text=True,override_text=False,title_font_size=25):
		QWidget.__init__(self)
		self.setWindowIcon(icon_get("icon"))
		self.setWindowTitle(_("gpvdm trial (www.gpvdm.com)")) 
		self.setWindowFlags(Qt.WindowStaysOnTopHint)

		vbox=QVBoxLayout()

		if override_text!=False:
			text=override_text
		self.title_text=QLabel(text)
		self.title_text.setFont(QFont('SansSerif', title_font_size))
		self.title_text.setWordWrap(True)
		vbox.addWidget(self.title_text)

		#if show_text==True:
		#	l=QLabel(_("<br>Dear User, <br><br>Thank you for downloading and using gpvdm. I hope you find the software useful. Developing gpvdm takes a considerable amount of time and money.  So that I can continue to develop the model and make it available, I would ask you to buy license.  I will invest all money in the future development of the model.<br><br>  If you really can’t afford to buy a license, or wish to use the model in classroom teaching please contact me. Please also contact me if you are a long time collaborator of mine. <br><br>Many thanks,<br>Rod"))
		#	l.setWordWrap(True)
		#	l.setFont(QFont('SansSerif', 14))
		#	vbox.addWidget(l)



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

		self.buy=QPushButton("Get upgrade key", self)
		self.buy.clicked.connect(self.callback_trial)
		button_box.addWidget(self.buy)

		self.validate=QPushButton("Validate key", self)
		self.validate.clicked.connect(self.callback_validate)
		button_box.addWidget(self.validate)
		self.validate.hide()

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)
		vbox.addWidget(button_box_widget)

		self.setLayout(vbox)

		self.setMinimumWidth(600)

		
	def run(self):
		return self.exec_()


