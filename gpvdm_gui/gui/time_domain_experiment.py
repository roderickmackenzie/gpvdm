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

## @package time_domain_experiment
#  The main experiment window, used for configuring time domain experiments.
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
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#window
from gui_util import yes_no_dlg
from PyQt5.QtCore import pyqtSignal
from util import wrap_text
from QWidgetSavePos import QWidgetSavePos
from cal_path import get_sim_path

from css import css_apply

from progress_class import progress_class
from process_events import process_events
from gpvdm_json import gpvdm_data
from experiment import experiment
from tb_lasers import tb_lasers

class time_domain_experiment(experiment):

	def ribbon_simulation(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.tb_laser_start_time = QAction(icon_get("laser"), wrap_text(_("Laser start time"),2), self)
		toolbar.addAction(self.tb_laser_start_time)

		self.tb_lasers=tb_lasers()
		toolbar.addWidget(self.tb_lasers)

		self.tb_start = QAction(icon_get("start"), _("Simulation start time"), self)
		toolbar.addAction(self.tb_start)

		return toolbar

	def __init__(self):
		gpvdm_data().time_domain.fix_identical_uids([])
		experiment.__init__(self,"time_domain_experiment_tab",window_save_name="time_domain_experiment", window_title=_("Time domain experiment window"),json_search_path="gpvdm_data().time_domain")

		w=self.ribbon_simulation()
		self.ribbon.addTab(w,_("Simulation"))

		self.tb_laser_start_time.triggered.connect(self.callback_laser_start_time)

		self.tb_start.triggered.connect(self.callback_start_time)

		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()

	def callback_laser_start_time(self):
		tab = self.notebook.currentWidget()
		tab.tmesh.callback_laser()

	def callback_start_time(self):
		tab = self.notebook.currentWidget()
		tab.tmesh.callback_start_time()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		if tab!=None:
			self.tb_lasers.update(tab.data)

