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

## @package contacts
#  Window to configure the contacts
#

import os
from numpy import *
import webbrowser
from inp import inp_search_token_value
from icon_lib import icon_get
from epitaxy import get_epi

import i18n
_ = i18n.language.gettext


#qt
from PyQt5.QtWidgets import QMainWindow, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QDialog,QFileDialog,QToolBar, QMessageBox, QVBoxLayout, QGroupBox, QTableWidget,QAbstractItemView, QTableWidgetItem, QComboBox

from PyQt5.QtCore import pyqtSignal


from str2bool import str2bool
from error_dlg import error_dlg

from global_objects import global_object_run

from QComboBoxLang import QComboBoxLang

from QWidgetSavePos import QWidgetSavePos

from contacts_boundary import contacts_boundary
from gpvdm_tab import gpvdm_tab

#from file_watch import get_watch

from mesh import get_mesh
from energy_to_charge import energy_to_charge

from gpvdm_select_material import gpvdm_select_material
from gpvdm_applied_voltage import gpvdm_applied_voltage
from gpvdm_json import gpvdm_data

class contacts_window(QWidgetSavePos):


	changed = pyqtSignal()

	#def callback_contacts_boundary(self):
	#	self.charge_density_window=contacts_boundary()
	#	self.charge_density_window.show()

	def update_contact_db(self):
		#first_contact
		#print("oh")
		for i in range(0,self.tab.rowCount()):
			try:
				float(self.tab.get_value(i, 3))
				float(self.tab.get_value(i, 4))
				float(self.tab.get_value(i, 5))
				#float(self.tab.get_value(i, 6))
				#float(self.tab.get_value(i, 7))
			except:
				return False

		if self.tab.rowCount()!=len(self.contacts.segments):
			print("don't match")
			return False

		for i in range(0,self.tab.rowCount()):
			self.contacts.segments[i].shape_name=self.tab.get_value(i, 0)
			self.contacts.segments[i].position=self.tab.get_value(i, 1)
			self.contacts.segments[i].applied_voltage_type=self.tab.get_value(i, 2).split(":")[0]
			self.contacts.segments[i].applied_voltage=self.tab.get_value(i, 2).split(":")[1]
			#print(self.tab.get_value(i, 2))

			if self.contacts.segments[i].position=="top" or self.contacts.segments[i].position=="bottom":
				self.contacts.segments[i].x0=float(self.tab.get_value(i, 3))
				self.contacts.segments[i].dx=float(self.tab.get_value(i, 4))
			else:
				self.contacts.segments[i].y0=float(self.tab.get_value(i, 3))
				self.contacts.segments[i].dy=float(self.tab.get_value(i, 4))

			self.contacts.segments[i].contact_resistance_sq=self.tab.get_value(i, 5)
			self.contacts.segments[i].shunt_resistance_sq=self.tab.get_value(i, 6)
			#self.contacts.segments[i].np=float(self.tab.get_value(i, 7))
			self.contacts.segments[i].charge_type=self.tab.get_value(i, 8)


			self.contacts.segments[i].ve0=self.tab.get_value(i, 9)
			self.contacts.segments[i].vh0=self.tab.get_value(i, 10)
			self.contacts.segments[i].physical_model=self.tab.get_value(i, 11)

		return True

	def on_move_down(self):
		self.tab.move_down()
		self.save()

	def on_move_up(self):
		self.tab.move_up()
		self.save()

	def set_row(self,pos,name,position,applied_voltage_type, applied_voltage, start, width, contact_resistance_sq, shunt_resistance_sq, contact_id, charge_type,ve0,vh0,type,id):
		self.tab.blockSignals(True)

		self.tab.set_value(pos,0,name)
		self.tab.set_value(pos,1,position.lower())
		self.tab.set_value(pos,2,str(applied_voltage_type)+":"+str(applied_voltage))
		self.tab.set_value(pos,3,start)
		self.tab.set_value(pos,4,width)
		self.tab.set_value(pos,5,contact_resistance_sq)
		self.tab.set_value(pos,6,shunt_resistance_sq)
		self.tab.set_value(pos,7,contact_id)
		self.tab.set_value(pos,8, charge_type.lower())
		self.tab.set_value(pos,9,ve0)
		self.tab.set_value(pos,10,vh0)
		self.tab.set_value(pos,11,type)
		self.tab.set_value(pos,12,id)

		self.tab.blockSignals(False)
		

	def add_row(self):

		pos= self.tab.insert_row()

		#name
		self.tab.blockSignals(True)
		self.tab.setItem(pos,0,QTableWidgetItem(""))

		#position
		combobox = QComboBoxLang()
		combobox.addItemLang("top",_("top"))
		combobox.addItemLang("bottom",_("bottom"))
		combobox.addItemLang("right",_("right"))
		combobox.addItemLang("left",_("left"))

		self.tab.setCellWidget(pos,1, combobox)
		combobox.currentIndexChanged.connect(self.save_and_redraw)


		applied_voltage=gpvdm_applied_voltage()
		self.tab.setCellWidget(pos,2, applied_voltage)
		applied_voltage.changed.connect(self.save)
		self.tab.setColumnWidth(2, 200)

		self.tab.setItem(pos,3,QTableWidgetItem(""))
		self.tab.setItem(pos,4,QTableWidgetItem(""))

		self.tab.setItem(pos,5,QTableWidgetItem(""))
		self.tab.setColumnWidth(5, 140)

		self.tab.setItem(pos,6,QTableWidgetItem(""))
		self.tab.setColumnWidth(6, 110)

		energy_to_charge_box=energy_to_charge()
		self.tab.setCellWidget(pos,7, energy_to_charge_box)
		self.tab.setColumnWidth(7, 150)

		#self.tab.setItem(pos,7,QTableWidgetItem(""))		

		combobox = QComboBoxLang()
		combobox.addItemLang("electron",_("Electron"))
		combobox.addItemLang("hole",_("Hole"))

		self.tab.setCellWidget(pos,8, combobox)
		combobox.currentIndexChanged.connect(self.save)
		self.tab.setColumnWidth(8, 120)

		self.tab.setItem(pos,9,QTableWidgetItem(""))
		self.tab.setItem(pos,10,QTableWidgetItem(""))

		combobox = QComboBoxLang()
		combobox.addItemLang("ohmic",_("Ohmic"))
		combobox.addItemLang("schottky",_("Schottky"))

		self.tab.setCellWidget(pos,11, combobox)
		combobox.currentIndexChanged.connect(self.save)

		self.tab.setItem(pos,12,QTableWidgetItem(""))
		self.tab.setColumnWidth(12, 20)

		self.tab.blockSignals(False)
		
		return pos

	def on_add_clicked(self, button):
		data=gpvdm_data()
		pos=self.add_row()
		c=self.contacts.insert(pos)
		data.save()
		self.set_row(pos,c.shape_name,c.position,c.applied_voltage_type,c.applied_voltage,str(c.x0),str(c.dx),str(c.contact_resistance_sq),c.shunt_resistance_sq,str(c.id),str(c.charge_type),str(c.ve0),str(c.vh0),str(c.physical_model),c.id)
		#print(pos,len(self.contacts))
		self.save_and_redraw()

	def on_remove_clicked(self):
		ids=[]
		rows=self.tab.selectionModel().selectedRows()

		for r in rows:
			ids.append(self.tab.get_value(r.row(), 12))

		self.tab.remove()
		#self.contacts.remove_by_id(ids)
		gpvdm_data().pop_object_by_id(ids[0])	
		self.save_and_redraw()

	def show_hide_cols(self):
		data=gpvdm_data()
		schottky=False
		for i in range(0,self.tab.rowCount()):
			if self.tab.get_value(i, 11)=="schottky":
				schottky=True
				break

		if schottky==True:
			self.tab.setColumnHidden(9,False)
			self.tab.setColumnHidden(10,False)
		else:
			self.tab.setColumnHidden(9,True)
			self.tab.setColumnHidden(10,True)

		if data.electrical_solver.solver_type=="circuit":
			self.tab.setColumnHidden(7,True)
			self.tab.setColumnHidden(8,True)
			self.tab.setColumnHidden(11,True)
		else:
			self.tab.setColumnHidden(7,False)
			self.tab.setColumnHidden(8,False)
			self.tab.setColumnHidden(11,False)

	def save_and_redraw(self):
		self.save()
		self.changed.emit()
		global_object_run("gl_force_redraw")

	def save(self):
		if self.update_contact_db()==True:
			for i in range(0,self.tab.rowCount()):
				self.tab.cellWidget(i,7).update()

			self.show_hide_cols()
			data=gpvdm_data()
			data.save()


	def callback_help(self):
		webbrowser.open('http://www.gpvdm.com/man/index.html')

	def tab_changed(self, x,y):
		self.save_and_redraw()

	def update(self):
		i=0
		for c in self.contacts.segments:
			if c.position=="top" or c.position=="bottom":
				start=str(c.x0)
				width=str(c.dx)
			else:
				start=str(c.y0)
				width=str(c.dy)
			self.set_row(i,str(c.shape_name),c.position,c.applied_voltage_type,c.applied_voltage,start,width,str(c.contact_resistance_sq),c.shunt_resistance_sq,str(c.id),str(c.charge_type),str(c.ve0),str(c.vh0),str(c.physical_model),c.id )

			i=i+1

		self.show_hide_cols()

	def hide_cols(self,val):
		self.tab.setColumnHidden(3,val)
		self.tab.setColumnHidden(4,val)
		#self.tab.setColumnHidden(5,val)


	def load(self):
		self.contacts=self.epi.contacts
		self.tab.clear()
		self.tab.setHorizontalHeaderLabels([_("Name"),_("Top/Bottom"),_("Applied\nvoltage"),_("Start")+" (m)", _("Width")+" (m)" , _("Contact resistance\n")+" (Ohms m^2)",_("Shunt resistance")+"\n(Ohms m^2)",_("Charge density/\nFermi-offset"),_("Majority\ncarrier"),_("ve0 (m/s)"),_("vh0 (m/s)"),_("Physical\nmodel"),_("ID")])
		self.tab.setColumnHidden(5,True)
		self.tab.setColumnHidden(6,True)

		self.tab.horizontalHeader().setFixedHeight(60)
		#self.contacts.load()

		if get_mesh().z.get_points()!=1 or get_mesh().x.get_points()!=1: 
			self.hide_cols(False)
		else:
			self.hide_cols(True)

		for c in self.contacts.segments:
			self.add_row()
		self.update()

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

		self.tab = gpvdm_tab(toolbar=self.toolbar)
		self.tab.resizeColumnsToContents()
		self.tab.segments=get_epi().contacts.segments
		self.tab.tb_add.triggered.connect(self.on_add_clicked)
		self.tab.user_remove_rows.connect(self.on_remove_clicked)
		self.tab.tb_down.triggered.connect(self.on_move_down)
		self.tab.tb_up.triggered.connect(self.on_move_up)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.toolbar.addWidget(spacer)

		self.help = QAction(icon_get("help"), _("Help"), self)
		self.help.setStatusTip(_("Close"))
		self.help.triggered.connect(self.callback_help)
		self.toolbar.addAction(self.help)

		self.tab.verticalHeader().setVisible(False)

		self.tab.clear()
		self.tab.setColumnCount(13)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)

		self.load()

		self.tab.cellChanged.connect(self.tab_changed)

		self.main_vbox.addWidget(self.tab)


		self.setLayout(self.main_vbox)

		gpvdm_data().add_call_back(self.update)

		#get_contactsio().changed.connect(self.update)


