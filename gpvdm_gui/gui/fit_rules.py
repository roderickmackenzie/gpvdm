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

## @package fit_rules
#  A window to define the math fitting rules 
#
import os

from token_lib import tokens

from icon_lib import icon_get
from str2bool import str2bool

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QAbstractItemView, QMenuBar, QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon

from gpvdm_select import gpvdm_select

from open_save_dlg import open_as_filter
from error_dlg import error_dlg
from gtkswitch import gtkswitch
from str2bool import str2bool

from scan_select import select_param
from gpvdm_json import gpvdm_data
from json_fit import json_fit_rules_line
from gpvdm_tab2 import gpvdm_tab2

class fit_rules(QWidget):

	def __init__(self):
		QWidget.__init__(self)
		data=gpvdm_data()
		self.data=data.fits.rules

		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))


		self.vbox.addWidget(toolbar)

		#tab
		self.tab2 = gpvdm_tab2(toolbar=toolbar)
		self.tab2.set_tokens(["fit_rule_enabled","human_x","human_y","function","json_x","json_y"])
		self.tab2.set_labels([_("Enabled"),_("x"), _("y"), _("Function"), _("JSON x"), _("JSON y")])


		self.tab2.json_search_path="gpvdm_data().fits.rules.segments"
		self.tab2.fixup_new_row=self.fixup_new_row
		self.tab2.setColumnWidth(0, 100)
		self.tab2.setColumnWidth(1, 300)
		self.tab2.setColumnWidth(2, 300)
		self.tab2.setColumnWidth(3, 100)
		self.tab2.setColumnWidth(4, 20)
		self.tab2.setColumnWidth(5, 20)
		self.tab2.base_obj=json_fit_rules_line()
		self.tab2.populate()

		self.tab2.changed.connect(self.callback_save)
		self.tab2.callback_a=self.callback_show_list_a
		self.tab2.callback_b=self.callback_show_list_b
		self.vbox.addWidget(self.tab2)
		####
		self.select_param_window_a=select_param(self.tab2)
		self.select_param_window_a.human_path_col=1
		self.select_param_window_a.json_path_col=4
		self.select_param_window_a.update()
		self.select_param_window_a.set_save_function(self.callback_save)

		self.select_param_window_b=select_param(self.tab2)
		self.select_param_window_b.human_path_col=2
		self.select_param_window_b.json_path_col=5
		self.select_param_window_b.update()
		self.select_param_window_b.set_save_function(self.callback_save)
		####

		self.setLayout(self.vbox)

	def callback_save(self):
		gpvdm_data().save()

	def callback_show_list_a(self):
		self.select_param_window_a.show()

	def callback_show_list_b(self):
		self.select_param_window_b.show()

	def fixup_new_row(self,row):
		self.tab2.cellWidget(row, 1).button.clicked.connect(self.callback_show_list_a)
		self.tab2.cellWidget(row, 2).button.clicked.connect(self.callback_show_list_b)
