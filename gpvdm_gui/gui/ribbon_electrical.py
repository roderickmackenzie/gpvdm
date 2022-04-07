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

## @package ribbon_electrical
#  The configure ribbon.
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QMenu, QToolButton
from PyQt5.QtWidgets import QTabWidget

from help import help_window

from global_objects import global_object_register

from gpvdm_open import gpvdm_open
from QAction_lock import QAction_lock
from gui_util import dlg_get_text

from lock import get_lock
from gpvdm_json import gpvdm_data
from cal_path import gpvdm_paths
from tb_item_solvers import tb_item_solvers
from ribbon_page import ribbon_page
from config_window import class_config_window

class ribbon_page2(QWidget):
	def __init__(self):
		QWidget.__init__(self)
		
		self.hbox = QHBoxLayout()
		self.hbox.setSpacing(0)
		self.hbox.setContentsMargins(0, 0, 0, 0)
		self.setLayout(self.hbox)
		#self.setStyleSheet(" padding-left: 0px; padding-right: 0px; padding-top: 0px;padding-bottom: 0px; margin: 0; border 0px;")

	def add_panel(self):
		t=QToolBar()
		t.setIconSize(QSize(42, 42))
		t.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		t.setStyleSheet(" padding-left: 0px; padding-right: 0px; padding-top: 0px;padding-bottom: 0px; margin: 0;")
		self.hbox.addWidget(t)
		return t

class ribbon_electrical(ribbon_page2):
	def __init__(self):
		ribbon_page2.__init__(self)
		self.enabled=False
		self.config_window=None
		self.electrical_mesh=None
		self.electrical_interfaces=None
		self.doping_window=None
		self.parasitic=None
		self.parasitic_window=None
		


		pan=self.add_panel()
		self.mesh = QAction_lock("mesh", _("Electrical\nmesh"), self,"ribbon_config_mesh")
		self.mesh.triggered.connect(self.callback_edit_mesh)
		pan.addAction(self.mesh)

		self.doping = QAction_lock("doping", _("Doping/\nIons"), self,"ribbon_device_doping")
		self.doping.clicked.connect(self.callback_doping)
		pan.addAction(self.doping)

		self.interfaces = QAction_lock("interfaces", _("Interfaces"), self,"ribbon_config_interfaces")
		self.interfaces.triggered.connect(self.callback_interfaces)
		pan.addAction(self.interfaces)

		self.boundary = QAction_lock("boundary", _("Boundary\nConditions"), self,"ribbon_electrical_boundary")
		self.boundary.clicked.connect(self.callback_boundary)
		pan.addAction(self.boundary)

		self.parasitic = QAction_lock("parasitic", _("Parasitic\n components"), self,"ribbon_device_parasitic")
		self.parasitic.clicked.connect(self.callback_parasitic)
		pan.addAction(self.parasitic)

		self.tb_solvers=tb_item_solvers()
		pan.addWidget(self.tb_solvers)
		self.tb_solvers.changed.connect(self.update_buttons)

		self.perovskite = QAction_lock("perovskite", _("Mobile\nIon solver"), self,"ribbon_device_mobile_ion")
		self.perovskite.clicked.connect(self.callback_perovskite)
		self.perovskite.setCheckable(True)
		pan.addAction(self.perovskite)

		self.singlet = QAction_lock("singlet", _("Excited\nstates"), self,"ribbon_device_mobile_singlet")
		self.singlet.clicked.connect(self.callback_singlet)
		self.singlet.setCheckable(True)
		pan.addAction(self.singlet)

		#a.setStyleSheet("QToolBar {margin-top: 0px;margin-bottom: 0px; padding 0px;}")
