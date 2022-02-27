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

## @package parasitic
#  Window to edit the parasitic components
#


import os
from tab import tab_class
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget
from PyQt5.QtGui import QPainter,QIcon


from QWidgetSavePos import QWidgetSavePos
from help import help_window
from cal_path import get_sim_path
from gpvdm_json import gpvdm_data
from help import QAction_help

class parasitic(QWidgetSavePos):

	def __init__(self):
		QWidgetSavePos.__init__(self,"parasitic")
		self.setFixedSize(900, 600)
		self.setWindowIcon(icon_get("parasitic"))

		self.setWindowTitle(_("Edit parasitic components")+"  (https://www.gpvdm.com)") 
		

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
		files=[data.parasitic]
		description=[_("Parasitic components")]


		for i in range(0,len(files)):
			tab=tab_class(files[i])
			self.notebook.addTab(tab,description[i])


		self.setLayout(self.main_vbox)
		
		gpvdm_data().add_call_back(self.update_values)
		self.destroyed.connect(self.doSomeDestruction)

	def doSomeDestruction(self):
		gpvdm_data().remove_call_back(self.update_values)

	def update_values(self):
		data=gpvdm_data()
		for i in range(0,self.notebook.count()):
			w=self.notebook.widget(i)
			w.tab.template_widget=data.parasitic
			w.tab.update_values()


