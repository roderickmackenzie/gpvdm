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

## @package ribbon_configure
#  The configure ribbon.
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

from config_window import class_config_window

from help import help_window

from global_objects import global_object_register
from emesh import tab_electrical_mesh

from gpvdm_open import gpvdm_open
from QAction_lock import QAction_lock
from gui_util import dlg_get_text
from inp import inp_get_token_value
from inp import inp_update_token_value

from QAction_lock import QAction_lock
from solar_spectrum_gen_window import solar_spectrum_gen_window

from inp import inp
from ver import ver_ctrl
from interface_editor import interface_editor

class ribbon_configure(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)
		self.config_window=None
		self.electrical_mesh=None
		self.electrical_interfaces=None

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))

		self.configwindow = QAction_lock("preferences-system", _("Configure"), self,"ribbon_config_config")
		self.configwindow.triggered.connect(self.callback_config_window)
		self.addAction(self.configwindow)

		self.dump = dump_io(self)
		global_object_register("ribbon_configure_dump_refresh",self.dump.refresh)
		self.addAction(self.dump)

		self.solar = QAction_lock("weather-few-clouds", _("Solar spectrum\ngenerator"), self,"solar_spectrum_tool")
		self.solar.clicked.connect(self.callback_solar)
		if ver_ctrl.is_gpvdm_next()==True:
			self.addAction(self.solar)


		self.mesh = QAction_lock("mesh", _("Electrical\nmesh"), self,"ribbon_config_mesh")
		self.mesh.triggered.connect(self.callback_edit_mesh)
		self.addAction(self.mesh)

		if ver_ctrl.is_gpvdm_next()==True:
			self.interfaces = QAction_lock("interfaces", _("Interfaces"), self,"ribbon_config_interfaces")
			self.interfaces.triggered.connect(self.callback_interfaces)
			self.addAction(self.interfaces)

	def update(self):
		if self.electrical_mesh!=None:
			self.electrical_mesh.update()

		if self.config_window!=None:
			del self.config_window
			self.config_window=None

	def setEnabled(self,val):
		self.configwindow.setEnabled(val)
		self.dump.setEnabled(val)
		self.mesh.setEnabled(val)

	def callback_config_window(self):

		self.config_window=gpvdm_open("/gpvdmroot/gpvdm_configure",show_inp_files=False,title=_("Configure"))
		self.config_window.toolbar.hide()
		self.config_window.show_directories=False
		ret=self.config_window.exec_()
		#self.config_window.changed.connect(self.dump.refresh)

		help_window().help_set_help(["preferences-system.png",_("<big><b>Configuration editor</b></big><br> Use this window to control advanced simulation parameters.")])


	def callback_solar(self):

		self.solar_window=solar_spectrum_gen_window()

		self.solar_window.show()


	def callback_edit_mesh(self):
		help_window().help_set_help(["mesh.png",_("<big><b>Mesh editor</b></big>\nUse this window to setup the mesh, the window can also be used to change the dimensionality of the simulation.")])

		if self.electrical_mesh==None:
			self.electrical_mesh=tab_electrical_mesh()
		if self.electrical_mesh.isVisible()==True:
			self.electrical_mesh.hide()
		else:
			self.electrical_mesh.show()

	def callback_interfaces(self):
		help_window().help_set_help(["interfaces.png",_("<big><b>Interface editor</b></big>\nUse this window to edit how electrical interfaces behave.")])

		if self.electrical_interfaces==None:
			self.electrical_interfaces=interface_editor()
		if self.electrical_interfaces.isVisible()==True:
			self.electrical_interfaces.hide()
		else:
			self.electrical_interfaces.show()



