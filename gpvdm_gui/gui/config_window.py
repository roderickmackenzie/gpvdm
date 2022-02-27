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

## @package config_window
#  Configuration window.
#

import os
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget
from PyQt5.QtGui import QPainter,QIcon

#python modules
import webbrowser

#windows
from tab import tab_class

from PyQt5.QtCore import pyqtSignal
from global_objects import global_object_run

from inp import inp

from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos

from css import css_apply
from gpvdm_json import gpvdm_data
from help import QAction_help

class class_config_window(QWidgetSavePos):

	changed = pyqtSignal()

	def callback_tab_changed(self):
		self.changed.emit()
		global_object_run("ribbon_configure_dump_refresh")

	def __init__(self,files,description,title=_("Configure"),icon="preferences-system",data=gpvdm_data()):
		QWidgetSavePos.__init__(self,"config_window")
		self.data=data
		self.toolbar=QToolBar()
		self.toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.toolbar.setIconSize(QSize(48, 48))

		self.setFixedSize(900, 600)
		self.setWindowIcon(icon_get(icon))

		self.setWindowTitle(title+" (https://www.gpvdm.com)")

		self.main_vbox = QVBoxLayout()

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.toolbar.addWidget(spacer)

		self.help = QAction_help()
		self.toolbar.addAction(self.help)

		self.main_vbox.addWidget(self.toolbar)



		self.notebook = QTabWidget()
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		if (len(files)>0):
			for i in range(0,len(files)):
				file_name=files[i]
				tab=tab_class(file_name,data=self.data)
				self.notebook.addTab(tab,description[i])

		self.setLayout(self.main_vbox)




