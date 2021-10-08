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

## @package fit_vars
#  A window to define the fit variables.
#
import os

from token_lib import tokens
from str2bool import str2bool

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QAbstractItemView, QMenuBar, QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon

from gpvdm_tab2 import gpvdm_tab2
from scan_select import select_param
from gpvdm_json import gpvdm_data
from json_ml import json_ml_patch_item

class tab_ml_patch(QWidget):

	def callback_show_list(self):
		self.select_param_window.show()

	def __init__(self,uid):
		QWidget.__init__(self)
		self.uid=uid
		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		self.vbox.addWidget(toolbar)


		self.tab2 = gpvdm_tab2(toolbar=toolbar)
		self.tab2.set_tokens(["ml_patch_enabled","human_var","ml_patch_val","json_var"])
		self.tab2.set_labels([_("Enabled"),_("Variable"), _("Value"),_("JSON Variable")])

		self.tab2.json_search_path="gpvdm_data().ml"
		self.tab2.uid=uid
		self.tab2.postfix="ml_patch.segments"

		self.tab2.fixup_new_row=self.fixup_new_row
		self.tab2.setColumnWidth(1, 400)
		self.tab2.setColumnWidth(2, 100)
		self.tab2.setColumnWidth(3, 100)
		self.tab2.setColumnWidth(4, 20)
		self.tab2.base_obj=json_ml_patch_item()
		self.tab2.populate()
		self.tab2.changed.connect(self.callback_save)
		self.tab2.callback_a=self.callback_show_list

		self.vbox.addWidget(self.tab2)

		self.select_param_window=select_param(self.tab2)
		self.select_param_window.human_path_col=1
		self.select_param_window.json_path_col=3

		self.select_param_window.update()

		self.select_param_window.set_save_function(self.callback_save)

		self.setLayout(self.vbox)

	def fixup_new_row(self,row):
		self.tab2.cellWidget(row, 1).button.clicked.connect(self.callback_show_list)


	def callback_save(self):
		gpvdm_data().save()

