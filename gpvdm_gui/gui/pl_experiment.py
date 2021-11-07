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

## @package pl_experiment
#  PL simulation set up, for PL spectra.
#

import os
from gui_util import dlg_get_text
import webbrowser

from global_objects import global_object_get
from icon_lib import icon_get
from global_objects import global_object_register

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#window
from tb_lasers import tb_lasers
from QWidgetSavePos import QWidgetSavePos
from plexperiment_tab import plexperiment_tab
from experiment import experiment

class pl_experiment(experiment):

	def ribbon_simulation(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.tb_lasers=tb_lasers()
		toolbar.addWidget(self.tb_lasers)

		return toolbar

	def __init__(self,data=None):
		experiment.__init__(self,"plexperiment_tab",window_save_name="plexperiment_editor", window_title=_("PL experiment window"),json_search_path="gpvdm_data().pl_ss")


		self.notebook.currentChanged.connect(self.switch_page)
		w=self.ribbon_simulation()
		self.ribbon.addTab(w,_("Simulation"))

		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		if tab!=None:
			self.tb_lasers.update(tab.data)



