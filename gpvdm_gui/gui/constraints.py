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

## @package constraints
#  Windows to set the fitting constraints.
#

import os

from scan_select import select_param
from token_lib import tokens

from scan_io import scan_push_to_hpc
from scan_io import scan_import_from_hpc
from cal_path import get_exe_command
from icon_lib import icon_get

from gpvdm_select import gpvdm_select


from str2bool import str2bool

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QAbstractItemView, QMenuBar, QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon


from inp import inp_save_lines_to_file
from inp import inp_load_file

from scan_human_labels import get_scan_human_labels

from cal_path import get_sim_path

from gpvdm_tab import gpvdm_tab

class constraints(QWidget):

	def insert_row_mm(self,i,file_name,token,path,function,max_value,min_value,error):
		self.tab_mm.blockSignals(True)

		item = QTableWidgetItem(file_name)
		self.tab_mm.setItem(i,0,item)

		item = QTableWidgetItem(token)
		self.tab_mm.setItem(i,1,item)

		self.item = gpvdm_select()
		self.item.setText(path)
		self.item.button.clicked.connect(self.callback_show_list_src)
		self.tab_mm.setCellWidget(i,2,self.item)


		item = QTableWidgetItem(function)
		self.tab_mm.setItem(i,3,item)

		item = QTableWidgetItem(max_value)
		self.tab_mm.setItem(i,4,item)

		item = QTableWidgetItem(min_value)
		self.tab_mm.setItem(i,5,item)

		item = QTableWidgetItem(error)
		self.tab_mm.setItem(i,6,item)
		
		self.tab_mm.blockSignals(False)

	def insert_row_math(self,i,file_a,token_a,path_a,file_b,token_b,path_b,equation):
		self.tab_math.blockSignals(True)
		self.tab_math.insertRow(i)

		item = QTableWidgetItem(file_a)
		self.tab_math.setItem(i,0,item)

		item = QTableWidgetItem(token_a)
		self.tab_math.setItem(i,1,item)

		self.item = gpvdm_select()
		self.item.setText(path_a)
		self.item.button.clicked.connect(self.callback_show_list_equation_a)
		self.tab_math.setCellWidget(i,2,self.item)

		item = QTableWidgetItem(file_b)
		self.tab_math.setItem(i,3,item)

		item = QTableWidgetItem(token_b)
		self.tab_math.setItem(i,4,item)

		self.item = gpvdm_select()
		self.item.setText(path_b)
		self.item.button.clicked.connect(self.callback_show_list_equation_b)
		self.tab_math.setCellWidget(i,5,self.item)

		item = QTableWidgetItem(equation)
		self.tab_math.setItem(i,6,item)

		self.tab_math.blockSignals(False)

	def callback_show_list_equation_a(self):
		self.select_param_window_math_a.update()
		self.select_param_window_math_a.show()

	def callback_show_list_equation_b(self):
		self.select_param_window_math_b.update()
		self.select_param_window_math_b.show()

	def callback_show_list_src(self):
		self.select_param_window_mm.update()
		self.select_param_window_mm.show()

	#def callback_show_list_dest(self):
	#	self.select_param_window_dest.update()
	#	self.select_param_window_dest.show()
		
	def callback_add_item_mm(self):
		pos=self.tab_mm.insert_row()
		self.insert_row_mm(pos,_("File"),_("Token"),_("Path"),_("Function"),_("Max"),_("Min"),_("Error"))
		self.save_combo()

	def callback_delete_item_mm(self):
		self.tab_mm.remove()
		self.save_combo()

	def callback_add_item_math(self):
		self.insert_row_math(self.tab_math.rowCount(),_("File (a)"),_("Token (a)"),_("Path (a)"),_("File (b)"),_("Token (b)"),_("Path (b)"),_("Equation"))
		self.save_combo()

	def callback_delete_item_math(self):
		self.tab_math.remove()
		self.save_combo()

	def save_combo(self):
		lines=[]
		for i in range(0,self.tab_mm.rowCount()):
			line="mm "+str(self.tab_mm.get_value(i, 0))+" "+str(self.tab_mm.get_value(i, 1))+" "+str(self.tab_mm.get_value(i, 3))+" "+str(self.tab_math.get_value(i, 4))+" "+str(self.tab_mm.get_value(i, 5))+" "+str(self.tab_mm.get_value(i, 6))
			lines.append(line)
			
		for i in range(0,self.tab_math.rowCount()):
			line="math "+str(self.tab_math.get_value(i, 0))+" "+str(self.tab_math.get_value(i, 1))+" "+str(self.tab_math.get_value(i, 3))+" "+str(self.tab_math.get_value(i, 4))+" "+str(self.tab_math.get_value(i, 6))
			lines.append(line)

		lines.append("#end")
		print("save as",self.file_name)
		inp_save_lines_to_file(self.file_name,lines)


	def tab_changed(self):
		print("here")
		self.save_combo()
		

	def create_model_mm(self):
		self.tab_mm.clear()
		self.tab_mm.setColumnCount(7)
		self.tab_mm.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab_mm.setHorizontalHeaderLabels([ _("File"), _("Token"),_("Path"),_("Function"), _("Max"), _("Min"),_("Error")])
		self.tab_mm.setColumnWidth(2, 300)
		#self.tab_mm.setColumnWidth(5, 200)


		lines=[]
		pos=0
		lines=inp_load_file(self.file_name)
		if lines!=False:
			mylen=len(lines)
			while(1):

				if lines[pos]=="#end":
					break
				line=lines[pos].split()
				print(line)
				if line[0]=="mm":
					path=self.get_scan_human_labels.get_human_label(line[1],line[2])
					self.tab_mm.insertRow(self.tab_mm.rowCount())
					self.insert_row_mm(self.tab_mm.rowCount()-1,line[1],line[2],path,line[3],line[4],line[5],line[6])

				pos=pos+1

	def create_model_math(self):
		self.tab_math.clear()
		self.tab_math.setColumnCount(7)
		self.tab_math.setSelectionBehavior(QAbstractItemView.SelectRows)
		#math fit_target2.inp #fxdomain_Ji ./sim/39.811Hz/sim_info.dat #fxdomain_Ji abs(20*(a-b))
		self.tab_math.setHorizontalHeaderLabels([ _("File (a)"), _("Token (a)"),_("Path (a)"),_("File (b)"),_("Token (b)"),_("Path (b)"), _("Equation")])

		lines=[]
		pos=0
		lines=inp_load_file(self.file_name)
		if lines!=False:
			mylen=len(lines)
			while(1):

				if lines[pos]=="#end":
					break
				line=lines[pos].split()
				if line[0]=="math":
					print(line)
					path_a=self.get_scan_human_labels.get_human_label(line[1],line[2])
					path_b=self.get_scan_human_labels.get_human_label(line[3],line[4])
					self.insert_row_math(self.tab_math.rowCount(),line[1],line[2],path_a,line[3],line[4],path_b,line[5])

				pos=pos+1

	def __init__(self):
		QWidget.__init__(self)
		self.file_name=os.path.join(get_sim_path(),"fit_constraints.inp")
		self.setWindowTitle(_("Fit constraints window")+" - https://www.gpvdm.com")   
		self.setWindowIcon(icon_get("constraints"))
		self.setFixedSize(900, 700)

		self.vbox=QVBoxLayout()

		########################mm##########################
		toolbar_mm=QToolBar()
		toolbar_mm.setIconSize(QSize(32, 32))

		self.tb_save = QAction(icon_get("list-add"), _("Add"), self)
		self.tb_save.triggered.connect(self.callback_add_item_mm)
		toolbar_mm.addAction(self.tb_save)

		self.tb_save = QAction(icon_get("list-remove"), _("Minus"), self)
		self.tb_save.triggered.connect(self.callback_delete_item_mm)
		toolbar_mm.addAction(self.tb_save)

		self.tb_down= QAction(icon_get("go-down"), _("Move down"), self)
		self.tb_down.triggered.connect(self.on_move_down)
		toolbar_mm.addAction(self.tb_down)

		self.tb_up= QAction(icon_get("go-up"), _("Move up"), self)
		self.tb_up.triggered.connect(self.on_move_up)
		toolbar_mm.addAction(self.tb_up)

		self.vbox.addWidget(toolbar_mm)

		self.tab_mm = gpvdm_tab()
		self.tab_mm.resizeColumnsToContents()

		self.tab_mm.verticalHeader().setVisible(False)
		self.create_model_mm()

		self.tab_mm.cellChanged.connect(self.tab_changed)

		self.select_param_window_mm=select_param(self.tab_mm)
		self.select_param_window_mm.set_save_function(self.save_combo)
		self.select_param_window_mm.file_name_tab_pos=0
		self.select_param_window_mm.token_tab_pos=1
		self.select_param_window_mm.path_tab_pos=2

		
		self.vbox.addWidget(self.tab_mm)

		#####################math##########################
		toolbar_math=QToolBar()
		toolbar_math.setIconSize(QSize(32, 32))

		self.tb_save = QAction(icon_get("list-add"), _("Add"), self)
		self.tb_save.triggered.connect(self.callback_add_item_math)
		toolbar_math.addAction(self.tb_save)

		self.tb_save = QAction(icon_get("list-remove"), _("Minus"), self)
		self.tb_save.triggered.connect(self.callback_delete_item_math)
		toolbar_math.addAction(self.tb_save)

		self.vbox.addWidget(toolbar_math)
		self.tab_math = gpvdm_tab()
		self.tab_math.resizeColumnsToContents()

		self.tab_math.verticalHeader().setVisible(False)

		self.create_model_math()

		self.tab_math.cellChanged.connect(self.tab_changed)

		self.select_param_window_math_a=select_param(self.tab_math)
		self.select_param_window_math_a.set_save_function(self.save_combo)
		self.select_param_window_math_a.file_name_tab_pos=0
		self.select_param_window_math_a.token_tab_pos=1
		self.select_param_window_math_a.path_tab_pos=2

		self.select_param_window_math_b=select_param(self.tab_math)
		self.select_param_window_math_b.set_save_function(self.save_combo)
		self.select_param_window_math_b.file_name_tab_pos=4
		self.select_param_window_math_b.token_tab_pos=5
		self.select_param_window_math_b.path_tab_pos=6
		self.vbox.addWidget(self.tab_math)
		
		self.setLayout(self.vbox)

		self.get_scan_human_labels=get_scan_human_labels()

	def closeEvent(self, event):
		self.hide()

	def on_move_down(self):
		self.tab_mm.move_down()
		self.save_combo()
		
	def on_move_up(self):
		self.tab_mm.move_up()
		self.save_combo()
