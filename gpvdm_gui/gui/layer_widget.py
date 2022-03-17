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

## @package layer_widget
#  The window to select and build the device structure.
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
from gpvdm_json import gpvdm_data


class layer_widget(QWidgetSavePos):

	
	def callback_tab_selection_changed(self):
		#self.tab_changed(0,0)
		self.emit_change()

	def emit_change(self):
		global_object_run("gl_force_redraw")
		
	def emit_structure_changed(self):
		epi=get_epi()
		self.save_model()
		global_object_run("dos_update")
		global_object_run("pl_update")
		global_object_run("interface_update")
		global_object_run("mesh_update")
		global_object_run("optics_force_redraw")
		global_object_run("gl_force_redraw")


	def __init__(self):
		QWidgetSavePos.__init__(self,"layer_widget")

		self.setWindowTitle(_("Layer editor")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("layers"))
		self.resize(800,500)

		self.cost_window=False

		self.main_vbox=QVBoxLayout()

		self.toolbar=QToolBar()

		self.tab2 = gpvdm_tab2(toolbar=self.toolbar)
		self.tab2.set_tokens(["shape_name","dy","optical_material","layer_type","solve_optical_problem","solve_thermal_problem","id"])
		self.tab2.set_labels([_("Layer name"), _("Thicknes")+" (m)", _("Optical material"), _("Layer type"), _("Solve optical\nproblem"), _("Solve thermal\nproblem"), _("ID")])
		self.tab2.json_search_path="gpvdm_data().epitaxy.layers"
		self.tab2.setColumnWidth(2, 250)
		self.tab2.setColumnWidth(6, 10)
		self.tab2.populate()
		self.tab2.new_row_clicked.connect(self.callback_new_row_clicked)
		self.tab2.changed.connect(self.emit_structure_changed)
		self.tab2.itemSelectionChanged.connect(self.layer_selection_changed)

		self.main_vbox.addWidget(self.toolbar)

		self.main_vbox.addWidget(self.tab2)
		self.setLayout(self.main_vbox)



	def callback_new_row_clicked(self,row):
		epi=get_epi()
		obj=epi.add_new_layer(pos=row)
		self.tab2.insert_row(obj,row)

		self.emit_structure_changed()

	def save_model(self):
		epi=get_epi()
		data=gpvdm_data()
		epi.symc_to_mesh(data.mesh.mesh_y)
		data.save()

	def layer_selection_changed(self):
		a=self.tab2.selectionModel().selectedRows()

		if len(a)>0:
			y=a[0].row()
			epi=get_epi()
			if global_isobject("display_set_selected_obj")==True:
				global_object_get("display_set_selected_obj")(epi.layers[y].id)
			global_object_run("gl_do_draw")



