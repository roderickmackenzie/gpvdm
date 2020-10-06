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

## @package fxexperiment
#  Main fx domain experiment window
#

import os
from gui_util import dlg_get_text
import webbrowser
from inp import inp_update_token_value
from cvexperiment_tab import cvexperiment_tab
from util_zip import zip_lsdir
from inp import inp
from inp import inp_copy_file
from inp import inp_remove_file
from inp import inp_get_token_value

from util import strextract_interger
from global_objects import global_object_get
from icon_lib import icon_get
from global_objects import global_object_register
from code_ctrl import enable_betafeatures
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
from experiment_util import experiment_new_filename
from cv_ribbon import cv_ribbon

from progress_class import progress_class
from process_events import process_events
from global_objects import global_object_run
from util import is_numbered_file

class cv_editor(QWidgetSavePos):

	changed = pyqtSignal()

	def callback_help(self):
		webbrowser.open("https://www.gpvdm.com/man/index.html")

	def callback_add_page(self,file_name):
		self.add_page(file_name)
		global_object_run("ribbon_sim_mode_update")
		self.changed.emit()


	def callback_tab_changed(self):
		return

	def load_tabs(self):
		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		process_events()

		file_list=zip_lsdir(os.path.join(get_sim_path(),"sim.gpvdm"))
		files=[]
		for i in range(0,len(file_list)):
			if is_numbered_file(file_list[i],"cv")==True:
				name=inp_get_token_value(file_list[i], "#sim_menu_name")
				files.append([name,file_list[i]])

		files.sort()

		for i in range(0,len(files)):
			self.add_page(files[i][1])

			progress_window.set_fraction(float(i)/float(len(files)))
			progress_window.set_text(_("Loading")+" "+files[i][0])
			process_events()

		progress_window.stop()

	def clear_pages(self):
		self.notebook.clear()

	def add_page(self,filename):
		name=inp_get_token_value(filename, "#sim_menu_name")
		tab=cvexperiment_tab()
		print(filename[2:-4],filename)
		tab.init(int(filename[2:-4]))
		self.notebook.addTab(tab,name.split("@")[0])

	def callback_mode_changed(self):
		return
		#tab = self.notebook.currentWidget()
		#tab.update_mode(self.ribbon.mode.mode.currentText())

	#def callback_save(self):
	#	tab = self.notebook.currentWidget()
	#	tab.save_image()

	def __init__(self):
		QWidgetSavePos.__init__(self,"cv_editor")
		self.setMinimumSize(1200, 700)

		self.main_vbox = QVBoxLayout()

		self.setWindowTitle(_("Capacitance voltage expriment editor")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("cv"))


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		self.ribbon=cv_ribbon()

		#self.ribbon.tb_save.triggered.connect(self.callback_save)

		self.ribbon.order_widget.changed.connect(self.callback_changed)
		self.ribbon.order_widget.added.connect(self.callback_add_page)

		self.main_vbox.addWidget(self.ribbon)

		self.notebook = QTabWidget()
		self.notebook.setTabBar(QHTabBar())

		self.notebook.setTabPosition(QTabWidget.West)
		self.notebook.setMovable(True)
		self.notebook.currentChanged.connect(self.callback_tab_changed)

		self.ribbon.order_widget.notebook_pointer=self.notebook

		self.load_tabs()

		self.main_vbox.addWidget(self.notebook)


		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)


		self.setLayout(self.main_vbox)

	def callback_changed(self):
		global_object_run("ribbon_sim_mode_update")
		self.changed.emit()

