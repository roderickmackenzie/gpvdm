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

## @package sim_warnings
#  The sim warnings window.
#
#qt
import os

from gui_enable import gui_get
import i18n
_ = i18n.language.gettext
if gui_get()==True:
	from PyQt5.QtWidgets import QWidget
	from PyQt5.uic import loadUi
	from cal_path import get_ui_path

	class sim_warnings(QWidget):

		def callback_close(self):
			self.window.hide()

		def __init__(self,text):
			QWidget.__init__(self)
			self.window = loadUi(os.path.join(get_ui_path(),"simulation_errors.ui"))
			self.window.text.setText(text)
			self.window.ok.clicked.connect(self.callback_close)
			self.window.show()
else:
	class sim_warnings():
		def __init__(self,text):
			print("sim warnings")
