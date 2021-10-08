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

## @package fxexperiment
#  Main fx domain experiment window
#

import os
from gui_util import dlg_get_text
import webbrowser
from inp import inp_update_token_value
from fxexperiment_tab import fxexperiment_tab
from util_zip import zip_lsdir
from inp import inp

from global_objects import global_object_get
from icon_lib import icon_get
from global_objects import global_object_register
from gui_util import yes_no_dlg

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#window
from QHTabBar import QHTabBar
from PyQt5.QtCore import pyqtSignal

from util import wrap_text

from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos

from progress_class import progress_class
from process_events import process_events
from experiment import experiment
from fxexperiment_tab import fxexperiment_tab


class fxexperiment(experiment):


	def __init__(self):
		experiment.__init__(self,"fxexperiment_tab",window_save_name="fx_domain_experiment", window_title=_("Frequency domain experiment window"),json_search_path="gpvdm_data().fx_domain")

		#w=self.ribbon_simulation()
		#self.ribbon.addTab(w,_("Simulation"))

		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()



	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)

