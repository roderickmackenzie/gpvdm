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

## @package fit_configure_window
#  The main window used for configuring the fit.
#

import os
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget
from PyQt5.QtGui import QPainter,QIcon

#windows
from tab import tab_class

from PyQt5.QtCore import pyqtSignal

from QWidgetSavePos import QWidgetSavePos
from gpvdm_json import gpvdm_data
from help import QAction_help

class fit_configure_window(QWidgetSavePos):

	changed = pyqtSignal()
	
	def callback_tab_changed(self):
		self.changed.emit()

	def __init__(self,name):
		from fit_duplicate import fit_duplicate
		from fit_vars import fit_vars
		from fit_rules import fit_rules

		QWidgetSavePos.__init__(self,name)
		
		self.setMinimumSize(900, 600)
		self.setWindowIcon(icon_get("preferences-system"))

		self.setWindowTitle(_("Fit configure")+" (https://www.gpvdm.com)") 
		

		self.main_vbox = QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.help = QAction_help()
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)		

		self.notebook = QTabWidget()
		self.notebook.setMovable(True)
		self.main_vbox.addWidget(self.notebook)

		data=gpvdm_data()

		self.duplicate_window=fit_duplicate()
		self.notebook.addTab(self.duplicate_window,_("Duplicate variables"))

		self.fit_vars_window=fit_vars()
		self.notebook.addTab(self.fit_vars_window,_("Fit variables"))

		self.fit_rules_window=fit_rules()
		self.notebook.addTab(self.fit_rules_window,_("Fit rules"))

		tab=tab_class(data.fits.fit_config)
		self.notebook.addTab(tab,_("Configure minimizer"))
	
		self.setLayout(self.main_vbox)



