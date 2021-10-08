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

## @package thermal_isothermal_button
#  Select thermal/isothermal simulation
#

import os

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit

from icon_lib import icon_get
from str2bool import str2bool
from help import help_window
from gpvdm_json import gpvdm_data

from cal_path import get_sim_path
import sys

class thermal_isothermal_button(QAction):

	def set_state(self,val):
		data=gpvdm_data()
		data.thermal.thermal=val
		data.save()

	def update_ui(self,update_help):
		self.blockSignals(True)
		data=gpvdm_data()
		self.thermal=str2bool(data.thermal.thermal)

		if self.thermal==True:
			self.setIcon(icon_get("thermal-on"))
			self.setText(_("Thermal model\nenabled"))
			if update_help==True:
				help_window().help_set_help(["thermal-on.png",_("<big><b>Thermal solver switched on</b></big><br>The heat equation will be solved across the device")])

		if self.thermal==False:
			self.setIcon(icon_get("thermal-off"))
			self.setText(_("Iso-thermal\nmodel"))
			if update_help==True:
				help_window().help_set_help(["thermal-off.png",_("<big><b>Isothermal mode</b></big><br>A single temperature will be assumed across the entire device.")])
		self.blockSignals(False)

	def refresh(self):
		self.update_ui(False)

	def __init__(self,parent):
		self.thermal=False
		QAction.__init__(self,icon_get("thermal-off"), _("Isothermal"),parent)
		self.triggered.connect(self.callback_state_changed)
		self.update_ui(False)

	def callback_state_changed(self):
		if self.thermal==True:
			self.thermal=False
		else:
			self.thermal=True

		#print(self.thermal)
		self.set_state(self.thermal)
		self.update_ui(True)
