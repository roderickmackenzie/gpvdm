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

## @package layer_widget
#  The window to select and build the device structure.
#


import os
from str2bool import str2bool
from icon_lib import icon_get
from cal_path import get_materials_path
from global_objects import global_object_get

#inp
from inp import inp_update_token_value

#epitaxy
from epitaxy import epitaxy_get_layers

#windows
from gpvdm_tab import gpvdm_tab
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


from gpvdm_select_material import gpvdm_select_material

from code_ctrl import enable_betafeatures
from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos

from epitaxy_mesh_update import epitaxy_mesh_update
from epitaxy import get_epi

from error_dlg import error_dlg

#from inp import inp_callback_add_write_hook



class layer_widget(QWidgetSavePos):

	
	def callback_tab_selection_changed(self):
		#self.tab_changed(0,0)
		self.emit_change()

	def cell_changed(self, y,x):
		epi=get_epi()

		if x==0:
			epi.layers[y].name=self.tab.get_value(y,x)
		elif x==1:
			ret=epi.layers[y].set_dy(self.tab.get_value(y,x))
			if ret==False:
				error_dlg(self,_("You have entered a non numeric value."))
		self.save_model()
		self.emit_structure_changed()
		
	def emit_change(self):
		global_object_run("gl_force_redraw")
		
	def emit_structure_changed(self):		#This will emit when there has been an edit
		global_object_run("mesh_update")
		global_object_run("optics_force_redraw")
		global_object_run("gl_force_redraw")

	def layer_type_edit(self):
		self.tab.blockSignals(True)

		epi=get_epi()
		for i in range(0,self.tab.rowCount()):
			epi.update_layer_type(i,self.tab.get_value(i,3).lower())
			self.tab.set_value(i,4,epi.layers[i].dos_file)
			self.tab.set_value(i,5,epi.layers[i].pl_file)
			self.tab.set_value(i,6,epi.layers[i].homo_file)
			self.tab.set_value(i,7,epi.layers[i].lumo_file)

		self.tab.blockSignals(False)

		self.save_model()
		self.emit_change()
		global_object_run("dos_update")
		global_object_run("pl_update")
		global_object_run("interface_update")

	def on_move_down(self):
		layer=self.tab.move_down()
		epi=get_epi()
		epi.move_down(layer)

		self.save_model()
		self.emit_change()
		self.emit_structure_changed()

	def on_move_up(self):
		layer=self.tab.move_up()
		epi=get_epi()
		epi.move_up(layer)
		self.save_model()
		self.emit_change()
		self.emit_structure_changed()

	def __init__(self):
		QWidgetSavePos.__init__(self,"layer_widget")

		self.setWindowTitle(_("Layer editor")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("layers"))
		self.resize(800,500)

		self.cost_window=False

		self.main_vbox=QVBoxLayout()

		self.toolbar=QToolBar()

		self.tab = gpvdm_tab(toolbar=self.toolbar)


		self.tab.tb_add.triggered.connect(self.on_add_item_clicked)
		self.tab.user_remove_rows.connect(self.on_remove_item_clicked)
		self.tab.tb_down.triggered.connect(self.on_move_down)
		self.tab.tb_up.triggered.connect(self.on_move_up)


		
		self.main_vbox.addWidget(self.toolbar)
	

		#self.tab.resizeColumnsToContents()


		self.tab.verticalHeader().setVisible(False)
		self.create_model()

		self.tab.cellChanged.connect(self.cell_changed)
		#self.tab.itemSelectionChanged.connect(self.callback_tab_selection_changed)
		self.main_vbox.addWidget(self.tab)

		self.setLayout(self.main_vbox)

		self.tab.itemSelectionChanged.connect(self.layer_selection_changed)

		#inp_callback_add_write_hook(os.path.join(get_sim_path(),"epitaxy.inp"),self.create_model,"layer_widget")

	def create_model(self):
		self.tab.blockSignals(True)
		self.tab.clear()
		self.tab.setColumnCount(11)
		#if enable_betafeatures()==False:
		#	self.tab.setColumnHidden(4, True)
		#	self.tab.setColumnHidden(5, True)

		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Layer name"), _("Thicknes"), _("Optical material"), _("Layer type"), _("DoS\nfile"), _("PL\nfile"), _("LUMO\nfile"), _("HOMO\nfile"), _("Solve optical\nproblem"), _("Solve thermal\nproblem"), _("ID")])
		self.tab.setColumnWidth(2, 250)
		self.tab.setColumnWidth(4, 80)
		self.tab.setColumnWidth(5, 80)
		self.tab.setColumnWidth(6, 80)
		self.tab.setColumnWidth(7, 80)
		self.tab.setColumnWidth(10, 10)

		self.tab.horizontalHeader().setFixedHeight(40)
		self.tab.setRowCount(epitaxy_get_layers())

		epi=get_epi()
		i=0
		for l in epi.layers:
			self.add_row(i,l)
			i=i+1

		self.tab.blockSignals(False)

	def add_row(self,i,layer):
		#l.dy,l.optical_material,l.dos_file,l.pl_file,l.name,l.lumo_file,l.homo_file,l.solve_optical_problem,l.solve_thermal_problem,l.id
		self.tab.blockSignals(True)
		
		#dos_file=""
		
		#if layer.dos_file.startswith("dos")==True:
		#	dos_file="active"
		#else:
		#	dos_file=""

		item1 = QTableWidgetItem(str(layer.name))
		self.tab.setItem(i,0,item1)

		item2 = QTableWidgetItem(str(layer.dy))
		self.tab.setItem(i,1,item2)


		combobox = gpvdm_select_material()
		combobox.setText(layer.optical_material)
		combobox.changed.connect(self.callback_material_select)
		
		self.tab.setCellWidget(i,2, combobox)

		combobox_layer_type = QComboBoxLang()

		combobox_layer_type.addItemLang("contact",_("contact"))
		combobox_layer_type.addItemLang("active",_("active layer"))
		combobox_layer_type.addItemLang("other",_("other"))

		self.tab.setCellWidget(i,3, combobox_layer_type)
		combobox_layer_type.setValue_using_english(str(layer.layer_type).lower())
		combobox_layer_type.currentIndexChanged.connect(self.layer_type_edit)

		item3 = QTableWidgetItem(str(layer.dos_file))
		self.tab.setItem(i,4,item3)

		item3 = QTableWidgetItem(str(layer.pl_file))
		self.tab.setItem(i,5,item3)

		item6 = QTableWidgetItem(str(layer.lumo_file))
		self.tab.setItem(i,6,item6)

		item7 = QTableWidgetItem(str(layer.homo_file))
		self.tab.setItem(i,7,item7)


		combobox = QComboBoxLang()

		combobox.addItemLang("true",_("Yes"))
		combobox.addItemLang("false",_("No"))

		self.tab.setCellWidget(i,8, combobox)
		combobox.setValue_using_english(str(layer.solve_optical_problem).lower())
		combobox.currentIndexChanged.connect(self.callback_model_select)

		combobox = QComboBoxLang()

		combobox.addItemLang("true",_("Yes"))
		combobox.addItemLang("false",_("No"))

		self.tab.setCellWidget(i,9, combobox)
		combobox.setValue_using_english(str(layer.solve_thermal_problem).lower())
		combobox.currentIndexChanged.connect(self.callback_model_select)

		item10 = QTableWidgetItem(layer.id)
		self.tab.setItem(i,10,item10)

		self.tab.blockSignals(False)

	def callback_material_select(self):
		epi=get_epi()
		for i in range(0,self.tab.rowCount()):
			epi.layers[i].optical_material=self.tab.cellWidget(i, 2).text()
			epi.layers[i].cal_rgb()

		self.emit_structure_changed()
		self.save_model()


	def callback_model_select(self):
		epi=get_epi()
		for i in range(0,self.tab.rowCount()):
			epi.layers[i].solve_optical_problem=str2bool(self.tab.get_value(i, 8))
			epi.layers[i].solve_thermal_problem=str2bool(self.tab.get_value(i, 9))
			#print(epi.layers[i].solve_optical_probelm,epi.layers[i].solve_thermal_probelm)
		#self.emit_structure_changed()

		self.save_model()


	def on_remove_item_clicked(self):
		ids=[]
		rows=self.tab.selectionModel().selectedRows()

		for r in rows:
			ids.append(self.tab.get_value(r.row(), 10))

		items=self.tab.remove()
		epi=get_epi()
		print(ids)
		epi.remove_by_id(ids)
		epi.save()
		epi.clean_unused_files()
		global_object_run("gl_force_redraw")

	def on_add_item_clicked(self):
		row=self.tab.insert_row()
		epi=get_epi()
		a=epi.add_new_layer(pos=row)
		self.add_row(row,a)
		epi.update_layer_type(row,self.tab.get_value(row,3).lower())
		epi.save()
		#self.emit_change()
		return

	def save_model(self):
		epi=get_epi()
		epi.save()

		epitaxy_mesh_update()

	def layer_selection_changed(self):
		a=self.tab.selectionModel().selectedRows()

		if len(a)>0:
			y=a[0].row()
			y="layer:"+str(self.tab.get_value(y, 0))
		else:
			y=-1
		
		if global_isobject("display_set_selected_layer")==True:
			global_object_get("display_set_selected_layer")(y)
		global_object_run("gl_force_redraw")

		#self.three_d.set_selected_layer(y)
		#self.three_d.update()
