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
from play import play
from server import server_get
from cal_path import get_sim_path

class window_light_src(experiment):

	def __init__(self,data=None):
		experiment.__init__(self,window_save_name="window_light_src", window_title=_("Light source editor"),name_of_tab_class="tab_light_src",json_search_path="gpvdm_data().light_sources.lights")

		self.base_json_obj="from json_light_sources import json_light_source"
		self.notebook.currentChanged.connect(self.switch_page)

		self.tb_configure = QAction(icon_get("cog"), _("Configure"), self)
		self.tb_configure.triggered.connect(self.callback_configwindow)
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.tb_configure)

		self.run = play(self,"optics_ribbon_run",run_text=_("Rebuild"))
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.run)
		self.run.start_sim.connect(self.callback_run)

		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)

	def callback_configwindow(self):
		from tab import tab_class
		data=gpvdm_data()
		self.configure_widget=tab_class(gpvdm_data().light_sources.config)
		self.configure_widget.show()

	def callback_run(self):
		data=gpvdm_data()
		self.dump_verbosity=data.light.dump_verbosity
		data.light.dump_verbosity=1
		data.save()

		self.my_server=server_get()
		self.my_server.clear_cache()
		self.my_server.add_job(get_sim_path(),"--simmode opticalmodel@optics")
		self.my_server.sim_finished.connect(self.callback_sim_finished)
		self.my_server.start()

	def callback_sim_finished(self):
		data=gpvdm_data()
		data.light.dump_verbosity=self.dump_verbosity
		data.save()
		self.update()
