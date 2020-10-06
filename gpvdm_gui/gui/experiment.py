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

## @package experiment
#  The main experiment window, used for configuring time domain experiments.
#

import os
from gui_util import dlg_get_text
import webbrowser
from code_ctrl import enable_betafeatures
from inp import inp_update_token_value
from util_zip import zip_lsdir
from inp import inp
from inp import inp_copy_file
from inp import inp_remove_file
from util import strextract_interger
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
from experiment_tab import experiment_tab
from QHTabBar import QHTabBar
from gui_util import yes_no_dlg
from PyQt5.QtCore import pyqtSignal
from util import wrap_text
from QWidgetSavePos import QWidgetSavePos
from cal_path import get_sim_path

from inp import inp_get_token_value
from experiment_util import experiment_new_filename

from css import css_apply

from progress_class import progress_class
from process_events import process_events

from timedomain_ribbon import timedomain_ribbon


class experiment(QWidgetSavePos):

	changed = pyqtSignal()

	def update(self):
		for item in self.notebook.get_children():
			item.update()

	def callback_help(self):
		webbrowser.open('http://www.gpvdm.com/man/index.html')


	def callback_add_page(self,file_name):
		self.add_page(file_name)
		self.changed.emit()

	def callback_save(self):
		tab = self.notebook.currentWidget()
		tab.image_save()

	def callback_changed(self):
		self.changed.emit()


	def remove_invalid(self,input_name):
		return input_name.replace (" ", "_")

	def callback_laser_start_time(self):
		tab = self.notebook.currentWidget()
		tab.tmesh.callback_laser()

	def callback_start_time(self):
		tab = self.notebook.currentWidget()
		tab.tmesh.callback_start_time()


	def load_tabs(self):

		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		process_events()

		file_list=zip_lsdir(os.path.join(get_sim_path(),"sim.gpvdm"))
		files=[]
		for i in range(0,len(file_list)):
			if file_list[i].startswith("pulse") and file_list[i].endswith(".inp"):
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
		tab=experiment_tab(int(filename[5:-4]))
		self.notebook.addTab(tab,name.split("@")[0])

	def switch_page(self):
		tab = self.notebook.currentWidget()
		self.ribbon.tb_lasers.update("pulse"+str(tab.tmesh.index)+".inp")


	def __init__(self):
		QWidgetSavePos.__init__(self,"experiment")
		self.main_vbox = QVBoxLayout()


		self.setMinimumSize(1200, 700)
		self.setWindowTitle(_("Time domain experiment window")+" (https://www.gpvdm.com)")
		self.setWindowIcon(icon_get("icon"))

		self.ribbon=timedomain_ribbon()

		self.ribbon.tb_save.triggered.connect(self.callback_save)

		self.ribbon.order_widget.changed.connect(self.callback_changed)
		self.ribbon.order_widget.added.connect(self.callback_add_page)

		self.ribbon.tb_laser_start_time.triggered.connect(self.callback_laser_start_time)

		self.ribbon.tb_start.triggered.connect(self.callback_start_time)


		self.main_vbox.addWidget(self.ribbon)


		self.notebook = QTabWidget()
		css_apply(self.notebook ,"style_h.css")

		self.notebook.setTabBar(QHTabBar())
		self.notebook.setTabPosition(QTabWidget.West)
		self.notebook.setMovable(True)
		self.ribbon.order_widget.notebook_pointer=self.notebook

		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		self.load_tabs()

		self.main_vbox.addWidget(self.notebook)


		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)


		self.setLayout(self.main_vbox)

		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()

