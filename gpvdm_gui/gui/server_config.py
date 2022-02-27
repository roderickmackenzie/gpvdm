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

from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos
from gpvdm_json import gpvdm_data
from help import QAction_help

class server_config(QWidget):

	def callback_tab_changed(self):
		self.changed.emit()

	def __init__(self):
		from server_cache_config import server_cache_config

		QWidgetSavePos.__init__(self,"server_config")
		
		self.setMinimumSize(900, 600)
		self.setWindowIcon(icon_get("cpu"))

		self.setWindowTitle(_("Configure hardware")+" (https://www.gpvdm.com)") 
		

		self.main_vbox = QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		toolbar.addWidget(spacer)


		self.help = QAction_help()
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)

		

		self.notebook = QTabWidget()

		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		data=gpvdm_data()

		tab=tab_class(data.server)
		self.notebook.addTab(tab,_("Server configuration"))

		self.cache_config=server_cache_config()
		self.notebook.addTab(self.cache_config,_("Cache"))
	
		self.setLayout(self.main_vbox)




