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

## @package tab_light_src
#  A tab to hold diffent types of JV experiments.
#

import os
import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTabWidget
from PyQt5.QtGui import QPainter,QIcon
from tab import tab_class
from css import css_apply
from optics_sources_tab import optics_light_src
from optics_filters_tab import optics_filters_tab
from cal_path import get_sim_path
from gpvdm_json import gpvdm_data
from global_objects import global_object_run

class tab_light_src(QTabWidget):

	def get_json_obj(self):
		data=gpvdm_data()
		data_obj=data.light_sources.lights.find_object_by_id(self.uid)
		return data_obj

	def update(self):
		self.light_src.update()
		self.light_filters.update()

	def __init__(self,data):
		QTabWidget.__init__(self)
		css_apply(self ,"tab_default.css")
		self.uid=data.id

		self.setMovable(True)
		self.light_src=optics_light_src("gpvdm_data().light_sources.lights",self.uid,_("Light source (y0)"))
		self.addTab(self.light_src,_("Light source"))

		self.light_filters=optics_filters_tab("gpvdm_data().light_sources.lights",self.uid,_("Filters (y0)"))
		self.addTab(self.light_filters,_("Filters"))

		self.configure=tab_class(self.get_json_obj())
		self.addTab(self.configure,_("Configure"))
		self.configure.tab.changed.connect(self.callback_configure_changed)

	def rename(self,tab_name):
		self.get_json_obj().english_name=tab_name
		gpvdm_data().save()

	def callback_configure_changed(self,item):
		if item=="light_illuminate_from":
			global_object_run("gl_force_redraw")

