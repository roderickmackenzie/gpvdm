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
import sys

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QApplication,QLineEdit,QComboBox,QHBoxLayout,QPushButton,QLabel,QDialog,QVBoxLayout,QSizePolicy
from PyQt5.QtGui import QPainter,QIcon,QImage
from PyQt5.QtGui import QFont

from icon_lib import icon_get

from PyQt5.QtCore import QSize, Qt

import re
from error_dlg import error_dlg
from gui_util import yes_no_dlg
from lock import lock
from spinner import spinner

def isValidEmail(email):
	if len(email) > 7:
		if email.count("@")==1:
			return True
	return False

from lock import get_lock
from cal_path import gpvdm_paths
from json_base import json_base
from i18n import get_full_language

class register(QDialog):

	def callback_register(self):

		if isValidEmail(self.email0.text()) == False :
			error_dlg(self,_("This is not a valide e-mail address"))
			return

		if self.email0.text()!=self.email1.text():
			error_dlg(self,_("The e-mail addresses do not match."))
			return

		if self.first_name.text()=="":
			error_dlg(self,_("Please enter your first name."))
			return

		if self.surname.text()=="":
			error_dlg(self,_("Please enter your surname."))
			return


		if self.company.text()=="":
			error_dlg(self,_("Please enter your Company/University."))
			return

		if self.use_for.currentText()=="Choose option":
			error_dlg(self,_("Please enter what you plan on using gpvdm for."))
			return

		#QApplication.processEvents()
		#QApplication.processEvents()

		#self.spinner.show()
		#self.working.show()

		self.register.setEnabled(False)
		user_data=json_base("register")
		user_data.include_name=False
		user_data.var_list=[]
		user_data.var_list.append(["email",str(self.email0.text().encode('ascii', 'xmlcharrefreplace'))[2:-1]])
		user_data.var_list.append(["title",self.title.currentText()])
		user_data.var_list.append(["first_name",str(self.first_name.text().encode('ascii', 'xmlcharrefreplace'))[2:-1]])
		user_data.var_list.append(["surname",str(self.surname.text().encode('ascii', 'xmlcharrefreplace'))[2:-1]])
		user_data.var_list.append(["company",str(self.company.text().encode('ascii', 'xmlcharrefreplace'))[2:-1]])
		user_data.var_list.append(["use_for",self.use_for.currentText()])
		user_data.var_list.append(["lang",get_full_language()])
		user_data.var_list_build()

		ret=get_lock().register(user_data)
		if ret==False:
			if get_lock().error=="no_internet":
				error_dlg(self,_("I can't access the internet, or gpvdm.com is down."))
			
			if get_lock().error=="too_old":
				error_dlg(self,_("Your version of gpvdm is too old to register, please download the latest version."))

			return

		self.allow_exit=True

		self.accept()

	def __init__(self):
		QWidget.__init__(self)
		self.allow_exit=False
		self.setWindowIcon(icon_get("icon"))
		self.setWindowTitle(_("Registration window (www.gpvdm.com)")) 
		self.setWindowFlags(Qt.WindowStaysOnTopHint)
		vbox=QVBoxLayout()

		l=QLabel(_("Please register to use gpvdm. Thanks!"))
		l.setFont(QFont('SansSerif', 25))
		vbox.addWidget(l)

		hbox_widget=QWidget()
		hbox=QHBoxLayout()
		hbox_widget.setLayout(hbox)

		self.title = QComboBox()
		self.title.addItem("Dr.")
		self.title.addItem("Prof.")
		self.title.addItem("Mr.")
		self.title.addItem("Mrs.")
		self.title.addItem("Ms.")
		self.title.addItem("Other")
		hbox.addWidget(self.title)
		l=QLabel("<b>"+_("First name")+"</b>:")
		l.setFont(QFont('SansSerif', 14))
		hbox.addWidget(l)
		self.first_name = QLineEdit()
		hbox.addWidget(self.first_name)

		l=QLabel("<b>"+_("Surname")+"</b>:")
		l.setFont(QFont('SansSerif', 14))
		hbox.addWidget(l)
		self.surname = QLineEdit()
		hbox.addWidget(self.surname)

		vbox.addWidget(hbox_widget)

		#Company
		hbox_widget=QWidget()
		hbox=QHBoxLayout()
		hbox_widget.setLayout(hbox)
		l=QLabel("<b>"+_("Company/University")+"</b>:")
		l.setFont(QFont('SansSerif', 14))
		hbox.addWidget(l)
		self.company = QLineEdit()
		hbox.addWidget(self.company)
		vbox.addWidget(hbox_widget)

		#Email 1
		hbox_widget=QWidget()
		hbox=QHBoxLayout()
		hbox_widget.setLayout(hbox)
		l=QLabel("<b>"+_("E-mail")+"</b>:")
		l.setFont(QFont('SansSerif', 14))
		hbox.addWidget(l)
		self.email0 = QLineEdit()
		hbox.addWidget(self.email0)
		vbox.addWidget(hbox_widget)

		#Email 2
		hbox_widget=QWidget()
		hbox=QHBoxLayout()
		hbox_widget.setLayout(hbox)
		l=QLabel("<b>"+_("Confirm e-mail")+"</b>:")
		l.setFont(QFont('SansSerif', 14))
		hbox.addWidget(l)
		self.email1 = QLineEdit()
		hbox.addWidget(self.email1)
		vbox.addWidget(hbox_widget)

		#Use for
		hbox_widget=QWidget()
		hbox=QHBoxLayout()
		hbox_widget.setLayout(hbox)
		l=QLabel("<b>"+_("I am most interested in simulating:")+"</b>:")
		l.setFont(QFont('SansSerif', 14))
		hbox.addWidget(l)
		self.use_for = QComboBox()
		self.use_for.addItem("Choose option")
		self.use_for.addItem("Organic PV ")
		self.use_for.addItem("Perovskite PV")
		self.use_for.addItem("Other 3rd gen PV")
		self.use_for.addItem("2rd gen PV")
		self.use_for.addItem("1st gen PV")
		self.use_for.addItem("OLEDs")
		self.use_for.addItem("Micro optics")
		self.use_for.addItem("Ray tracing")
		self.use_for.addItem("FDTD")
		self.use_for.addItem("Thermal effects")
		self.use_for.addItem("Other")

		hbox.addWidget(self.use_for)
		vbox.addWidget(hbox_widget)

		button_box=QHBoxLayout()

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		button_box.addWidget(spacer)
		self.register=QPushButton("Register", self)
		self.register.clicked.connect(self.callback_register)
		button_box.addWidget(self.register)

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)
		vbox.addWidget(button_box_widget)

		self.setLayout(vbox)

		self.setMinimumWidth(400)


		if gpvdm_paths.am_i_rod()==True:
			self.first_name.setText("Rod")
			self.surname.setText("Rod")
			self.email0.setText("r.c.i.mackenzie@googlemail.com")
			self.email1.setText("r.c.i.mackenzie@googlemail.com")
			self.company.setText("my company")
			self.use_for.setCurrentIndex(1)


	def closeEvent(self, event):
		if self.allow_exit==False:
			response=yes_no_dlg(self,_("Gpvdm will not work until you register.  Would do you want to exit gpvdm?"))

			if response == True:
				sys.exit(0)

			event.ignore()
		else:
			event.accept()
		
	def run(self):
		return self.exec_()


