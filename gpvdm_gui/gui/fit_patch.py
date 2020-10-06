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

from inp import inp_save_lines_to_file
from inp import inp_load_file

from gpvdm_select import gpvdm_select

from cal_path import get_sim_path
from gpvdm_tab import gpvdm_tab

class fit_patch(QWidget):

	def insert_row(self,i,file_name,token,path,value):
		self.tab.blockSignals(True)
		self.tab.insertRow(i)

		item = QTableWidgetItem(file_name)
		self.tab.setItem(i,0,item)

		item = QTableWidgetItem(token)
		self.tab.setItem(i,1,item)


		self.item = gpvdm_select()
		self.item.setText(path)
		self.item.button.clicked.connect(self.callback_show_list)

		self.tab.setCellWidget(i,2,self.item)

		item = QTableWidgetItem(value)
		self.tab.setItem(i,3,item)
		self.tab.blockSignals(False)

	def callback_show_list(self):
		self.select_param_window.update()
		self.select_param_window.show()
		
	def callback_add_item(self):
		self.insert_row(self.tab.rowCount(),"File","token","path",_("value"))
		self.save_combo()

	def callback_delete_item(self):
		self.tab.remove()
		self.save_combo()

	def save_combo(self):
		lines=[]
		for i in range(0,self.tab.rowCount()):
			lines.append(str(self.tab.get_value(i, 1)))
			lines.append(str(self.tab.get_value(i, 0)))
			lines.append(str(self.tab.get_value(i, 2)))
			lines.append(str(self.tab.get_value(i, 3)))

		lines.append("#end")
		#print("save as",self.file_name)
		inp_save_lines_to_file(self.file_name,lines)


	def tab_changed(self):
		self.save_combo()
		

	def create_model(self):
		lines=[]
		self.tab.clear()
		self.tab.setColumnCount(4)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("File"), _("Token"), _("Path"), _("Values")])
		self.tab.setColumnWidth(2, 300)
		self.file_name=os.path.join(get_sim_path(),"fit_patch"+str(self.index)+".inp")

		lines=inp_load_file(self.file_name)
		if lines!=False:

			pos=0
			mylen=len(lines)
			while(1):
				t=lines[pos]
				if t=="#end":
					break
				pos=pos+1

				f=lines[pos]
				if f=="#end":
					break
				pos=pos+1

				path=lines[pos]
				if f=="#end":
					break
				pos=pos+1
				
				v=lines[pos]
				if v=="#end":
					break
				pos=pos+1

				self.insert_row(self.tab.rowCount(),f,t,path,v)

				if pos>mylen:
					break

	def __init__(self,index):
		QWidget.__init__(self)


		self.index=index
		
		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		self.tb_save = QAction(icon_get("list-add"), _("Add"), self)
		self.tb_save.triggered.connect(self.callback_add_item)
		toolbar.addAction(self.tb_save)

		self.tb_save = QAction(icon_get("list-remove"), _("Minus"), self)
		self.tb_save.triggered.connect(self.callback_delete_item)
		toolbar.addAction(self.tb_save)

		self.tb_down = QAction(icon_get("go-down"), _("Move down"), self)
		self.tb_down.triggered.connect(self.callback_move_down)
		toolbar.addAction(self.tb_down)

		self.tb_up = QAction(icon_get("go-up"), _("Move up"), self)
		self.tb_up.triggered.connect(self.callback_move_up)
		toolbar.addAction(self.tb_up)
	
		self.vbox.addWidget(toolbar)

		self.tab = gpvdm_tab()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)

		self.select_param_window=select_param(self.tab)
		self.select_param_window.set_save_function(self.save_combo)

		self.create_model()

		self.tab.cellChanged.connect(self.tab_changed)

		self.vbox.addWidget(self.tab)


		self.setLayout(self.vbox)

	def callback_move_down(self):
		self.tab.move_down()

	def callback_move_up(self):
		self.tab.move_up()
