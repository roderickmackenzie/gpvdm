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

## @package tab_banned_files
#  A window to list files which will not be written to disk
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
from gpvdm_json import gpvdm_data
from json_dump import json_banned_file

class tab_banned_files(QWidget):

	def callback_show_list(self):
		self.select_param_window.show()

	def __init__(self):
		QWidget.__init__(self)

		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		self.vbox.addWidget(toolbar)

		self.tab2 = gpvdm_tab2(toolbar=toolbar)
		self.tab2.set_tokens(["banned_enabled","banned_file_name"])
		self.tab2.set_labels([_("Enabled"),_("File to ban")])

		self.tab2.json_search_path="gpvdm_data().dump.banned_files.segments"
		self.tab2.setColumnWidth(1, 400)
		self.tab2.setColumnWidth(2, 100)

		self.tab2.base_obj=json_banned_file()
		self.tab2.populate()
		self.tab2.changed.connect(self.callback_save)

		self.vbox.addWidget(self.tab2)

		self.setLayout(self.vbox)



	def callback_save(self):
		gpvdm_data().save()

