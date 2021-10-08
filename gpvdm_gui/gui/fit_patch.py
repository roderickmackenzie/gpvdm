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

## @package fit_patch
#  Patch the fit.
#

import os

from scan_select import select_param

from icon_lib import icon_get
from str2bool import str2bool

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon

from gpvdm_select import gpvdm_select

from cal_path import get_sim_path
from gpvdm_tab import gpvdm_tab
from json_fit import json_fit_patch_line
from gpvdm_json import gpvdm_data

class fit_patch(QWidget):

	def insert_row(self,i,var,val,json_var):
		self.tab.blockSignals(True)
		self.tab.insertRow(i)

		self.item = gpvdm_select()
		self.item.setText(var)
		self.item.button.clicked.connect(self.callback_show_list)
		self.tab.setCellWidget(i,0,self.item)

		item = QTableWidgetItem(val)
		self.tab.setItem(i,1,item)

		item = QTableWidgetItem(json_var)
		self.tab.setItem(i,2,item)

		self.tab.blockSignals(False)

	def callback_show_list(self):
		self.select_param_window.update()
		self.select_param_window.show()
		
	def callback_add_item(self):
		self.insert_row(self.tab.rowCount(),"Variable","value",_("JSON Variable"))
		self.save_combo()

	def callback_delete_item(self):
		self.tab.remove()
		self.save_combo()

	def save_combo(self):
		data=gpvdm_data()
		self.data.segments=[]
		for i in range(0,self.tab.rowCount()):
			a=json_fit_patch_line()
			a.human_path=self.tab.get_value(i, 0)
			a.val=self.tab.get_value(i, 1)
			a.json_path=self.tab.get_value(i, 2)
			self.data.segments.append(a)

		data.save()


	def tab_changed(self):
		self.save_combo()
		

	def create_model(self):
		self.tab.clear()
		self.tab.setColumnCount(3)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Variable"), _("Value"), _("JSON Variable")])
		self.tab.setColumnWidth(0, 300)
		self.tab.setColumnWidth(2, 20)

		pos=0
		for s in self.data.segments:
			self.insert_row(pos,s.human_path,s.val,s.json_path)
			print(s.human_path,s.val,s.json_path)
			pos=pos+1

	def __init__(self,data):
		QWidget.__init__(self)
		self.data=data

		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		self.vbox.addWidget(toolbar)

		self.tab = gpvdm_tab(toolbar=toolbar)

		self.tab.tb_add.triggered.connect(self.callback_add_item)
		self.tab.user_remove_rows.connect(self.callback_delete_item)
		self.tab.tb_down.triggered.connect(self.callback_move_down)
		self.tab.tb_up.triggered.connect(self.callback_move_up)


		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)

		self.select_param_window=select_param(self.tab)
		self.select_param_window.human_path_col=0
		self.select_param_window.json_path_col=2
		self.select_param_window.set_save_function(self.save_combo)

		self.create_model()

		self.tab.cellChanged.connect(self.tab_changed)

		self.vbox.addWidget(self.tab)


		self.setLayout(self.vbox)

	def callback_move_down(self):
		self.tab.move_down()

	def callback_move_up(self):
		self.tab.move_up()
