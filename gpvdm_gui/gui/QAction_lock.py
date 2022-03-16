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

## @package ribbon_sim_mode
#  The sim mode ribbon.
#


import os
import sys
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QTextEdit, QAction
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QToolButton,QDialog

from PyQt5.QtCore import pyqtSignal
from icon_lib import icon_get

from msg_dlg import msg_dlg

from lock import get_lock
from lock_trial import lock_trial

class QAction_lock(QAction):
	clicked=pyqtSignal(QAction)

	def __init__(self,icon_name,text,s,id):
		sub_icon=None
		self.locked=False
		self.text=text
		
		if get_lock().is_function_locked(id)==True:
			self.locked=True

		if self.locked==True:
			sub_icon="lock"
		QAction.__init__(self,icon_get(icon_name,sub_icon=sub_icon), text, s)
		self.triggered.connect(self.callback_secure_click)

	def callback_secure_click(self):
		if self.locked==False:
			self.clicked.emit(self)
		else:
			self.setChecked(False)

			self.trial=lock_trial(override_text="<br><br><br><br>Upgrade today to gpvdm professional to use this function!.<br><br><br>",show_text=False,title_font_size=14)
			self.trial.title_text.setAlignment(Qt.AlignCenter)
			ret=self.trial.run()
			if ret==QDialog.Accepted:
				msgBox = msg_dlg()
				msgBox.setText("Thank you for buying gpvdm")
				reply = msgBox.exec_()


