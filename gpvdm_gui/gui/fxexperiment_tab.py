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

## @package fxexperiment_tab
#  fx experiment tab widget
#

import os
from numpy import *
from fxexperiment_mesh_tab import fxexperiment_mesh_tab
from circuit import circuit

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QTabWidget,QTabWidget
from PyQt5.QtGui import QPainter,QIcon

from tab import tab_class
from css import css_apply
from gpvdm_json import gpvdm_data

class fxexperiment_tab(QTabWidget):

	def update(self):
		self.fxmesh.update()

	def image_save(self):
		self.fxmesh.image_save()

	def __init__(self,data):
		QTabWidget.__init__(self)
		css_apply(self ,"tab_default.css")
		self.data=data

		self.setMovable(True)

		#self.tmesh = tab_time_mesh(self.data)
		#self.addTab(self.tmesh,_("time mesh"))
		self.tmesh = fxexperiment_mesh_tab(self.data.id)
		self.addTab(self.tmesh,_("Frequency mesh"))

		self.config_tab=tab_class(self.data.config)
		self.addTab(self.config_tab,_("Configure"))

		if gpvdm_data().electrical_solver.solver_type!="circuit":
			self.circuit=circuit(self.data)
			self.addTab(self.circuit,_("Circuit"))
			self.circuit.load_type.changed.connect(self.config_tab.tab.hide_show_widgets)

	def rename(self,tab_name):
		self.data.english_name=tab_name
		gpvdm_data().save()

	def get_json_obj(self):
		return self.data


