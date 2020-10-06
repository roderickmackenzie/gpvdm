# -*- coding: utf-8 -*-
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

## @package ribbon_thermal
#  The thermal ribbon.
#


import os
from icon_lib import icon_get

from dump_io import dump_io
from tb_item_sim_mode import tb_item_sim_mode
from tb_item_sun import tb_item_sun

from code_ctrl import enable_betafeatures
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from emesh import tab_electrical_mesh

from help import help_window

from gui_util import dlg_get_text
from inp import inp_get_token_value
from inp import inp_update_token_value

from QAction_lock import QAction_lock
from thermal_isothermal_button import thermal_isothermal_button
from config_window import class_config_window

from inp import inp
from str2bool import str2bool
from cal_path import get_sim_path

class ribbon_thermal(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)
		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))

		self.thermal_isothermal_button=thermal_isothermal_button(self)
		self.thermal_isothermal_button.triggered.connect(self.callback_thermal_isothermal_button)
		self.addAction(self.thermal_isothermal_button)

		self.addSeparator()

		self.temperature = QAction_lock("thermal", _("Set\nTemperature"), self,"ribbon_thermal_settemp")
		self.temperature.clicked.connect(self.callback_thermal)
		self.addAction(self.temperature)
		self.addSeparator()

		self.boundary = QAction_lock("boundary", _("boundary\nConditions"), self,"ribbon_thermal_boundary")
		self.boundary.clicked.connect(self.callback_boundary)
		self.addAction(self.boundary)

		self.configure = QAction_lock("cog", _("Configure\nModel"), self,"ribbon_thermal_configure")
		self.configure.clicked.connect(self.callback_configure)
		self.addAction(self.configure)

		self.addSeparator()

		self.joule_heating = QAction_lock("joule_heating", _("Joule\nHeating"), self,"ribbon_thermal_joule")
		self.joule_heating.clicked.connect(self.callback_joule_heating)
		self.joule_heating.setCheckable(True)
		self.addAction(self.joule_heating)

		self.optical_heating = QAction_lock("optical_heating", _("Optical\nHeating"), self,"ribbon_thermal_optical")
		self.optical_heating.clicked.connect(self.callback_optical_heating)
		self.optical_heating.setCheckable(True)
		self.addAction(self.optical_heating)

		self.recombination_heating = QAction_lock("recombination_heating", _("Recombination\nheating"), self,"ribbon_thermal_recombination")
		self.recombination_heating.clicked.connect(self.callback_recombination_heating)
		self.recombination_heating.setCheckable(True)
		self.addAction(self.recombination_heating)

	def callback_joule_heating(self):
		f=inp()
		f.load(os.path.join(get_sim_path(),"thermal.inp"))
		f.replace("#joule_heating",str(self.joule_heating.isChecked()))
		f.replace("#optical_heating",str(self.optical_heating.isChecked()))
		f.replace("#recombination_heating",str(self.recombination_heating.isChecked()))
		f.save()

	def callback_recombination_heating(self):
		pass

	def callback_optical_heating(self):
		pass

	def callback_thermal_isothermal_button(self):
		self.update()

	def callback_thermal(self):
		f=inp().load("thermal_boundary.inp")
		temp=f.get_token("#Ty0")
		new_temp=dlg_get_text( _("Enter the new temperature"), temp,"thermal.png")
		if new_temp.ret!=None:
			f.set_token("#Ty0",new_temp.ret)
			f.set_token("#Ty1",new_temp.ret)
			f.set_token("#Tx0",new_temp.ret)
			f.set_token("#Tx1",new_temp.ret)
			f.set_token("#Tz0",new_temp.ret)
			f.set_token("#Tz1",new_temp.ret)
			f.save()

	def update(self):
		self.thermal_isothermal_button.refresh()
		if 	self.thermal_isothermal_button.thermal==False:
			self.temperature.setEnabled(True)
			self.boundary.setEnabled(False)
			self.configure.setEnabled(False)
			self.joule_heating.setEnabled(False)
			self.optical_heating.setEnabled(False)
			self.recombination_heating.setEnabled(False)
		else:
			self.temperature.setEnabled(False)
			self.boundary.setEnabled(True)
			self.configure.setEnabled(True)
			self.joule_heating.setEnabled(True)
			self.optical_heating.setEnabled(True)
			self.recombination_heating.setEnabled(True)

		f=inp()
		f.load(os.path.join(get_sim_path(),"thermal.inp"))
		self.joule_heating.setChecked(str2bool(f.get_token("#joule_heating")))
		self.optical_heating.setChecked(str2bool(f.get_token("#optical_heating")))
		self.recombination_heating.setChecked(str2bool(f.get_token("#recombination_heating")))

	def setEnabled(self,val):
		self.temperature.setEnabled(val)
		self.boundary.setEnabled(val)
		self.configure.setEnabled(val)
		self.joule_heating.setEnabled(val)
		self.optical_heating.setEnabled(val)
		self.recombination_heating.setEnabled(val)

	def callback_boundary(self):
		self.config_window=class_config_window(title=_("Thermal boundary conditions"),icon="thermal")
		self.config_window.files=["thermal_boundary.inp"]
		self.config_window.description=[_("Thermal boundary conditions")]
		self.config_window.init()
		self.config_window.show()

	def callback_configure(self):
		self.config_window=class_config_window(title=_("Configure thermal model"),icon="thermal")
		self.config_window.files=["thermal.inp"]
		self.config_window.description=[_("Configure")]
		self.config_window.init()
		self.config_window.show()

