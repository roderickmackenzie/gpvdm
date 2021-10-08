# -*- coding: utf-8 -*-
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

## @package solar_main
#  Part of solar module - delete?
#
import sys
from PyQt5.QtWidgets import QMenuBar, QWidget, QApplication, QAction,QDesktopWidget,QTabWidget,QVBoxLayout
from PyQt5.QtGui import QIcon

import os

from ribbon_solar import ribbon_solar

from icon_lib import icon_get
from PyQt5.QtCore import pyqtSignal

from spctral2_gui import spctral2_gui

class solar_spectrum_gen_window(QWidget):

	update = pyqtSignal()

	def __init__(self):
		self.export_file_name="data.dat"#os.path.join(self.path,"spectra.inp")
		super().__init__()
		self.resize(1200,600)
		self.setWindowIcon(icon_get("weather-few-clouds"))

		self.vbox=QVBoxLayout()

		self.ribbon = ribbon_solar()
		self.vbox.addWidget(self.ribbon)
		
		self.ribbon.run.clicked.connect(self.callback_run)

		self.ribbon.export.triggered.connect(self.callback_export)
		
		self.setWindowTitle(_("Solar Spectrum Generator")+" (https://www.gpvdm.com)")


		self.notebook = QTabWidget()

		self.vbox.addWidget(self.notebook)
		self.spctral2_gui=spctral2_gui()
		self.notebook.addTab(self.spctral2_gui,"spctral2")
		self.setLayout(self.vbox)


	def callback_run(self):
		tab = self.notebook.currentWidget()
		tab.calculate()


	def save(self):
		self.notebook.currentWidget().save()

	def copy(self):
		self.notebook.currentWidget().copy2clip()

	def callback_export(self):
		self.notebook.currentWidget().export()



