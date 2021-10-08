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

## @package tb_pulse_load_type
#  tool bar item to select the type of load to have on a pulsed simulation.
#

import os

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QLabel,QComboBox
from PyQt5.QtCore import pyqtSignal
from gpvdm_json import gpvdm_data

class tb_pulse_load_type(QWidget):

	changed = pyqtSignal()

	def __init__(self,config_class):
		self.config_class=config_class
		QWidget.__init__(self)


		layout=QHBoxLayout()
		label=QLabel()
		label.setText(_("Load type")+":")
		layout.addWidget(label)

		self.sim_mode = QComboBox(self)
		self.sim_mode.setEditable(True)


		layout.addWidget(self.sim_mode)

		self.setLayout(layout)

		self.sim_mode.addItem("open_circuit")
		self.sim_mode.addItem("load")
		self.sim_mode.addItem("ideal_diode_ideal_load")


		mode=self.config_class.load_type

		all_items  = [self.sim_mode.itemText(i) for i in range(self.sim_mode.count())]
		for i in range(0,len(all_items)):
		    if all_items[i] == mode:
		        self.sim_mode.setCurrentIndex(i)

		self.sim_mode.currentIndexChanged.connect(self.call_back_sim_mode_changed)


	def call_back_sim_mode_changed(self):
		mode=self.sim_mode.currentText()
		self.config_class.load_type=mode
		gpvdm_data().save()
		self.changed.emit()

