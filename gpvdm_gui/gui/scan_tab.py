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

## @package scan_tab
#  A scan tab - sub widget.
#

import os
from plot import check_info_file
from plot_gen import plot_gen
from dat_file import dat_file
from cmp_class import cmp_class
from token_lib import tokens


from scan_plot import scan_gen_plot_data
from server import server_find_simulations_to_run

from gpvdm_open import gpvdm_open
from icon_lib import icon_get

from str2bool import str2bool

#scan_io
from scan_io import scan_clean_unconverged
from scan_io import scan_push_to_hpc
from scan_io import scan_import_from_hpc
from scan_io import scan_plot_fits
from scan_io import scan_gen_report
from scan_io import scan_io

#scan_tree
from scan_tree import tree_gen
from scan_tree import tree_load_flat_list
from scan_tree import tree_save_flat_list


#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QApplication,QDialog,QSizePolicy,QAction,QTableWidgetItem,QTabWidget,QMenuBar,QStatusBar, QMenu, QTableWidget, QAbstractItemView, QComboBox
from PyQt5.QtGui import QPainter,QIcon,QCursor,QClipboard

#window
from plot_dlg import plot_dlg_class
from scan_select import select_param
from error_dlg import error_dlg
from gui_util import yes_no_dlg
from gpvdm_tab import gpvdm_tab

import i18n
_ = i18n.language.gettext

from gpvdm_select import gpvdm_select
from help import help_window

from cal_path import get_sim_path

#from scan_ml import scan_ml

from progress_class import progress_class
from process_events import process_events

from scan_io import scan_archive

from win_lin import desktop_open

import datetime

from dir_decode import get_dir_type
from css import css_apply
from gpvdm_viewer import gpvdm_viewer
from util import wrap_text
from server import server_get

from scan_tab_ribbon import scan_tab_ribbon
from scan_program_line import scan_program_line

from multiplot import multiplot

