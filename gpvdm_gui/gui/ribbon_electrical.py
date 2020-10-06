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

## @package ribbon_electrical
#  The electrical ribbon.
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

from doping import doping_window
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
from file_watch import get_watch
from dos_main import dos_main

class ribbon_electrical(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)
		self.electrical_mesh=None
		self.doping_window=None
		self.electrical_editor=None

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))

		self.doping = QAction_lock("doping", _("Doping/\nIons"), self,"ribbon_device_doping")
		self.doping.clicked.connect(self.callback_doping)
		self.addAction(self.doping)

		self.mesh = QAction_lock("mesh", _("Electrical\nmesh"), self,"ribbon_config_mesh")
		self.mesh.triggered.connect(self.callback_edit_mesh)
		self.addAction(self.mesh)

		self.tb_electrical_editor = QAction_lock("electrical", _("Electrical\nparameters"), self,"ribbon_device_electrical")
		self.tb_electrical_editor.clicked.connect(self.callback_electrical_editor)
		self.addAction(self.tb_electrical_editor)

		get_watch().add_call_back("diagram.inp",self.callback_circuit_diagram)
		get_watch().add_call_back("math.inp",self.callback_circuit_diagram)
		self.callback_circuit_diagram()

	def update(self):
		if self.electrical_mesh!=None:
			self.electrical_mesh.update()

		if self.doping_window!=None:
			del self.doping_window
			self.doping_window=None

		if self.electrical_editor!=None:
			del self.electrical_editor
			self.electrical_editor=None

	def setEnabled(self,val):
		self.mesh.setEnabled(val)
		self.doping.setEnabled(val)
		self.tb_electrical_editor.setEnabled(val)

	def callback_edit_mesh(self):
		help_window().help_set_help(["mesh.png",_("<big><b>Mesh editor</b></big>\nUse this window to setup the mesh, the window can also be used to change the dimensionality of the simulation.")])

		if self.electrical_mesh==None:
			self.electrical_mesh=tab_electrical_mesh()
		if self.electrical_mesh.isVisible()==True:
			self.electrical_mesh.hide()
		else:
			self.electrical_mesh.show()

	def callback_doping(self):
		help_window().help_set_help(["doping.png",_("<big><b>Doping window</b></big>\nUse this window to add doping to the simulation")])

		if self.doping_window==None:
			self.doping_window=doping_window()

		if self.doping_window.isVisible()==True:
			self.doping_window.hide()
		else:
			self.doping_window.show()

	def callback_electrical_editor(self):
		help_window().help_set_help(["electrical.png",_("<big><b>Electrical parameters</b></big>\nUse this window to change the electrical parameters of each layer.")])

		if self.electrical_editor!=None:
			del self.electrical_editor

		self.electrical_editor=dos_main()
		self.electrical_editor.show()

	def callback_circuit_diagram(self):
		f=inp()
		f.load(os.path.join(get_sim_path(),"math.inp"))
		solver_name=f.get_token("#newton_name")

		if inp().isfile(os.path.join(get_sim_path(),"diagram.inp"))==True and solver_name=="newton_simple":
			self.tb_electrical_editor.setEnabled(False)
		else:
			self.tb_electrical_editor.setEnabled(True)