#		spacer = QWidget()
#		spacer.setMinimumSize(100,20)
#		self.addWidget(spacer)


	def update(self):
		if self.electrical_mesh!=None:
			self.electrical_mesh.update()

		if self.config_window!=None:
			del self.config_window
			self.config_window=None

		if self.doping_window!=None:
			del self.doping_window
			self.doping_window=None

		if self.parasitic_window!=None:
			del self.parasitic_window
			self.parasitic_window=None

		self.tb_solvers.update()
		self.update_buttons()

	def update_buttons(self):
		data=gpvdm_data()
		if self.enabled==True:
			if data.electrical_solver.solver_type=="drift-diffusion":
				self.doping.setEnabled(True)
				self.interfaces.setEnabled(True)
				self.parasitic.setEnabled(True)
				self.boundary.setEnabled(True)
				self.perovskite.setEnabled(True)
				self.singlet.setEnabled(True)
			elif data.electrical_solver.solver_type=="poisson":
				self.doping.setEnabled(True)
				self.interfaces.setEnabled(False)
				self.parasitic.setEnabled(False)
				self.boundary.setEnabled(True)
				self.perovskite.setEnabled(False)
				self.singlet.setEnabled(False)
			else:
				self.doping.setEnabled(False)
				self.interfaces.setEnabled(False)
				self.parasitic.setEnabled(False)
				self.boundary.setEnabled(False)
				self.perovskite.setEnabled(False)
				self.singlet.setEnabled(False)

		self.perovskite.setChecked(data.perovskite.perovskite_enabled)
		self.singlet.setChecked(data.singlet.singlet_enabled)

	def callback_perovskite(self):
		data=gpvdm_data()
		data.perovskite.perovskite_enabled=self.perovskite.isChecked()
		data.save()

	def callback_singlet(self):
		data=gpvdm_data()
		data.singlet.singlet_enabled=self.singlet.isChecked()
		data.save()

	def setEnabled(self,val):
		self.enabled=val
		self.mesh.setEnabled(val)
		self.doping.setEnabled(val)
		self.interfaces.setEnabled(val)
		self.parasitic.setEnabled(val)
		self.tb_solvers.setEnabled(val)
		self.boundary.setEnabled(val)
		self.perovskite.setEnabled(val)
		self.singlet.setEnabled(val)
		self.update_buttons()



	def callback_edit_mesh(self):
		from electrical_mesh_editor import electrical_mesh_editor
		help_window().help_set_help(["mesh.png",_("<big><b>Mesh editor</b></big>\nUse this window to setup the mesh, the window can also be used to change the dimensionality of the simulation.")])

		if self.electrical_mesh==None:
			self.electrical_mesh=electrical_mesh_editor()
		if self.electrical_mesh.isVisible()==True:
			self.electrical_mesh.hide()
		else:
			self.electrical_mesh.show()

	def callback_interfaces(self):
		from interface_editor import interface_editor
		help_window().help_set_help(["interfaces.png",_("<big><b>Interface editor</b></big>\nUse this window to edit how electrical interfaces behave.")])

		if self.electrical_interfaces==None:
			self.electrical_interfaces=interface_editor()
		if self.electrical_interfaces.isVisible()==True:
			self.electrical_interfaces.hide()
		else:
			self.electrical_interfaces.show()

	def callback_boundary(self):
		data=gpvdm_data()
		self.config_window=class_config_window([data.electrical_solver.boundary,data.exciton_boundary],[_("Electrical boundary conditions"),_("Excitonic boundary conditions")],title=_("Electrical boundary conditions"),icon="electrical")
		self.config_window.show()

	def callback_doping(self):
		from doping import doping_window
		help_window().help_set_help(["doping.png",_("<big><b>Doping window</b></big>\nUse this window to add doping to the simulation")])

		if self.doping_window==None:
			self.doping_window=doping_window()

		if self.doping_window.isVisible()==True:
			self.doping_window.hide()
		else:
			self.doping_window.show()

	def callback_parasitic(self):
		from parasitic import parasitic
		help_window().help_set_help(["parasitic.png",_("<big><b>Parasitic components</b></big>\nUse this window to edit the shunt and series resistance.")])

		if self.parasitic_window==None:
			self.parasitic_window=parasitic()

		if self.parasitic_window.isVisible()==True:
			self.parasitic_window.hide()
		else:
			self.parasitic_window.show()


