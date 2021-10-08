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
from json_ml import json_ml_sims_item
from tab_ml_patch import tab_ml_patch

class tab_ml_sims(QWidget):

	def __init__(self,uid):
		QWidget.__init__(self)
		self.uid=uid

		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		self.vbox.addWidget(toolbar)


		self.tab2 = gpvdm_tab2(toolbar=toolbar)
		self.tab2.set_tokens(["ml_sim_enabled","sim_name","ml_edit_sim"])
		self.tab2.set_labels([_("Enabled"),_("Sim name"),_("Patch")])

		data=gpvdm_data().ml.find_object_by_id(self.uid)
		index=gpvdm_data().ml.segments.index(data)
		self.tab2.fixup_new_row=self.fixup_new_row
		self.tab2.json_search_path="gpvdm_data().ml.segments["+str(index)+"].ml_sims.segments"
		self.tab2.setColumnWidth(1, 400)
		self.tab2.setColumnWidth(2, 100)
		self.tab2.setColumnWidth(3, 100)
		self.tab2.setColumnWidth(4, 100)
		self.tab2.setColumnWidth(5, 20)
		self.tab2.base_obj=json_ml_sims_item()
		self.tab2.populate()
		self.tab2.changed.connect(self.callback_save)

		self.vbox.addWidget(self.tab2)

		self.setLayout(self.vbox)


	def callback_save(self):
		gpvdm_data().save()

	def fixup_new_row(self,row):
		data=gpvdm_data().ml.find_object_by_id(self.uid)
		index=gpvdm_data().ml.segments.index(data)
		self.tab2.cellWidget(row, 2).uid=gpvdm_data().ml.segments[index].ml_sims.segments[row].id
		self.tab2.cellWidget(row, 2).changed.connect(self.callback_edit_clicked)

	def callback_edit_clicked(self,uid):
		print(uid)
		w=tab_ml_patch(uid)
		w.setMinimumSize(700,700)
		w.show()
