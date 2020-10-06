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

## @package electrical
#  The main window to electrical properties of the device.
#

import os
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget

#python modules
import webbrowser

#windows
from tab import tab_class

from PyQt5.QtCore import pyqtSignal

from QWidgetSavePos import QWidgetSavePos

from help import help_window

from code_ctrl import enable_betafeatures

from dos_main import dos_main

articles = []
mesh_articles = []

class contacts_boundary(QWidgetSavePos):



	def __init__(self):
		QWidgetSavePos.__init__(self,"electrical")
		self.setFixedSize(1000, 600)
		self.setWindowIcon(icon_get("preferences-system"))

		self.setWindowTitle(_("Electrical parameter editor")+" (https://www.gpvdm.com)") 
		

		self.main_vbox = QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)


		self.undo = QAction(icon_get("help"), _("Help"), self)
		self.undo.setStatusTip(_("Help"))
		self.undo.triggered.connect(self.callback_help)
		toolbar.addAction(self.undo)

		self.main_vbox.addWidget(toolbar)

		self.notebook = QTabWidget()

		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		widget=tab_class("device.inp")
		self.notebook.addTab(widget,_("Device"))

						
		self.setLayout(self.main_vbox)

	def callback_help(self,widget):
		webbrowser.open('http://www.gpvdm.com/man/index.html')

	def update(self):
		for i in range(0,self.count()):
			w=self.widget(i)
			w.update()


