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

## @package fit_vars
#  A window to define the fit variables.
#
import os

from token_lib import tokens

from cal_path import get_exe_command
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

class fit_vars(QWidget):

	def insert_row(self,i,enabled,f,t,p,min,max,error_function):
		self.tab.blockSignals(True)

		self.item = gtkswitch()
		self.item.set_value(str2bool(enabled))
		self.item.changed.connect(self.tab_changed)
		self.tab.setCellWidget(i,0,self.item)
		
		item = QTableWidgetItem(f)
		self.tab.setItem(i,1,item)

		item = QTableWidgetItem(t)
		self.tab.setItem(i,2,item)


		self.item = gpvdm_select()
		self.item.setText(p)
		self.item.button.clicked.connect(self.callback_show_list)

		self.tab.setCellWidget(i,3,self.item)

		item = QTableWidgetItem(min)
		self.tab.setItem(i,4,item)

		item = QTableWidgetItem(max)
		self.tab.setItem(i,5,item)

		item = QTableWidgetItem(error_function)
		self.tab.setItem(i,6,item)

		self.tab.blockSignals(False)
		
	def callback_add_item(self):
		pos=self.tab.insert_row()
		self.insert_row(pos,"true",_("File"),_("token"),_("path"),_("min"),_("max"),"100")
		self.save_combo()
		
	def callback_open(self):
		file_name=open_as_filter(self,"dat (*.dat);;csv (*.csv);;txt (*.txt);;inp (*.inp);;gmat (*.gmat)")

		if file_name!=None:
			lines=[]
			lines=inp_load_file(file_name)
			begin=-1
			end=-1

			for i in range(0,len(lines)):
				if lines[i]=="#data":
					begin=i
				if lines[i]=="#end":
					end=i
				
			if begin==-1 or end==-1:
				error_dlg(self,_("The data file does not have a #begin or #end token indicating where the data starts and ends."))
				return

			rel_path=os.path.relpath(file_name, get_sim_path())
			self.tab.insertRow(self.tab.rowCount())
			self.insert_row(self.tab.rowCount(),"true",rel_path,"#data",rel_path)
			self.save_combo()

	def callback_show_list(self):
		self.select_param_window.show()

	def callback_delete_item(self):
		self.tab.remove()
		self.save_combo()

	def save_combo(self):
		lines=[]

		for i in range(0,self.tab.rowCount()):
			lines.append(str(self.tab.get_value(i, 2)))
			lines.append(str(self.tab.get_value(i, 1)))
			lines.append(str(self.tab.get_value(i, 3)))
			lines.append(str(self.tab.get_value(i, 0)))			
			lines.append(str(self.tab.get_value(i, 4)))
			lines.append(str(self.tab.get_value(i, 5)))
			lines.append(str(self.tab.get_value(i, 6)))


		lines.append("#ver")
		lines.append("1.1")
		lines.append("#end")

		inp_save_lines_to_file(self.file_name,lines)

	def tab_changed(self):
		self.save_combo()

	def create_model(self):
		self.tab.clear()
		self.tab.setColumnCount(7)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Enabled"),_("File"), _("Token"), _("Path"), _("Min"), _("Max"), _("Error function")])
		self.tab.setColumnWidth(2, 270)
		self.tab.setColumnWidth(3, 400)
		self.file_name=os.path.join(get_sim_path(),"fit_vars.inp")

		lines=[]
		pos=0
		lines=inp_load_file(self.file_name)
		if inp_load_file(self.file_name)!=False:
			mylen=len(lines)
			line=0
			while(1):
				token=lines[pos]
				if token=="#end":
					break
				pos=pos+1

				file=lines[pos]
				if file=="#end":
					break
				pos=pos+1

				path=lines[pos]
				if path=="#end":
					break
				pos=pos+1

				enabled=lines[pos]
				if enabled=="#end":
					break
				pos=pos+1

				min=lines[pos]
				if enabled=="#end":
					break
				pos=pos+1

				max=lines[pos]
				if enabled=="#end":
					break
				pos=pos+1

				error_function=lines[pos]
				if enabled=="#end":
					break
				pos=pos+1

				self.tab.insertRow(line)
				self.insert_row(line,enabled,file,token,path,min,max,error_function)

				if pos>mylen:
					break

				line=line+1

	def on_move_down(self):
		self.tab.move_down()
		self.save_combo()
		
	def on_move_up(self):
		self.tab.move_up()
		self.save_combo()

	def __init__(self):
		QWidget.__init__(self)

		self.setWindowTitle(_("Fit vars window - gpvdm"))   
		self.setWindowIcon(icon_get("fit"))
		#self.setFixedSize(900, 700)
		self.setMinimumWidth(1200)

		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		self.tb_save = QAction(icon_get("list-add"), _("Add line"), self)
		self.tb_save.triggered.connect(self.callback_add_item)
		toolbar.addAction(self.tb_save)

		self.tb_save = QAction(icon_get("list-remove"), _("Remove line"), self)
		self.tb_save.triggered.connect(self.callback_delete_item)
		toolbar.addAction(self.tb_save)

		self.tb_down= QAction(icon_get("go-down"), _("Move down"), self)
		self.tb_down.triggered.connect(self.on_move_down)
		toolbar.addAction(self.tb_down)

		self.tb_up= QAction(icon_get("go-up"), _("Move up"), self)
		self.tb_up.triggered.connect(self.on_move_up)
		toolbar.addAction(self.tb_up)

		self.tb_open = QAction(icon_get("document-open"), _("Open"), self)
		self.tb_open.triggered.connect(self.callback_open)
		toolbar.addAction(self.tb_open)
		
		self.vbox.addWidget(toolbar)

		self.tab = gpvdm_tab()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)
		self.create_model()

		self.tab.cellChanged.connect(self.tab_changed)

		self.vbox.addWidget(self.tab)

		self.select_param_window=select_param(self.tab)
		self.select_param_window.file_name_tab_pos=1
		self.select_param_window.token_tab_pos=2
		self.select_param_window.path_tab_pos=3
		self.select_param_window.update()

		self.select_param_window.set_save_function(self.save_combo)

		self.setLayout(self.vbox)
