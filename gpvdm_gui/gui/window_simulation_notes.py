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

## @package scripts
#  The main script editor
#


import os
from icon_lib import icon_get
from tab import tab_class
import webbrowser
from help import my_help_class

#path
#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QHBoxLayout,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu, QComboBox, QMenuBar, QLabel
from PyQt5.QtGui import QIcon

#windows
from error_dlg import error_dlg

from QWidgetSavePos import QWidgetSavePos

from ribbon_simulation_notes import ribbon_simulation_notes

from css import css_apply
from gui_util import yes_no_dlg
from script_editor import script_editor
from gui_util import dlg_get_text
from gpvdm_json import gpvdm_data
from ref import ref_window
from cal_path import get_sim_path
from tab import tab_class

class window_simulation_notes(QWidgetSavePos):

	def __init__(self,path="",api_callback=None):

		QWidgetSavePos.__init__(self,"simulation_notes")

		self.setWindowIcon(icon_get("script"))

		self.setMinimumSize(1000, 600)
		self.setWindowTitle(_("Simulation notes")+" (https://www.gpvdm.com)")    

		self.ribbon=ribbon_simulation_notes()

		data=gpvdm_data()

		self.setWindowIcon(icon_get("script"))

		self.main_vbox=QVBoxLayout()

		self.ribbon.tb_save.clicked.connect(self.callback_save)

		#self.ribbon.tb_rename.clicked.connect(self.callback_rename_page)

		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)

		self.main_vbox.addWidget(self.ribbon)
		self.api_callback=api_callback


		self.notebook = QTabWidget()
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

		#file_name=os.path.join(self.path,f)
		a=script_editor()
		a.api_callback=self.api_callback
		a.setText(data.sim.notes)
		a.status_changed.connect(self.callback_tab_changed)
		a.save_signal.connect(self.callback_save)
		self.notebook.addTab(a,_("Notes"))

		self.ref_window=ref_window(os.path.join(get_sim_path(),"json.bib"),"device",show_toolbar=False)
		self.notebook.addTab(self.ref_window,_("Bibtex reference"))

		self.config_window=tab_class(data)
		self.notebook.addTab(self.config_window,_("Config"))

		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.main_vbox.addWidget(self.notebook)


		self.setLayout(self.main_vbox)

	def callback_tab_changed(self):
		tab = self.notebook.currentWidget()
		index=self.notebook.currentIndex() 

		#short_name=os.path.basename(tab.file_name)

		if tab.not_saved==True:
			self.notebook.setTabText(index, _("Simulation Notes")+"*")
		else:
			self.notebook.setTabText(index, _("Simulation Notes"))

	def callback_save(self):
		tab = self.notebook.currentWidget()
		if type(tab)==script_editor:
			data=gpvdm_data()
			data.sim.notes=tab.getText()
			data.save()
			tab.not_saved=False
			self.callback_tab_changed()

		
	def callback_run(self):
		tab = self.notebook.currentWidget()
		tab.run()

