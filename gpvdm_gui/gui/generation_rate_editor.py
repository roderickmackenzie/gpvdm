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

## @package generation_rate_editor
#  Editor to set the Generation rate in each layer
#


import os
from str2bool import str2bool
from icon_lib import icon_get
from global_objects import global_object_get

#inp
from inp import inp_update_token_value

#epitaxy
from epitaxy import epitaxy_get_layers

#windows
from gpvdm_tab2 import gpvdm_tab2
from error_dlg import error_dlg

#qt
from PyQt5.QtCore import QSize
from PyQt5.QtWidgets import QWidget, QVBoxLayout , QDialog,QToolBar,QAction, QSizePolicy, QTableWidget, QTableWidgetItem,QAbstractItemView

from global_objects import global_object_run

from global_objects import global_isobject
from global_objects import global_object_get

from QComboBoxLang import QComboBoxLang

import i18n
_ = i18n.language.gettext


from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos
from epitaxy import get_epi

from error_dlg import error_dlg
from inp import inp
from cal_path import get_sim_path
from gpvdm_json import gpvdm_data

class generation_rate_editor(QWidgetSavePos):

	def cell_changed(self, y,x):
		data=gpvdm_data()
		epi=get_epi()

		for i in range(0,self.tab.rowCount()):
			uid=self.tab.get_value(i,2)
			obj=epi.find_object_by_id(uid)
			obj.Gnp=float(self.tab.get_value(i,1))

		data.save()

	def __init__(self):
		QWidgetSavePos.__init__(self,"generation_rate_editor")

		self.setWindowTitle(_("Generaton rate editor")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("layers"))
		self.resize(400,250)

		self.main_vbox=QVBoxLayout()

		self.tab = gpvdm_tab2()

		self.tab.verticalHeader().setVisible(False)
		self.create_model()

		self.tab.cellChanged.connect(self.cell_changed)
		self.main_vbox.addWidget(self.tab)

		self.setLayout(self.main_vbox)

	def create_model(self):
		epi=get_epi()
		self.tab.blockSignals(True)
		self.tab.clear()
		self.tab.setColumnCount(3)

		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Layer name"), _("Generation rate (m^{-3}s^{-1})"), _("json id")])
		self.tab.setColumnWidth(1, 250)
		self.tab.setColumnWidth(2, 10)

		for l in epi.layers:
			self.add_row(l.shape_name,l.Gnp,l.id)
			for s in l.shapes:
				self.add_row(s.shape_name,s.Gnp,s.id)


		self.tab.blockSignals(False)

	def add_row(self,name,Gnp,uid):

		self.tab.blockSignals(True)
		i=self.tab.rowCount()
		self.tab.insertRow ( i )
		item1 = QTableWidgetItem(str(name))
		self.tab.setItem(i,0,item1)

		item2 = QTableWidgetItem(str(Gnp))
		self.tab.setItem(i,1,item2)

		item2 = QTableWidgetItem(str(uid))
		self.tab.setItem(i,2,item2)

		self.tab.blockSignals(False)


