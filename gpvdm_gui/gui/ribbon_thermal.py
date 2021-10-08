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

## @package ribbon_thermal
#  The thermal ribbon.
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from help import help_window

from gui_util import dlg_get_text

from QAction_lock import QAction_lock
from thermal_isothermal_button import thermal_isothermal_button
from config_window import class_config_window

from gpvdm_json import gpvdm_data
from ribbon_page import ribbon_page

class ribbon_thermal(ribbon_page):
	def __init__(self):
		ribbon_page.__init__(self)
		self.enabled=False
		self.thermal_isothermal_button=thermal_isothermal_button(self)
		self.thermal_isothermal_button.triggered.connect(self.callback_thermal_isothermal_button)
		self.addAction(self.thermal_isothermal_button)

		self.addSeparator()

		self.temperature = QAction_lock("thermal", _("Set\nTemperature"), self,"ribbon_thermal_settemp")
		self.temperature.clicked.connect(self.callback_thermal)
		self.addAction(self.temperature)
		self.addSeparator()

		self.boundary = QAction_lock("boundary", _("Boundary\nConditions"), self,"ribbon_thermal_boundary")
		self.boundary.clicked.connect(self.callback_boundary)
		self.addAction(self.boundary)

		self.configure = QAction_lock("cog", _("Configure\nModel"), self,"ribbon_thermal_configure")
		self.configure.clicked.connect(self.callback_configure)
		self.addAction(self.configure)

		self.addSeparator()

		self.joule_heating = QAction_lock("joule_heating", _("Joule\nHeating"), self,"ribbon_thermal_joule")
		self.joule_heating.clicked.connect(self.callback_heating_click)
		self.joule_heating.setCheckable(True)
		self.addAction(self.joule_heating)

		self.parasitic_heating = QAction_lock("parasitic_heating", _("Parasitic\nHeating"), self,"ribbon_thermal_joule")
		self.parasitic_heating.clicked.connect(self.callback_heating_click)
		self.parasitic_heating.setCheckable(True)
		self.addAction(self.parasitic_heating)

		self.optical_heating = QAction_lock("optical_heating", _("Optical\nHeating"), self,"ribbon_thermal_optical")
		self.optical_heating.clicked.connect(self.callback_heating_click)
		self.optical_heating.setCheckable(True)
		self.addAction(self.optical_heating)

		self.recombination_heating = QAction_lock("recombination_heating", _("Recombination\nheating"), self,"ribbon_thermal_recombination")
		self.recombination_heating.clicked.connect(self.callback_heating_click)
		self.recombination_heating.setCheckable(True)
		self.addAction(self.recombination_heating)

		self.thermal_parameters = QAction_lock("thermal_kappa", _("Thermal\nparameters"), self,"ribbon_thermal_parameters")
		self.thermal_parameters.clicked.connect(self.callback_thermal_parameters)
		self.addAction(self.thermal_parameters)


	def callback_thermal_parameters(self):
		help_window().help_set_help(["thermal_kappa.png",_("<big><b>Thermal parameters</b></big>\nUse this window to change the thermal parameters of each layer.")])

		from thermal_main import thermal_main
		self.thermal_editor=thermal_main()
		self.thermal_editor.show()

	def callback_heating_click(self):
		data=gpvdm_data()
		data.thermal.joule_heating=self.joule_heating.isChecked()
		data.thermal.parasitic_heating=self.parasitic_heating.isChecked()
		data.thermal.optical_heating=self.optical_heating.isChecked()
		data.thermal.recombination_heating=self.recombination_heating.isChecked()
		data.save()

	def callback_thermal_isothermal_button(self):
		self.update()

	def callback_thermal(self):
		data=gpvdm_data()
		
		temp=data.thermal_boundary.Ty0

		new_temp=dlg_get_text( _("Enter the new temperature"), str(temp),"thermal.png")
		if new_temp.ret!=None:
			new_temp=float(new_temp.ret)
			data.thermal_boundary.Ty0=new_temp
			data.thermal_boundary.Ty1=new_temp
			data.thermal_boundary.Tx0=new_temp
			data.thermal_boundary.Tx1=new_temp
			data.thermal_boundary.Tz0=new_temp
			data.thermal_boundary.Tz1=new_temp

			for l in data.epi.layers:
				l.shape_dos.Tstart=new_temp
				l.shape_dos.Tstop=new_temp+5.0
				for s in l.shapes:
					s.shape_dos.Tstart=new_temp
					s.shape_dos.Tstop=new_temp+5.0

			data.save()

	def update(self):
		if self.enabled==True:
			data=gpvdm_data()
			self.thermal_isothermal_button.refresh()
			if 	self.thermal_isothermal_button.thermal==False:
				self.temperature.setEnabled(True)
				self.boundary.setEnabled(False)
				self.configure.setEnabled(False)
				self.joule_heating.setEnabled(False)
				self.parasitic_heating.setEnabled(False)
				self.optical_heating.setEnabled(False)
				self.recombination_heating.setEnabled(False)
				self.thermal_isothermal_button.setEnabled(True)
				self.thermal_parameters.setEnabled(False)
			else:
				self.temperature.setEnabled(False)
				self.boundary.setEnabled(True)
				self.configure.setEnabled(True)
				self.joule_heating.setEnabled(True)
				self.parasitic_heating.setEnabled(True)
				self.optical_heating.setEnabled(True)
				self.recombination_heating.setEnabled(True)
				self.thermal_isothermal_button.setEnabled(True)
				self.thermal_parameters.setEnabled(True)

			self.joule_heating.setChecked(data.thermal.joule_heating)
			self.parasitic_heating.setChecked(data.thermal.parasitic_heating)
			self.optical_heating.setChecked(data.thermal.optical_heating)
			self.recombination_heating.setChecked(data.thermal.recombination_heating)
		else:
			self.temperature.setEnabled(False)
			self.boundary.setEnabled(False)
			self.configure.setEnabled(False)
			self.joule_heating.setEnabled(False)
			self.parasitic_heating.setEnabled(False)
			self.optical_heating.setEnabled(False)
			self.recombination_heating.setEnabled(False)
			self.thermal_isothermal_button.setEnabled(False)
			self.thermal_parameters.setEnabled(False)

	def setEnabled(self,val):
		self.enabled=val
		self.update()

	def callback_boundary(self):
		data=gpvdm_data()
		self.config_window=class_config_window([data.thermal_boundary],[_("Thermal boundary conditions")],title=_("Thermal boundary conditions"),icon="thermal")
		self.config_window.show()

	def callback_configure(self):
		data=gpvdm_data()
		self.config_window=class_config_window([data.thermal],[_("Configure")],title=_("Configure thermal model"),icon="thermal")
		self.config_window.show()

