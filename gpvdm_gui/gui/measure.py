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

## @package measure
#  The main measurement window for setting up measurement experiments.
#


import os

from gui_util import dlg_get_text
from token_lib import tokens

from scan_io import scan_push_to_hpc
from scan_io import scan_import_from_hpc
from cal_path import get_exe_command
from icon_lib import icon_get
from str2bool import str2bool

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon

from inp import inp_save_lines_to_file
from inp import inp_load_file
from inp import inp
from inp import inp_copy_file
from inp import inp_update_token_value
from inp import inp_get_token_value
from inp import inp_remove_file

from gpvdm_select import gpvdm_select

from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos
from window_list import resize_window_to_be_sane
from measure_ribbon import measure_ribbon
from measure_tab import measure_tab
from QHTabBar import QHTabBar
from util_zip import zip_lsdir

from util import strextract_interger



def measure_new_filename():
	for i in range(0,20):
		pulse_name="measure"+str(i)+".inp"
		if inp().isfile(os.path.join(get_sim_path(),pulse_name))==False:
			return i
	return -1

class measure(QWidgetSavePos):


	def add_page(self,file_name):
		tab=measure_tab(file_name)
		name=inp_get_token_value(file_name, "#tab_name")
		self.notebook.addTab(tab,name)

	def __init__(self):
		QWidgetSavePos.__init__(self,"measure_window")
		resize_window_to_be_sane(self,0.5,0.7)

		self.setWindowIcon(icon_get("measure"))
		self.setWindowTitle(_("Measurment editor")+" (https://www.gpvdm.com)")

		self.vbox=QVBoxLayout()

		self.ribbon=measure_ribbon()
		self.ribbon.order_widget.added.connect(self.callback_add_page)


		self.vbox.addWidget(self.ribbon)

		self.notebook = QTabWidget()
		self.notebook.setTabBar(QHTabBar())
		self.notebook.setTabPosition(QTabWidget.West)
		self.notebook.setMovable(True)

		self.ribbon.order_widget.notebook_pointer=self.notebook
		self.ribbon.order_widget.load_tabs()

		self.vbox.addWidget(self.notebook)


		self.setLayout(self.vbox)


	def callback_add_page(self,new_filename):
		self.add_page(new_filename)


	def remove_invalid(self,input_name):
		return input_name.replace (" ", "_")

