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

## @package contacts
#  Window to configure the contacts
#

import os
from icon_lib import icon_get
from epitaxy import get_epi

import i18n
_ = i18n.language.gettext


#qt
from PyQt5.QtWidgets import  QAction
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QToolBar, QMessageBox, QVBoxLayout, QTableWidget,QAbstractItemView, QTableWidgetItem

from PyQt5.QtCore import pyqtSignal

from error_dlg import error_dlg
from global_objects import global_object_run

from QWidgetSavePos import QWidgetSavePos
from gpvdm_tab2 import gpvdm_tab2

from energy_to_charge import energy_to_charge

from gpvdm_applied_voltage import gpvdm_applied_voltage
from gpvdm_json import gpvdm_data
from contacts_io import contact
from help import QAction_help

class contacts_window(QWidgetSavePos):

	changed = pyqtSignal()

	def show_hide_cols(self):
		schottky=False
		for c in gpvdm_data().epi.contacts.segments:
			if c.physical_model=="schottky":
				schottky=True
				break

		if schottky==True:
			self.tab.setColumnHidden(9,False)
			self.tab.setColumnHidden(10,False)
		else:
			self.tab.setColumnHidden(9,True)
			self.tab.setColumnHidden(10,True)

		if gpvdm_data().electrical_solver.solver_type=="circuit":
			self.tab.setColumnHidden(7,True)
			self.tab.setColumnHidden(8,True)
			self.tab.setColumnHidden(11,True)
		else:
			self.tab.setColumnHidden(7,False)
			self.tab.setColumnHidden(8,False)
			self.tab.setColumnHidden(11,False)

		if gpvdm_data().mesh.mesh_z.get_points()!=1 or gpvdm_data().mesh.mesh_x.get_points()!=1: 
			self.hide_cols(False)
		else:
			self.hide_cols(True)


	def save_and_redraw(self):
		self.changed.emit()
		gpvdm_data().save()
		global_object_run("gl_force_redraw")

	def update(self):
		#self.tab.clear()
		#self.tab.populate()
		self.show_hide_cols()

	def hide_cols(self,val):
		self.tab.setColumnHidden(3,val)
		self.tab.setColumnHidden(4,val)
		#self.tab.setColumnHidden(5,val)


	def __init__(self):
		QWidgetSavePos.__init__(self,"contacts")
		self.epi=get_epi()

		self.setMinimumSize(1000, 400)

		self.setWindowIcon(icon_get("contact"))

		self.setWindowTitle(_("Edit contacts")+" (www.gpvdm.com)") 
		
		self.main_vbox = QVBoxLayout()

		self.toolbar=QToolBar()
		self.toolbar.setIconSize(QSize(48, 48))
		self.main_vbox.addWidget(self.toolbar)

		self.tab = gpvdm_tab2(toolbar=self.toolbar)
		self.tab.set_tokens(["shape_name","position","applied_voltage","x0","dx","contact_resistance_sq","shunt_resistance_sq","np","charge_type", "ve0", "vh0", "physical_model","id"])
		self.tab.set_labels([_("Name"),_("Top/Bottom"),_("Applied\nvoltage"),_("Start")+" (m)", _("Width")+" (m)" , _("Contact resistance\n")+" (Ohms m^2)",_("Shunt resistance")+"\n(Ohms m^2)",_("Charge density/\nFermi-offset"),_("Majority\ncarrier"),_("ve0 (m/s)"),_("vh0 (m/s)"),_("Physical\nmodel"),_("ID")])

		self.tab.json_search_path="gpvdm_data().epitaxy.contacts.segments"
		self.tab.setColumnWidth(2, 200)
		self.tab.setColumnWidth(7, 200)
		self.tab.setColumnHidden(5,True)
		self.tab.setColumnHidden(6,True)
		self.tab.setColumnWidth(12, 10)

		self.tab.base_obj=contact()
		self.tab.populate()
		#self.tab.new_row_clicked.connect(self.callback_new_row_clicked)
		self.tab.changed.connect(self.emit_structure_changed)
		#self.tab.itemSelectionChanged.connect(self.save_and_redraw)



		self.main_vbox.addWidget(self.tab)

		self.update()

		self.setLayout(self.main_vbox)

		gpvdm_data().add_call_back(self.update)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.toolbar.addWidget(spacer)

		self.help = QAction_help()
		self.toolbar.addAction(self.help)

		#get_contactsio().changed.connect(self.update)

	def emit_structure_changed(self):
		self.show_hide_cols()
		gpvdm_data().save()
		self.changed.emit()
		global_object_run("gl_force_redraw")

