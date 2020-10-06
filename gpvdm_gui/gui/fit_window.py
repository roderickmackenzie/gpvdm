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

## @package fit_window
#  Main fit window
#

import os
import shutil
import webbrowser
from code_ctrl import enable_betafeatures
from util_zip import zip_lsdir
from util import strextract_interger
from global_objects import global_object_get
from icon_lib import icon_get

from global_objects import global_object_register
from server import server_get
from help import help_window

import i18n
_ = i18n.language.gettext

#inp
from inp import inp
from inp import inp_copy_file
from inp import inp_remove_file
from inp import inp_update_token_value
from fit_configure_window import fit_configure_window

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QStatusBar, QTableWidget, QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon,QCursor

#windows
from gui_util import yes_no_dlg

from fit_tab import fit_tab
from QHTabBar import QHTabBar

from gui_util import dlg_get_text

from fit_progress import fit_progress
from inp import inp_get_token_value
from str2bool import str2bool

from util import wrap_text
from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos

from css import css_apply
from cal_path import get_inp_file_path

from fit_ribbon import fit_ribbon

class fit_window(QWidgetSavePos):

	def update(self):
		for i in range(0,self.notebook.count()):
			tab = self.notebook.widget(i)
			tab.update()

		self.fit_progress.update()

	def callback_configure(self):
		if self.fit_configure_window==None:
			self.fit_configure_window=fit_configure_window("fit_config")

		help_window().help_set_help(["vars.png",_("<big><b>The fitting variables window</b></big><br> Use this window to select the variables use to perform the fit.")])
		if self.fit_configure_window.isVisible()==True:
			self.fit_configure_window.hide()
		else:
			self.fit_configure_window.show()

	def callback_help(self):
		webbrowser.open('https://www.gpvdm.com/man/index.html')

	def callback_add_page(self,file_name):
		new_tab=fit_tab(file_name)
		self.notebook.addTab(new_tab,new_tab.get_tab_text())

	def remove_invalid(self,input_name):
		return input_name.replace (" ", "_")

	def callback_import(self):
		tab = self.notebook.currentWidget()
		tab.import_data()

	def callback_view_toggle_tab(self):
		print("add code")
		#self.toggle_tab_visible(data)

	def load_tabs(self):

		self.ribbon.order_widget.load_tabs()
		self.fit_progress=fit_progress()
		self.notebook.addTab(self.fit_progress,"Fit progress")


	def callback_one_fit(self):
		my_server=server_get()
		my_server.clear_cache()
		my_server.add_job(get_sim_path(),"--1fit")
		my_server.start()

	def callback_do_fit(self):
		my_server=server_get()
		my_server.clear_cache()
		my_server.add_job(get_sim_path(),"--fit")
		my_server.start()

	def callback_enable(self):
		tab = self.notebook.currentWidget()
		tab.set_enable(self.ribbon.enable.enabled)
		index=self.notebook.currentIndex()
		self.notebook.setTabText(index, tab.get_tab_text())

	def changed_click(self):
		tab = self.notebook.currentWidget()
		if type(tab)==fit_tab:
			self.ribbon.enable.setState(tab.is_enabled())
			self.ribbon.enable.setEnabled(True)
			self.ribbon.import_data.setEnabled(True)

		if type(tab)==fit_progress:
			self.ribbon.enable.setEnabled(False)
			self.ribbon.import_data.setEnabled(False)

	def callback_export_image(self):
		tab = self.notebook.currentWidget()
		tab.callback_export_image()

	def callback_export_csv(self):
		tab = self.notebook.currentWidget()
		tab.callback_export_csv()

	def callback_export_xls(self):
		tab = self.notebook.currentWidget()
		tab.callback_export_xls()

	def callback_export_gnuplot(self):
		tab = self.notebook.currentWidget()
		tab.callback_export_gnuplot()

	def __init__(self,name):
		QWidgetSavePos.__init__(self,name)
		self.main_vbox = QVBoxLayout()

		#self.setFixedSize(900, 700)
		self.setWindowTitle(_("Fit the simulation to experimental data")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("fit"))

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)



		self.main_vbox.addWidget(toolbar)

		self.ribbon=fit_ribbon()

		self.ribbon.order_widget.added.connect(self.callback_add_page)
		self.ribbon.help.triggered.connect(self.callback_help)
		self.ribbon.play.start_sim.connect(self.callback_do_fit)

		self.ribbon.play_one.start_sim.connect(self.callback_one_fit)

		self.ribbon.import_data.triggered.connect(self.callback_import)
		self.ribbon.tb_configure.triggered.connect(self.callback_configure)

		self.ribbon.tb_export_as_jpg.clicked.connect(self.callback_export_image)

		self.ribbon.tb_export_as_csv.clicked.connect(self.callback_export_csv)

		self.ribbon.tb_export_as_xls.clicked.connect(self.callback_export_xls)

		self.ribbon.tb_export_as_gnuplot.clicked.connect(self.callback_export_gnuplot)

		self.main_vbox.addWidget(self.ribbon)

		self.notebook = QTabWidget()
		self.ribbon.order_widget.notebook_pointer=self.notebook

		self.ribbon.enable.changed.connect(self.callback_enable)
		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		css_apply(self.notebook,"style_h.css")
		self.notebook.setTabBar(QHTabBar())
		self.notebook.setTabPosition(QTabWidget.West)


		self.notebook.setMovable(True)

		self.notebook.currentChanged.connect(self.changed_click)

		self.load_tabs()

		self.main_vbox.addWidget(self.notebook)

		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)

		self.setLayout(self.main_vbox)

		self.fit_configure_window=None





