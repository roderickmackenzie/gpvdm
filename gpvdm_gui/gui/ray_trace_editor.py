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

## @package ray_trace_editor
#  An editor for the ray tracer.
#

import os
from tab import tab_class
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget
from PyQt5.QtGui import QPainter,QIcon

from help import help_window
from QWidgetSavePos import QWidgetSavePos
from css import css_apply
from gpvdm_json import gpvdm_data
from help import QAction_help

class ray_trace_editor(QWidgetSavePos):


	def __init__(self):
		QWidgetSavePos.__init__(self,"ray_trace")
		self.setFixedSize(900, 600)
		self.setWindowIcon(icon_get("ray"))

		self.setWindowTitle(_("Ray trace editor")+"  (https://www.gpvdm.com)") 
		

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
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		data=gpvdm_data()
		if len(data.ray.segments)>0:
			files=[data.ray.segments[0].config]
			description=[_("Ray trace")]


		for i in range(0,len(files)):
			tab=tab_class(files[i])
			self.notebook.addTab(tab,description[i])


		self.setLayout(self.main_vbox)
		


