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

## @package thermal_main
#  The main thermal dialog.
#

import os
from tab_base import tab_base
from tab import tab_class
from global_objects import global_object_register
from epitaxy import get_epi

#qt5
from PyQt5.QtWidgets import  QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QTabWidget,QStatusBar

#windows
from global_objects import global_object_register
from icon_lib import icon_get

from css import css_apply

from inp import inp

from cal_path import get_sim_path

from gpvdm_json import gpvdm_data
import webbrowser
from cal_path import get_materials_path
from help import QAction_help

class thermal_main(QWidget,tab_base):

	def __init__(self):
		QWidget.__init__(self)
		self.setMinimumSize(1000, 600)

		self.main_vbox = QVBoxLayout()

		self.setWindowIcon(icon_get("thermal_kappa"))

		self.setWindowTitle(_("Thermal parameter editor")+" (https://www.gpvdm.com)") 

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		toolbar.addWidget(spacer)

		self.help = QAction_help()
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)

		self.notebook = QTabWidget()

		css_apply(self,"tab_default.css")

		self.main_vbox.addWidget(self.notebook)
		self.setLayout(self.main_vbox)

		self.status_bar=QStatusBar()

		self.main_vbox.addWidget(self.status_bar)	
		self.update()

		#gpvdm_data().add_call_back(self.update_values)
		#self.destroyed.connect(self.doSomeDestruction)

	#def doSomeDestruction(self):
	#	gpvdm_data().remove_call_back(self.update_values)

	#def update_values(self):
	#	epi=get_epi()
	#	for i in range(0,self.notebook.count()):
	#		w=self.notebook.widget(i)
	#		w.tab.template_widget=epi.find_object_by_id(w.tab.template_widget.id)
	#		w.tab.update_values()

	def update(self):
		self.notebook.clear()

		data=gpvdm_data()
		epi=get_epi()
		for l in epi.layers:
			if data.electrical_solver.solver_type!="circuit":
				name=l.shape_name
				#print(l.shape_dos)
				db_json_file=os.path.join(get_materials_path(),l.optical_material,"data.json")
				db_json_sub_path="thermal_constants"
				widget=tab_class(l.shape_heat,db_json_file=db_json_file,json_path=db_json_sub_path)
				self.notebook.addTab(widget,name)




