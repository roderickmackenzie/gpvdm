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

## @package dos_main
#  The main DoS dialog.
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
from QHTabBar import QHTabBar
from global_objects import global_object_register
from icon_lib import icon_get

from css import css_apply

from inp import inp

from cal_path import get_sim_path

from gpvdm_json import gpvdm_data
from help import QAction_help
from lock import get_lock

class dos_main(QWidget,tab_base):

	def __init__(self):
		QWidget.__init__(self)
		self.setMinimumSize(1000, 600)

		self.main_vbox = QVBoxLayout()

		self.setWindowIcon(icon_get("preferences-system"))

		self.setWindowTitle(_("Electrical parameter editor")+" (https://www.gpvdm.com)") 

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		self.auger = QAction(icon_get("auger"), _("Enable\nAuger"), self)
		self.auger.setCheckable(True)
		self.auger.triggered.connect(self.callback_auger)
		toolbar.addAction(self.auger)

		self.traps = QAction(icon_get("traps"), _("Dynamic\nSRH traps"), self)
		self.traps.setCheckable(True)
		self.traps.triggered.connect(self.callback_traps)
		toolbar.addAction(self.traps)

		self.steady_state_srh = QAction(icon_get("srh"), _("Equilibrium\nSRH traps"), self)
		self.steady_state_srh.setCheckable(True)
		self.steady_state_srh.triggered.connect(self.callback_srh)
		toolbar.addAction(self.steady_state_srh)

		self.exciton = QAction(icon_get("exciton"), _("Excitons"), self)
		self.exciton.setCheckable(True)
		self.exciton.triggered.connect(self.callback_exciton)
		toolbar.addAction(self.exciton)

		self.singlet = QAction(icon_get("singlet"), _("Excited\nstates"), self)
		self.singlet.setCheckable(True)
		self.singlet.triggered.connect(self.callback_singlet)
		if get_lock().is_gpvdm_next()==True:
			toolbar.addAction(self.singlet)

		toolbar.addWidget(spacer)

		self.help = QAction_help()
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)

		self.notebook = QTabWidget()

		css_apply(self,"tab_default.css")

		self.main_vbox.addWidget(self.notebook)
		self.setLayout(self.main_vbox)

		global_object_register("dos_update",self.update)
		self.status_bar=QStatusBar()
		self.notebook.currentChanged.connect(self.changed_click)

		self.main_vbox.addWidget(self.status_bar)	
		self.update()

		gpvdm_data().add_call_back(self.update_values)
		self.destroyed.connect(self.doSomeDestruction)

	def doSomeDestruction(self):
		gpvdm_data().remove_call_back(self.update_values)

	def update_values(self):
		epi=get_epi()
		for i in range(0,self.notebook.count()):
			w=self.notebook.widget(i)
			w.tab.template_widget=epi.find_object_by_id(w.tab.template_widget.id)
			w.tab.update_values()

	def update(self):
		self.notebook.blockSignals(True)
		self.notebook.clear()
		data=gpvdm_data()
		for l in data.epi.layers:
			if data.electrical_solver.solver_type!="circuit":
				if l.shape_dos.enabled==True and l.shape_enabled==True:
					
						name="DoS of "+l.shape_name
						#print(l.shape_dos)
						widget=tab_class(None,json_path="gpvdm_data().epi",uid=l.shape_dos.id)
						self.notebook.addTab(widget,name)

						for s in l.shapes:
							if s.shape_dos.enabled==True and s.shape_enabled==True:
								name="DoS of "+s.shape_name
								widget=tab_class(None,json_path="gpvdm_data().epi",uid=s.shape_dos.id)
								widget.tab.json_human_path=""
								self.notebook.addTab(widget,name)
			else:
				if l.layer_type=="active":
					name="Electrical "+l.shape_name
					widget=tab_class(l.shape_electrical)

					self.notebook.addTab(widget,name)

		self.changed_click()
		self.notebook.blockSignals(False)

	def help(self):
		help_window().help_set_help(["tab.png","<big><b>Density of States</b></big>\nThis tab contains the electrical model parameters, such as mobility, tail slope energy, and band gap."])

	def changed_click(self):
		data=gpvdm_data()
		if data.electrical_solver.solver_type!="circuit":
			self.auger.setEnabled(True)
			self.traps.setEnabled(True)
			self.singlet.setEnabled(True)
			self.steady_state_srh.setEnabled(True)
			self.singlet.setEnabled(True)

			tab = self.notebook.currentWidget()
			tab.tab.refind_template_widget()
			self.auger.setChecked(tab.tab.template_widget.dos_enable_auger)
			self.steady_state_srh.setChecked(tab.tab.template_widget.ss_srh_enabled)
			self.exciton.setChecked(tab.tab.template_widget.exciton_enabled)
			self.singlet.setChecked(tab.tab.template_widget.singlet_enabled)

			traps_enabled=False
			for l in data.epi.layers:
				if l.shape_dos.enabled==True and l.shape_enabled==True:
					if l.shape_dos.srh_bands>0:
						traps_enabled=True
						break
					for s in l.shapes:
						if s.shape_dos.enabled==True and s.shape_enabled==True:
							if s.shape_dos.srh_bands>0:
								traps_enabled=True
								break
			self.traps.setChecked(traps_enabled)
		else:
			self.auger.setEnabled(False)
			self.traps.setEnabled(False)
			self.steady_state_srh.setEnabled(False)
			self.singlet.setEnabled(False)

	def callback_auger(self):
		data=gpvdm_data()
		if data.electrical_solver.solver_type!="circuit":
			tab = self.notebook.currentWidget()
			tab.tab.refind_template_widget()
			tab.tab.template_widget.dos_enable_auger=self.auger.isChecked()
			tab.tab.hide_show_widgets()
			data.save()

	def callback_srh(self):
		data=gpvdm_data()
		if data.electrical_solver.solver_type!="circuit":
			tab = self.notebook.currentWidget()
			tab.tab.refind_template_widget()
			tab.tab.template_widget.ss_srh_enabled=self.steady_state_srh.isChecked()
			tab.tab.hide_show_widgets()
			data.save()

	def callback_exciton(self):
		data=gpvdm_data()
		if data.electrical_solver.solver_type!="circuit":
			tab = self.notebook.currentWidget()
			tab.tab.refind_template_widget()

			for l in data.epi.layers:
				l.shape_dos.exciton_enabled=self.exciton.isChecked()
				for s in l.shapes:
					s.shape_dos.exciton_enabled=self.exciton.isChecked()

			for i in range(0,self.notebook.count()):
				tab=self.notebook.widget(i)
				tab.tab.update_values()
				tab.tab.hide_show_widgets()

			data.exciton.exciton_enabled=self.exciton.isChecked()

			tab.tab.hide_show_widgets()
			data.save()

	def callback_singlet(self):
		data=gpvdm_data()
		if data.electrical_solver.solver_type!="circuit":
			tab = self.notebook.currentWidget()
			tab.tab.refind_template_widget()

			for l in data.epi.layers:
				l.shape_dos.singlet_enabled=self.singlet.isChecked()
				for s in l.shapes:
					s.shape_dos.singlet_enabled=self.singlet.isChecked()

			for i in range(0,self.notebook.count()):
				tab=self.notebook.widget(i)
				tab.tab.update_values()
				tab.tab.hide_show_widgets()

			data.singlet.singlet_enabled=self.singlet.isChecked()

			tab.tab.hide_show_widgets()
			data.save()

	def callback_traps(self):
		data=gpvdm_data()
		if self.traps.isChecked()==True:
			ntraps=8
		else:
			ntraps=0

		for l in data.epi.layers:
			l.shape_dos.srh_bands=ntraps
			for s in l.shapes:
				s.shape_dos.srh_bands=ntraps

		for i in range(0,self.notebook.count()):
			tab=self.notebook.widget(i)
			tab.tab.update_values()
			tab.tab.hide_show_widgets()

		data.save()
