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

## @package time_domain_experiment_tab
#  A tab to hold time domain experiments.
#


import os
from time_domain_mesh_tab import tab_time_mesh
from circuit import circuit
from tab import tab_class


import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTabWidget
from PyQt5.QtGui import QPainter,QIcon
from cal_path import get_sim_path
from inp import inp
from css import css_apply
from gpvdm_json import gpvdm_data

class time_domain_experiment_tab(QTabWidget):

	def update(self):
		self.tmesh.update()

	def image_save(self):
		self.tmesh.image_save()

	def __init__(self,data):
		QTabWidget.__init__(self)
		css_apply(self ,"tab_default.css")
		self.data=data
		
		self.setMovable(True)

		self.tmesh = tab_time_mesh(self.data.id)
		self.addTab(self.tmesh,_("time mesh"))

 
		if gpvdm_data().electrical_solver.solver_type!="circuit":
			self.circuit=circuit(self.data)
			self.addTab(self.circuit,_("Circuit"))

		tab=tab_class(self.data.config)
		self.addTab(tab,_("Configure"))


	def rename(self,tab_name):
		self.data.english_name=tab_name
		gpvdm_data().save()

	def get_json_obj(self):
		return self.data

