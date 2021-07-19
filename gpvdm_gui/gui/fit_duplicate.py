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

from gpvdm_tab import gpvdm_tab
from gpvdm_json import gpvdm_data
from json_fit import json_duplicate_line

class fit_duplicate(QWidget):

	def insert_row(self,i,enabled,src_path,dst_path,mul,src_json_path,dest_json_path):
		self.tab.blockSignals(True)
		self.tab.insertRow(i)

		item = gtkswitch()
		item.set_value(str2bool(enabled))
		item.changed.connect(self.tab_changed)
		self.tab.setCellWidget(i,0,item)

		self.item = gpvdm_select()
		self.item.setText(src_path)
		self.item.button.clicked.connect(self.callback_show_list_src)
		self.tab.setCellWidget(i,1,self.item)

		self.item = gpvdm_select()
		self.item.setText(dst_path)
		self.item.button.clicked.connect(self.callback_show_list_dest)
		self.tab.setCellWidget(i,2,self.item)

		item = QTableWidgetItem(str(mul))
		self.tab.setItem(i,3,item)

		item = QTableWidgetItem(src_json_path)
		self.tab.setItem(i,4,item)

		item = QTableWidgetItem(dest_json_path)
		self.tab.setItem(i,5,item)

		self.tab.blockSignals(False)

	def callback_show_list_src(self):
		#self.select_param_window_src.update()
		self.select_param_window.human_path_col=1
		self.select_param_window.json_path_col=4
		self.select_param_window.dest_treeview=self.tab
		self.select_param_window.show()

	def callback_show_list_dest(self):
		#self.select_param_window_dest.update()
		self.select_param_window.human_path_col=2
		self.select_param_window.json_path_col=5
		self.select_param_window.dest_treeview=self.tab
		self.select_param_window.show()
		
	def callback_add_item(self):
		self.insert_row(self.tab.rowCount(),True,_("Source"),_("Dest"),_("Multiplier"),_("JSON Soruce"),_("JSON Dest"))
		self.save_combo()

	def callback_delete_item(self):
		self.tab.remove()
		self.save_combo()

	def save_combo(self):
		data=gpvdm_data()
		self.data.segments=[]
		for i in range(0,self.tab.rowCount()):
			a=json_duplicate_line()
			a.enabled=self.tab.get_value(i, 0)
			a.human_src=self.tab.get_value(i, 1)
			a.human_dest=self.tab.get_value(i, 2)
			a.multiplier=self.tab.get_value(i, 3)
			a.json_src=self.tab.get_value(i, 4)
			a.json_dest=self.tab.get_value(i, 5)
			self.data.segments.append(a)

		data.save()


	def tab_changed(self):
		self.save_combo()

	def on_move_down(self):
		self.tab.move_down()
		self.save_combo()
		
	def on_move_up(self):
		self.tab.move_up()
		self.save_combo()

	def create_model(self):
		self.tab.clear()
		self.tab.setColumnCount(6)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Enabled"),_("Source")+" (x)", _("Destination")+" (y)", _("Function")+" y=f(x)", _("Source (JSON)"), _("Destination (JSON)") ])
		self.tab.setColumnWidth(0, 150)
		self.tab.setColumnWidth(1, 350)
		self.tab.setColumnWidth(2, 350)
		self.tab.setColumnWidth(3, 100)
		self.tab.setColumnWidth(4, 20)
		self.tab.setColumnWidth(5, 20)

		pos=0
		for s in self.data.segments:
			self.insert_row(pos,s.enabled,s.human_src,s.human_dest,s.multiplier,s.json_src,s.json_dest)
			pos=pos+1

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

		self.tab = gpvdm_tab(toolbar=toolbar)

		self.tab.tb_add.triggered.connect(self.callback_add_item)
		self.tab.user_remove_rows.connect(self.callback_delete_item)
		self.tab.tb_down.triggered.connect(self.on_move_down)
		self.tab.tb_up.triggered.connect(self.on_move_up)


		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)
		self.create_model()

		self.tab.cellChanged.connect(self.tab_changed)

		self.select_param_window=select_param(self.tab)
		self.select_param_window.set_save_function(self.save_combo)
		self.select_param_window.update()

		self.vbox.addWidget(self.tab)


		self.setLayout(self.vbox)
