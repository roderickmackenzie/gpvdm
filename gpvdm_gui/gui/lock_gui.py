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
from PyQt5.QtCore import Qt 
from PyQt5.QtWidgets import QWidget,QDialog

from error_dlg import error_dlg
from lock import lock
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import pyqtSignal
from lock_register import register

from lock import get_lock
from lock_trial import lock_trial
from msg_dlg import msg_dlg

from cal_path import get_image_file_path
from help import help_window

class lock_gui(QWidget):
	#disable_all=pyqtSignal()
	#enable_all=pyqtSignal()


	def timer_callback(self):
		self.timer.stop()

		if get_lock().get_next_gui_action()=="register":
			self.register=register()
			ret=self.register.run()
			if ret==QDialog.Accepted:
				get_lock().check_license_thread()
				image_file=""
				from video import video
				self.v=video()
				self.v.show()
				#text="Thank you for registering gpvdm."

				#msgBox = msg_dlg()

				#msgBox.setText(text)


				#msgBox.exec_()
				#self.enable_all.emit()
				return
			else:
				return

		if get_lock().data.status=="expired":
			self.trial=lock_trial(override_text="<br><br>Thank you for using gpvdm.  Gpvdm can only continue to exist if users support it by buying licenses.  Your license has expired, please purchase a new one.  Thank you for using gpvdm.<br>",title_font_size=14)
			ret=self.trial.run()
			if ret==QDialog.Accepted:
				msgBox = msg_dlg()
				msgBox.setText("Thank you for buying gpvdm")
				reply = msgBox.exec_()
		#if get_lock().is_disabled()==True:
		#	self.disable_all.emit()

		get_lock().check_license_thread()

		
		if get_lock().get_next_gui_action()=="no_internet":
			msgBox = msg_dlg()
			msgBox.setText("I can not connect to the update server.  Gpvdm may not be able to run.  Please connect to the internet.")
			reply = msgBox.exec_()
			return

		if get_lock().data.update_available==True:
			help_window().help_append(["star.png",_("<big><b>Update available!</b></big><br>Download it now from <a href=\"www.gpvdm.com\">www.gpvdm.com</a>")])

		if get_lock().data.message!="":
			msgBox = msg_dlg()
			msgBox.setText(get_lock().data.message)
			msgBox.exec_()

	def __init__(self):
		QWidget.__init__(self)
		self.timer=QTimer()
		self.timer.timeout.connect(self.timer_callback)
		self.update_available=False

	def run(self):
		self.timer.start(1000)


