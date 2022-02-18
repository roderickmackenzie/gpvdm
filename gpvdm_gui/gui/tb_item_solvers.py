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

## @package tb_item_solvers
#  A toolbar item to select the solver type
#


#inp
import os

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication, QMenu
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QLabel,QComboBox
from PyQt5.QtCore import pyqtSignal

from gpvdm_json import gpvdm_data
from global_objects import global_object_run
from QAction_lock import QAction_lock
from config_window import class_config_window

class tb_item_solvers(QToolBar):

	changed = pyqtSignal()
	

	def __init__(self):
		QWidget.__init__(self)
		self.setOrientation(Qt.Vertical)
		self.setToolButtonStyle( Qt.ToolButtonTextBesideIcon)
		self.setStyleSheet(" QToolButton { padding-left: 0px; padding-right: 0px; padding-top: 0px;padding-bottom: 0px; width: 200px;} QToolButton:checked {background-color: LightBlue; }")
		#self.setMinimumSize(800,10)

		self.tb_item_poisson = QAction_lock("poisson", _("Electrostatic solver"), self,"ribbon_thermal_joule")
		self.tb_item_poisson.clicked.connect(self.callback_solver_poisson_click)
		self.tb_item_poisson.setCheckable(True)

		self.menu_poisson = QMenu(self)
		configure_item=QAction(_("Configure"), self)
		self.menu_poisson.addAction(configure_item)
		self.tb_item_poisson.setMenu(self.menu_poisson)
		configure_item.triggered.connect(self.callback_poisson)
		self.addAction(self.tb_item_poisson)

		self.tb_item_newton = QAction_lock("newton", _("Drift diffusion"), self,"ribbon_thermal_optical")
		self.tb_item_newton.clicked.connect(self.callback_solver_newton_click)
		self.tb_item_newton.setCheckable(True)

		self.menu_newton = QMenu(self)
		configure_item=QAction(_("Configure"), self)
		self.menu_newton.addAction(configure_item)
		self.tb_item_newton.setMenu(self.menu_newton)
		configure_item.triggered.connect(self.callback_newton)

		self.addAction(self.tb_item_newton)

		self.tb_item_circuit = QAction_lock("kirchhoff", _("Simple circuit solver"), self,"ribbon_thermal_recombination")
		self.tb_item_circuit.clicked.connect(self.callback_circuit_clicked)
		self.tb_item_circuit.setCheckable(True)
		self.addAction(self.tb_item_circuit)

		self.update()

	def update(self):
		data=gpvdm_data()

		self.tb_item_newton.blockSignals(True)
		self.tb_item_poisson.blockSignals(True)
		self.tb_item_circuit.blockSignals(True)
		if data.electrical_solver.solver_type=="drift-diffusion":
			self.tb_item_poisson.setChecked(True)
			self.tb_item_newton.setChecked(True)
			self.tb_item_circuit.setChecked(False)
		elif data.electrical_solver.solver_type=="poisson":
			self.tb_item_poisson.setChecked(True)
			self.tb_item_newton.setChecked(False)
			self.tb_item_circuit.setChecked(False)
		elif data.electrical_solver.solver_type=="circuit":
			self.tb_item_poisson.setChecked(False)
			self.tb_item_newton.setChecked(False)
			self.tb_item_circuit.setChecked(True)
		elif data.electrical_solver.solver_type=="none":
			self.tb_item_poisson.setChecked(False)
			self.tb_item_newton.setChecked(False)
			self.tb_item_circuit.setChecked(False)
		self.tb_item_newton.blockSignals(False)
		self.tb_item_poisson.blockSignals(False)
		self.tb_item_circuit.blockSignals(False)

	def callback_poisson(self):
		data=gpvdm_data()
		self.mesh_config=class_config_window([data.electrical_solver.poisson],[_("Poission solver")],title=_("Poission solver configuration"),icon="poisson")
		self.mesh_config.show()

	def callback_newton(self):
		from tab_banned_files import tab_banned_files
		data=gpvdm_data()
		self.mesh_config=class_config_window([data.math,data.dump,data.exciton],[_("Newton solver"),_("Output files"),_("Exciton solver")],title=_("Newton solver configuration"),icon="newton")

		tab=tab_banned_files()
		self.mesh_config.notebook.addTab(tab,_("Banned files"))
		self.mesh_config.show()

	def callback_solver_poisson_click(self):
		data=gpvdm_data()
		pos=self.tb_item_poisson.isChecked()
		dd=self.tb_item_newton.isChecked()
		cir=self.tb_item_circuit.isChecked()
		if pos==True:
			data.electrical_solver.solver_type="poisson"
		else:
			data.electrical_solver.solver_type="none"
		data.save()
		self.update()
		self.changed.emit()

	def callback_solver_newton_click(self):
		data=gpvdm_data()
		pos=self.tb_item_poisson.isChecked()
		dd=self.tb_item_newton.isChecked()
		cir=self.tb_item_circuit.isChecked()
		if dd==True:
			data.electrical_solver.solver_type="drift-diffusion"
		else:
			data.electrical_solver.solver_type="none"
		data.save()
		self.update()
		self.changed.emit()

	def callback_circuit_clicked(self):
		data=gpvdm_data()
		pos=self.tb_item_poisson.isChecked()
		dd=self.tb_item_newton.isChecked()
		cir=self.tb_item_circuit.isChecked()
		if cir==True:
			data.electrical_solver.solver_type="circuit"
		else:
			data.electrical_solver.solver_type="none"
		data.save()
		self.update()
		self.changed.emit()
		
	def setEnabled(self,val):
		self.tb_item_poisson.setEnabled(val)
		self.tb_item_newton.setEnabled(val)
		self.tb_item_circuit.setEnabled(val)

