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

## @package ribbon_device
#  The device editor ribbon.
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
from contacts import contacts_window
from cal_path import get_materials_path
from gpvdm_open import gpvdm_open

from help import help_window
from materials_main import materials_main

from parasitic import parasitic
from cal_path import get_spectra_path

from spectra_main import spectra_main
from layer_widget import layer_widget
from dim_editor import dim_editor

from dos_main import dos_main
from global_objects import global_object_register
from pl_main import pl_main
from QAction_lock import QAction_lock
from file_watch import get_watch

from cal_path import get_sim_path
from inp import inp

class ribbon_device(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)
		self.doping_window=None
		self.cost_window=None
		self.parasitic=None
		self.contacts_window=None
		self.parasitic_window=None
		self.layer_editor=None
		self.dim_editor=None
		self.electrical_editor=None
		self.emission_editor=None


		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setOrientation(Qt.Vertical);
		self.setIconSize(QSize(42, 42))

		self.tb_layer_editor = QAction_lock("layers", _("Layer\neditor"), self,"ribbon_device_layers")
		self.tb_layer_editor.clicked.connect(self.callback_layer_editor)
		self.addAction(self.tb_layer_editor)
		global_object_register("show_layer_editor",self.callback_layer_editor)
		
		self.contacts = QAction_lock("contact", _("Contacts"), self,"ribbon_device_contacts")
		self.contacts.clicked.connect(self.callback_contacts)
		self.addAction(self.contacts)
		
		self.doping = QAction_lock("doping", _("Doping/\nIons"), self,"ribbon_device_doping")
		self.doping.clicked.connect(self.callback_doping)
		self.addAction(self.doping)

		self.parasitic = QAction_lock("parasitic", _("Parasitic\n components"), self,"ribbon_device_parasitic")
		self.parasitic.clicked.connect(self.callback_parasitic)
		self.addAction(self.parasitic)

		self.tb_electrical_editor = QAction_lock("electrical", _("Electrical\nparameters"), self,"ribbon_device_electrical")
		self.tb_electrical_editor.clicked.connect(self.callback_electrical_editor)
		self.addAction(self.tb_electrical_editor)

		self.tb_emission_editor = QAction_lock("emission", _("Emission\nparameters"), self,"ribbon_device_emission")
		self.tb_emission_editor.clicked.connect(self.callback_emission_editor)
		self.addAction(self.tb_emission_editor)

		self.tb_dimension_editor = QAction_lock("dimensions", _("xz-size"), self,"ribbon_device_dim")
		self.tb_dimension_editor.clicked.connect(self.callback_dimension_editor)
		self.addAction(self.tb_dimension_editor)

		get_watch().add_call_back("diagram.inp",self.callback_circuit_diagram)
		self.callback_circuit_diagram()

	def callback_circuit_diagram(self):
		f=inp()
		f.load(os.path.join(get_sim_path(),"math.inp"))
		solver_name=f.get_token("#newton_name")
		if inp().isfile(os.path.join(get_sim_path(),"diagram.inp"))==True and solver_name=="newton_simple":
			self.tb_electrical_editor.setEnabled(False)
		else:
			self.tb_electrical_editor.setEnabled(True)


	def update(self):
		if self.cost_window!=None:
			del self.cost_window
			self.cost_window=None
			
		if self.doping_window!=None:
			del self.doping_window
			self.doping_window=None

		if self.contacts_window!=None:
			del self.contacts_window
			self.contacts_window=None

		if self.parasitic_window!=None:
			del self.parasitic_window
			self.parasitic_window=None

		if self.layer_editor!=None:
			del self.layer_editor
			self.layer_editor=None

		if self.dim_editor!=None:
			del self.dim_editor
			self.dim_editor=None

		if self.electrical_editor!=None:
			del self.electrical_editor
			self.electrical_editor=None

	def setEnabled(self,val):
		self.doping.setEnabled(val)
		self.cost.setEnabled(val)
		self.contacts.setEnabled(val)
		self.parasitic.setEnabled(val)
		self.tb_electrical_editor.setEnabled(val)
		
	def callback_doping(self):
		help_window().help_set_help(["doping.png",_("<big><b>Doping window</b></big>\nUse this window to add doping to the simulation")])

		if self.doping_window==None:
			self.doping_window=doping_window()

		if self.doping_window.isVisible()==True:
			self.doping_window.hide()
		else:
			self.doping_window.show()
		
	def callback_contacts(self):
		
		
		help_window().help_set_help(["contact.png",_("<big><b>Contacts window</b></big>\nUse this window to change the layout of the contacts on the device")])

		if self.contacts_window==None:
			self.contacts_window=contacts_window()
			
		if self.contacts_window.isVisible()==True:
			self.contacts_window.hide()
		else:
			self.contacts_window.show()


	def callback_parasitic(self):
		help_window().help_set_help(["parasitic.png",_("<big><b>Parasitic components</b></big>\nUse this window to edit the shunt and series resistance.")])

		if self.parasitic_window==None:
			self.parasitic_window=parasitic()

		if self.parasitic_window.isVisible()==True:
			self.parasitic_window.hide()
		else:
			self.parasitic_window.show()

	def callback_layer_editor(self):
		help_window().help_set_help(["layers.png",_("<big><b>Device layers</b></big>\nUse this window to configure the structure of the device.")])

		if self.layer_editor==None:
			self.layer_editor=layer_widget()

		if self.layer_editor.isVisible()==True:
			self.layer_editor.hide()
		else:
			self.layer_editor.show()

	def callback_dimension_editor(self):
		help_window().help_set_help(["dimension.png",_("<big><b>xz dimension editor</b></big>\nUse this window to configure the xz size of the device.")])

		if self.dim_editor==None:
			self.dim_editor=dim_editor()

		if self.dim_editor.isVisible()==True:
			self.dim_editor.hide()
		else:
			self.dim_editor.show()


	def callback_electrical_editor(self):
		help_window().help_set_help(["electrical.png",_("<big><b>Electrical parameters</b></big>\nUse this window to change the electrical parameters of each layer.")])

		if self.electrical_editor!=None:
			del self.electrical_editor

		self.electrical_editor=dos_main()
		self.electrical_editor.show()

	def callback_emission_editor(self):
		help_window().help_set_help(["emission.png",_("<big><b>Emission parameters</b></big>\nUse this window to set if a layer emits light or not.  You can choose between theoretically calculated emission spectra and imported experimental spectra.")])
			
		if self.emission_editor==None:
			self.emission_editor=pl_main()

		if self.emission_editor.isVisible()==True:
			self.emission_editor.hide()
		else:
			self.emission_editor.show()
