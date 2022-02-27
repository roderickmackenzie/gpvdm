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

## @package scripts
#  The main script editor
#


import os
from icon_lib import icon_get
import zipfile
import glob
from tab import tab_class
from progress_class import progress_class
from help import my_help_class

#path
from cal_path import get_exe_command
from inp import inp_save_lines_to_file

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QHBoxLayout,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu, QComboBox, QMenuBar, QLabel
from PyQt5.QtGui import QIcon

#windows
from plot_widget import plot_widget
from error_dlg import error_dlg

from server import server_get

from global_objects import global_object_run
from global_objects import global_object_delete
from QWidgetSavePos import QWidgetSavePos

from script_ribbon import script_ribbon

from css import css_apply
from gui_util import yes_no_dlg
from script_editor import script_editor
from inp import inp_lsdir
from gui_util import dlg_get_text
from cal_path import get_scripts_path
from os import listdir
from inp import inp_save

from gpvdm_api import gpvdm_api

class scripts(QWidgetSavePos):

	def __init__(self,path="",api_callback=None):
		if path=="":
			self.path=get_scripts_path()
		else:
			self.path=path

		QWidgetSavePos.__init__(self,"scripts")

		self.setWindowIcon(icon_get("script"))

		self.setMinimumSize(1000, 600)
		self.setWindowTitle(_("Script editor")+" (https://www.gpvdm.com)")    

		self.ribbon=script_ribbon()


		self.setWindowIcon(icon_get("script"))

		self.main_vbox=QVBoxLayout()

		self.ribbon.run.start_sim.connect(self.callback_run)

		self.ribbon.tb_save.clicked.connect(self.callback_save)

		self.ribbon.tb_new.clicked.connect(self.callback_add_page)

		self.ribbon.tb_rename.clicked.connect(self.callback_rename_page)

		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)

		self.main_vbox.addWidget(self.ribbon)
		self.api_callback=api_callback


		self.notebook = QTabWidget()
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

		added=0
		for f in listdir(self.path):
			if f.endswith(".py"):
				file_name=os.path.join(self.path,f)
				a=script_editor()
				a.api_callback=self.api_callback
				a.load(file_name)
				a.status_changed.connect(self.callback_tab_changed)
				a.save_signal.connect(self.callback_save)
				self.notebook.addTab(a,f)
				added=added+1
		if added==0:
			file_name=os.path.join(self.path,"example.py")
			self.new_script(file_name)
			a=script_editor()
			a.api_callback=self.api_callback
			a.load(file_name)
			a.status_changed.connect(self.callback_tab_changed)
			a.save_signal.connect(self.callback_save)
			self.notebook.addTab(a,"example.py")

		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.main_vbox.addWidget(self.notebook)


		self.setLayout(self.main_vbox)

	def new_script(self,file_name):
		data=[]
		data.append("#!/usr/bin/env python3")
		data.append("# -*- coding: utf-8 -*-")
		data.append("import os")
		data.append("import sys")
		data.append("")
		data.append("from gpvdm_api import gpvdm_api")
		data.append("")
		data.append("def run():")
		data.append("	a=gpvdm_api(verbose=True)")
		data.append("	a.set_save_dir(device_data)")
		data.append("	a.edit(\"light.inp\",\"#light_model\",\"qe\")")
		data.append("	a.edit(\"jv0.inp\",\"#Vstop\",\"0.8\")")
		data.append("	a.run()")

		inp_save_lines_to_file(file_name,data)

	def callback_add_page(self):
		new_sim_name=dlg_get_text( "Add a new script:", "exampe.py","document-new.png")
		if new_sim_name.ret!=None:
			name=os.path.join(self.path,new_sim_name.ret)
			self.new_script(name)
			a=script_editor()
			a.load(name)
			a.status_changed.connect(self.callback_tab_changed)
			a.save_signal.connect(self.callback_save)
			self.notebook.addTab(a,os.path.basename(name))

	def callback_rename_page(self):
		tab = self.notebook.currentWidget()
		index=self.notebook.currentIndex()
		new_sim_name=dlg_get_text( _("Rename the script:"), os.path.basename(tab.file_name),"rename.png")

		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			if new_sim_name.endswith(".py")==False:
				new_sim_name=new_sim_name+".py"

			new_full_name=os.path.join(self.path,new_sim_name)
			os.rename(os.path.join(self.path,tab.file_name) , new_full_name)
			tab.file_name=new_full_name
			self.callback_tab_changed()

	def callback_tab_changed(self):
		tab = self.notebook.currentWidget()
		index=self.notebook.currentIndex() 

		short_name=os.path.basename(tab.file_name)

		if tab.not_saved==True:
			self.notebook.setTabText(index, "*"+short_name)
		else:
			self.notebook.setTabText(index, short_name)

	def closeEvent(self, event):
		global_object_delete("optics_force_redraw")
		self.hide()
		event.accept()

	def callback_save(self):
		tab = self.notebook.currentWidget()
		text=tab.toPlainText().split("\n")
		tab.not_saved=False
		inp_save(tab.file_name,text)
		self.callback_tab_changed()


	def callback_run(self):
		self.callback_save()
		tab = self.notebook.currentWidget()
		tab.run()


