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

from inp import inp_load_file
from inp import inp_save_lines_to_file
from gpvdm_select import gpvdm_select

from open_save_dlg import open_as_filter
from error_dlg import error_dlg
from cal_path import get_sim_path
from gtkswitch import gtkswitch
from str2bool import str2bool

from gpvdm_tab import gpvdm_tab
from scan_select import select_param
from gpvdm_json import gpvdm_data
from json_fit import json_fit_rules_line

class fit_rules(QWidget):

	def insert_row(self,i,enabled,human_x,human_y,fun,json_x,json_y):
		self.tab.blockSignals(True)

		self.item = gtkswitch()
		self.item.set_value(str2bool(enabled))
		self.item.changed.connect(self.tab_changed)
		self.tab.setCellWidget(i,0,self.item)

		self.item = gpvdm_select()
		self.item.setText(human_x)
		self.item.button.clicked.connect(self.callback_show_list_x)
		self.tab.setCellWidget(i,1,self.item)

		self.item = gpvdm_select()
		self.item.setText(human_y)
		self.item.button.clicked.connect(self.callback_show_list_y)
		self.tab.setCellWidget(i,2,self.item)

		item = QTableWidgetItem(str(fun))
		self.tab.setItem(i,3,item)

		item = QTableWidgetItem(str(json_x))
		self.tab.setItem(i,4,item)

		item = QTableWidgetItem(str(json_y))
		self.tab.setItem(i,5,item)

		self.tab.blockSignals(False)
		
	def callback_add_item(self):
		pos=self.tab.insert_row()
		self.insert_row(pos,True,_("x"),_("y"),"100.0*(x>y)",_("JSON Variable x"),_("JSON Variable y"))
		self.save_combo()
		

	def callback_show_list_x(self):
		self.select_param_window_x.show()

	def callback_show_list_y(self):
		self.select_param_window_y.show()

	def callback_delete_item(self):
		self.tab.remove()
		self.save_combo()

	def save_combo(self):
		data=gpvdm_data()
		self.data.segments=[]
		for i in range(0,self.tab.rowCount()):
			a=json_fit_rules_line()
			a.enabled=self.tab.get_value(i, 0)
			a.human_x=self.tab.get_value(i, 1)
			a.human_y=self.tab.get_value(i, 2)
			a.function=self.tab.get_value(i, 3)
			a.json_x=self.tab.get_value(i, 4)
			a.json_y=self.tab.get_value(i, 5)
			self.data.segments.append(a)

		data.save()

	def tab_changed(self):
		self.save_combo()

	def create_model(self):
		self.tab.clear()
		self.tab.setColumnCount(6)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Enabled"),_("x"), _("y"), _("Function"), _("JSON x"), _("JSON y")])
		self.tab.setColumnWidth(0, 100)
		self.tab.setColumnWidth(1, 300)
		self.tab.setColumnWidth(2, 300)
		self.tab.setColumnWidth(3, 100)
		self.tab.setColumnWidth(4, 20)
		self.tab.setColumnWidth(5, 20)

		for s in self.data.segments:
			pos=self.tab.insert_row()
			self.insert_row(pos,s.enabled,s.human_x,s.human_y,s.function,s.json_x,s.json_y)

	def on_move_down(self):
		self.tab.move_down()
		self.save_combo()
		
	def on_move_up(self):
		self.tab.move_up()
		self.save_combo()

	def __init__(self):
		QWidget.__init__(self)
		data=gpvdm_data()
		self.data=data.fits.rules

		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))


		self.vbox.addWidget(toolbar)

		self.tab = gpvdm_tab(toolbar=toolbar)

		self.tab.tb_add.triggered.connect(self.callback_add_item)

		self.tab.user_remove_rows.connect(self.callback_delete_item)

		self.tab.tb_down.triggered.connect(self.on_move_down)

		self.tab.tb_up.triggered.connect(self.on_move_up)


		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)
		self.create_model()

		self.tab.cellChanged.connect(self.tab_changed)

		self.vbox.addWidget(self.tab)

		self.select_param_window_x=select_param(self.tab)
		self.select_param_window_x.human_path_col=1
		self.select_param_window_x.json_path_col=4
		self.select_param_window_x.update()
		self.select_param_window_x.set_save_function(self.save_combo)


		self.select_param_window_y=select_param(self.tab)
		self.select_param_window_y.human_path_col=2
		self.select_param_window_y.json_path_col=5
		self.select_param_window_y.update()
		self.select_param_window_y.set_save_function(self.save_combo)


		self.setLayout(self.vbox)