class scan_vbox(QWidget):

	def callback_move_down(self):
		self.tab.move_down()

	def callback_move_up(self):
		self.tab.move_up()

	def callback_insert_command(self):
		a=self.tab.selectionModel().selectedRows()

		if len(a)>0:
			a=a[0].row()
			self.tab.set_value(a,3,"str(round(random.uniform(1.0, 9.9),2))+\"e-\"+str(randint(1, 9))")
			self.tab.set_value(a,4,"python_code")


	def add_line(self,data):
		help_window().help_set_help(["list-add.png",_("<big><b>The scan window</b></big><br> Now using the drop down menu in the prameter to change 'column', select the device parameter you wish to vary, an example may be dos0/Electron Mobility. Now enter the values you would like it to scan oveer in the  'Values', an example could be '1e-3 1e-4 1e-5 1e-6'.  And hit the double arrorw to run the simulation.")])

		self.insert_row(self.tab.rowCount(),data[0],data[1],data[2])
		
		self.rebuild_op_type_widgets()
		
		self.save_combo()


	def callback_add_item(self, widget, data=None):
		self.add_line([_("Select parameter"), "0.0 0.0", "scan",True])


	def callback_copy_item(self):
		data=self.get_selected()
		combine=';'.join(data)
		
		cb = QApplication.clipboard()
		cb.clear(mode=cb.Clipboard )
		cb.setText(combine, mode=cb.Clipboard)

	def callback_paste_item(self, widget, data=None):
		cb = QApplication.clipboard()
		text=cb.text()
		lines=text.rstrip().split(';')
		self.tab.add(lines)
		self.save_combo()

	def callback_show_list(self):
		self.select_param_window.set_save_function(self.save_combo)
		self.select_param_window.show()

	def callback_delete_item(self):
		self.tab.remove()
		self.rebuild_op_type_widgets()
		self.save_combo()

	def plot_results(self,data_file):
		data_file.key_units=self.get_units()
		plot_files, plot_labels, config_file = scan_gen_plot_data(data_file,self.scan_io.scan_dir)
		plot_gen(plot_files,plot_labels,config_file)

		self.last_plot_data=data_file
		return

	def get_units(self):
		token=""
		return ""
		for i in range(0,self.tab.rowCount()):
			if self.tab.get_value(i,2)=="scan":
				for ii in range(0,len(self.param_list)):
					if self.tab.get_value(i,0)==self.param_list[ii].human_label:
						token=self.param_list[ii].token
				break
		if token!="":
			found_token=self.tokens.find(token)
			if type(found_token)!=bool:
				return found_token.units

		return ""

	def gen_report(self):
		scan_gen_report(self.scan_io.scan_dir)


	def clean_scan_dir(self):
		self.scan_io.clean_dir()

	def scan_clean_unconverged(self):
		scan_clean_unconverged(self,self.scan_io.scan_dir)

	def scan_clean_simulation_output(self):
		s=scan_io()
		s.parent_window=self
		s.set_path(self.scan_io.scan_dir)
		s.clean_dir()

	def import_from_hpc(self):
		scan_import_from_hpc(self.scan_io.scan_dir)

	def plot_fits(self):
		scan_plot_fits(self.scan_io.scan_dir)

	def push_to_hpc(self):
		scan_push_to_hpc(self.scan_io.scan_dir,False)

	def push_unconverged_to_hpc(self):
		scan_push_to_hpc(self.scan_io.scan_dir,True)

	def scan_archive(self):
		scan_archive(self.scan_io.scan_dir)

	def build_scan(self):
		self.scan_io.set_base_dir(get_sim_path())
		self.scan_io.build_scan()


	def callback_plot_results(self, widget, data=None):
		self.plot_results(self.last_plot_data)

	def callback_last_menu_click(self, widget, data):
		#print("here one!")
		self.plot_results(data)

	def callback_reopen_xy_window(self, widget, data=None):
		if len(self.plotted_graphs)>0:
			pos=len(self.plotted_graphs)-1
			plot_data=dat_file()
			plot_data.file0=self.plotted_graphs[pos].file0
			plot_xy_window=plot_dlg_class(plot_data)
			plot_xy_window.run()
			plot_now=plot_xy_window.ret

			if plot_now==True:
				self.plot_results(plot_data)
				self.plotted_graphs.refresh()

	def callback_gen_plot_command(self):
		dialog=gpvdm_open(self.scan_io.scan_dir,act_as_browser=False)
		ret=dialog.exec_()

		if ret==QDialog.Accepted:
			full_file_name=dialog.get_filename()

			dir_type=get_dir_type(full_file_name)
			if dir_type!=None:
				if dir_type.type=="snapshots":
					from cmp_class import cmp_class
					self.snapshot_window=cmp_class(full_file_name)
					self.snapshot_window.show()
					return

			file_name=os.path.basename(full_file_name)

			plot_data=dat_file()
			plot_data.path=self.scan_io.scan_dir
			plot_data.example_file0=full_file_name
			plot_data.example_file1=full_file_name

			plot_now=False
			if check_info_file(file_name)==True:
				plot_data.file0=file_name
				plot_xy_window=plot_dlg_class(plot_data)
				plot_xy_window.run()
				plot_now=plot_xy_window.ret
			else:
				plot_data.file0=file_name
				plot_data.tag0=""
				plot_data.file1=""
				plot_data.tag1=""
				plot_now=True

			if plot_now==True:
				self.plot_results(plot_data)

				#self.plotted_graphs.refresh()


	def save_combo(self):
		self.scan_io.make_dir()
		self.scan_io.program_list=[]

		for i in range(0,self.tab.rowCount()):
			item=scan_program_line()
			item.human_name=self.tab.get_value(i,0)
			item.values=self.tab.get_value(i,1)
			item.opp=self.tab.get_value(i,2)
			self.scan_io.program_list.append(item)

		self.scan_io.save()

	def cell_changed(self, y,x):
		print(x,y)
		self.rebuild_op_type_widgets()
		self.save_combo()

	def callback_combo_changed(self):
		combobox = self.sender()
		ix = self.tab.indexAt(combobox.pos())
		y=ix.row()
		value=self.tab.get_value(y,2)
		if value == "constant":
			self.tab.set_value(y,1,"0.0")

		elif value == "scan":
			self.tab.set_value(y,1,"1e-5 1e-6 1e-7 1e-8")

		elif value == "python_code":
			self.tab.set_value(y,1,"python code")

		self.tab.set_value(y,1,"duplicate")

		self.save_combo()

	def insert_row(self,i,v0,v1,v2):
		self.tab.blockSignals(True)
		self.tab.insertRow(i)

		self.item = gpvdm_select()
		self.item.setText(v0)
		self.item.button.clicked.connect(self.callback_show_list)

		self.tab.setCellWidget(i,0,self.item)

		item = QTableWidgetItem(v1)
		self.tab.setItem(i,1,item)

		item = QTableWidgetItem(v2)
		self.tab.setItem(i,2,item)
		self.tab.blockSignals(False)

	def load(self):
		self.tab.blockSignals(True)
		self.tab.clear()
		self.tab.setRowCount(0)
		self.tab.setHorizontalHeaderLabels([ _("Parameter to change"), _("Values"), _("Opperation")])
		self.tab.blockSignals(False)

		self.status_bar.showMessage(self.scan_io.scan_dir+"("+os.path.basename(self.scan_io.config_file)+")")
		
		self.scan_io.load(self.scan_io.config_file)
		for i in range(0,len(self.scan_io.program_list)):
			l=self.scan_io.program_list[i]
			self.insert_row(i,l.human_name,l.values,l.opp)

		self.rebuild_op_type_widgets()


	def contextMenuEvent(self, event):
		self.popMenu.popup(QCursor.pos())

	def rebuild_op_type_widgets(self):
		self.tab.blockSignals(True)
		items=[]
		items.append("scan")
		items.append("constant")
		items.append("python_code")
		items.append("none")

		for i in range(0,self.tab.rowCount()):
			items.append(str(self.tab.get_value(i,0)))

		for i in range(0,self.tab.rowCount()):
			save_value=self.tab.get_value(i,2)
			
			combobox = QComboBox()
			for a in items:
				combobox.addItem(a)

			self.tab.setCellWidget(i,2, combobox)
			
			self.tab.set_value(i,2,save_value)
			combobox.currentIndexChanged.connect(self.callback_combo_changed)
		self.tab.blockSignals(False)


	def callback_run_simulation(self):
		if self.tab.rowCount() == 0:
			error_dlg(self,_("You have not selected any parameters to scan through.  Use the add button."))
			return

		self.save_combo()
		self.scan_io.myserver=server_get()
		self.scan_io.myserver.callback=self.callback_build_plots
		self.scan_io.set_base_dir(get_sim_path())
		self.scan_io.run()

	def callback_build_plots(self):
		a=multiplot(gnuplot=True)
		a.find_files(self.scan_io.scan_dir)
		a.save(gnuplot=True,multi_plot=True)

	def callback_notes(self):
		notes_path=os.path.join(self.scan_io.scan_dir,"notes.txt")
		if os.path.isfile(notes_path)==False:
			out = open(notes_path, 'w')
			out.write("Notes on the simulation: "+os.path.basename(self.scan_io.human_name)+"\n")
			out.write("Date: "+datetime.datetime.today().strftime('%Y-%m-%d')+"\n")
			out.write("Generated by: "+os.getusername()+"\n")
			out.write("\n")
			out.close()
		else:
			out = open(notes_path, 'a')
			out.write("\n"+datetime.datetime.today().strftime('%Y-%m-%d')+":\n")
			out.close()


		desktop_open(notes_path)

	def __init__(self,scan_file):
		QWidget.__init__(self)
		self.notebook=QTabWidget()
		self.setWindowTitle(_("Parameter scan editor - (www.gpvdm.com)"))
		self.setWindowIcon(icon_get("scan"))

		self.main_vbox = QVBoxLayout()

		self.scan_tab_vbox = QVBoxLayout()

		self.tokens=tokens()
		self.myserver=server_get()
		self.status_bar=QStatusBar()
		#self.tab_label=tab_label

		self.scan_io=scan_io()
		self.scan_io.parent_window=self
		self.scan_io.load(scan_file)
		self.scan_io.make_dir()

		toolbar=QToolBar()
		

		self.scan_tab_vbox.addWidget(toolbar)

		self.tab = gpvdm_tab(toolbar=toolbar)

		self.tb_command = QAction(icon_get("utilities-terminal"), _("Insert python command"), self)
		self.tb_command.triggered.connect(self.callback_insert_command)
		toolbar.addAction(self.tb_command)

		self.tab.tb_add.triggered.connect(self.callback_add_item)

		self.tab.user_remove_rows.connect(self.callback_delete_item)

		self.tab.tb_down.triggered.connect(self.callback_move_down)

		self.tab.tb_up.triggered.connect(self.callback_move_up)

		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

	
		self.tab.verticalHeader().setVisible(False)


		
		self.tab.setColumnCount(3)

		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setColumnWidth(0, 300)
		self.tab.setColumnWidth(1, 300)
		self.tab.setColumnWidth(2, 300)
		self.load()

		self.tab.cellChanged.connect(self.cell_changed)
		
		self.scan_tab_vbox.addWidget(self.tab)

		self.notebook.setMinimumSize(1000,500)
				
		self.program_widget=QWidget()
		self.program_widget.setLayout(self.scan_tab_vbox)
		self.notebook.addTab(self.program_widget,"Commands")

		self.viewer=gpvdm_viewer(self.scan_io.scan_dir)
		self.viewer.show_back_arrow=True
		self.notebook.addTab(self.viewer,"Output")

		self.ribbon=scan_tab_ribbon()
		self.ribbon.tb_simulate.triggered.connect(self.callback_run_simulation)
		self.ribbon.tb_clean.triggered.connect(self.clean_scan_dir)
		self.ribbon.tb_plot.triggered.connect(self.callback_gen_plot_command)
		self.ribbon.tb_notes.triggered.connect(self.callback_notes)

		self.main_vbox.addWidget(self.ribbon)
		self.main_vbox.addWidget(self.notebook)


		self.main_vbox.addWidget(self.status_bar)		
		self.setLayout(self.main_vbox)
		self.select_param_window=select_param(self.tab)
		
