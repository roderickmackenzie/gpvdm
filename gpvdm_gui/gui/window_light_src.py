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

## @package jv_experiment
#  JV experiment editor
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
from QWidgetSavePos import QWidgetSavePos
from progress_class import progress_class
from process_events import process_events
from global_objects import global_object_run
from experiment import experiment
from gpvdm_json import gpvdm_data
class window_light_src(experiment):


	def __init__(self,data=None):
		experiment.__init__(self,window_save_name="window_light_src", window_title=_("Light source editor"),name_of_tab_class="tab_light_src",json_search_path="gpvdm_data().light_sources.lights")


		self.notebook.currentChanged.connect(self.switch_page)

		self.tb_configure = QAction(icon_get("cog"), _("Configure"), self)
		self.tb_configure.triggered.connect(self.callback_configwindow)
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.tb_configure)

		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)

	def callback_configwindow(self):
		from tab import tab_class
		data=gpvdm_data()
		self.configure_widget=tab_class(gpvdm_data().light_sources.config)
		self.configure_widget.show()
