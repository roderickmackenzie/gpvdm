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

## @package duplicate
#  Window to configure duplicated variables for fitting.
#

import os

from scan_select import select_param

from icon_lib import icon_get

from gpvdm_select import gpvdm_select

from str2bool import str2bool

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QAbstractItemView, QMenuBar, QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon

from gtkswitch import gtkswitch

from gpvdm_tab2 import gpvdm_tab2
from gpvdm_json import gpvdm_data
from json_fit import json_duplicate_line


class fit_duplicate(QWidget):

	def __init__(self):
		QWidget.__init__(self)
		self.setWindowTitle(_("Fit variables duplicate window")+" - https://www.gpvdm.com")   
		self.setWindowIcon(icon_get("duplicate"))
		data=gpvdm_data()
		self.data=data.fits.duplicate

		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))


		self.vbox.addWidget(toolbar)

		self.tab = gpvdm_tab2(toolbar=toolbar)
		self.tab.set_tokens(["duplicate_var_enabled","human_src","human_dest","multiplier","json_src","json_dest"])
		self.tab.set_labels([_("Enabled"),_("Source")+" (x)", _("Destination")+" (y)", _("Function")+" y=f(x)", _("Source (JSON)"), _("Destination (JSON)")])
		self.tab.json_search_path="gpvdm_data().fits.duplicate.segments"
		self.tab.fixup_new_row=self.fixup_new_row
		self.tab.populate()
		self.tab.base_obj=json_duplicate_line()
		self.tab.changed.connect(self.callback_save)
		self.tab.callback_a=self.callback_show_list_a
		self.tab.callback_b=self.callback_show_list_b

		self.tab.setColumnWidth(0, 150)
		self.tab.setColumnWidth(1, 350)
		self.tab.setColumnWidth(2, 350)
		self.tab.setColumnWidth(3, 100)
		self.tab.setColumnWidth(4, 20)
		self.tab.setColumnWidth(5, 20)

		self.vbox.addWidget(self.tab)

		self.select_param_window_a=select_param(self.tab)
		self.select_param_window_a.human_path_col=1
		self.select_param_window_a.json_path_col=4
		self.select_param_window_a.update()
		self.select_param_window_a.set_save_function(self.callback_save)

		self.select_param_window_b=select_param(self.tab)
		self.select_param_window_b.human_path_col=2
		self.select_param_window_b.json_path_col=5
		self.select_param_window_b.update()
		self.select_param_window_b.set_save_function(self.callback_save)

		self.setLayout(self.vbox)

	def callback_save(self):
		gpvdm_data().save()

	def callback_show_list_a(self):
		self.select_param_window_a.show()

	def callback_show_list_b(self):
		self.select_param_window_b.show()

	def fixup_new_row(self,row):
		self.tab.cellWidget(row, 1).button.clicked.connect(self.callback_show_list_a)
		self.tab.cellWidget(row, 2).button.clicked.connect(self.callback_show_list_b)

