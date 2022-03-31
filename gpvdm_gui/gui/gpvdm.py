#!/usr/bin/env python3
# -*- coding: utf-8 -*-
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

## @package gpvdm
#  The main gpvdm gui code
#

import os
import sys

#paths
sys.path.append('./gui/')
sys.path.append('./gui/base/')
sys.path.append('./base/')
sys.path.append('/usr/lib/gpvdm/')
sys.path.append('/usr/lib64/gpvdm/')
sys.path.append('/usr/share/gpvdm/gui/')	#debian
sys.path.append('/usr/share/sip/PyQt5/')


from gui_enable import gui_test
gui_test()

from win_lin import running_on_linux
from cal_path import get_image_file_path
from cal_path import calculate_paths
from cal_path import calculate_paths_init
from cal_path import get_share_path
from cal_path import set_sim_path

calculate_paths_init()
calculate_paths()

from notice import notice
from inp import inp

print(notice())
print("loading.... please wait...")

from cal_path import get_lang_path

import i18n
_ = i18n.language.gettext


#undo
from undo import undo_list_class

from command_args import command_args

command_args(len(sys.argv),sys.argv)

from plot_gen import plot_gen
from help import help_window
from help import help_init
from help import language_advert
from QWidgetSavePos import resize_window_to_be_sane

from gpvdm_notebook import gpvdm_notebook

#server
from server import server
from server import server_init
from server import server_get

from splash import splash_window
from process_events import process_events
from error_han import error_han

#qt
from PyQt5.QtWidgets import QMainWindow,QApplication
from PyQt5.QtWidgets import QTextEdit, QAction
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtCore import QTimer


from cal_path import get_materials_path
from clone_materials import clone_materials
from cal_path import get_spectra_path
from clone import clone_spectras
from used_files import used_files_add

from epitaxy import get_epi
from cal_path import get_scripts_path
from cal_path import get_base_scripts_path
from icon_lib import icon_init_db
import shutil
from check_lib_in_bash_rc import check_lib_in_bash_rc
from msg_dlg import msg_dlg
from lock_gui import lock_gui
from lock import get_lock
def do_import():
	global webbrowser

	global yes_no_dlg
	global isfiletype
	global dbus
	global DBusQtMainLoop
	global win_pipe

	global QTabWidget
	global ribbon

	global error_dlg
	global to_native_path
	global get_sim_path
	global global_object_run
	global check_sim_exists

	#qt
	from PyQt5.QtWidgets import QTextEdit, QAction
	from PyQt5.QtGui import QIcon
	from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
	from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
	
	#python modules
	import webbrowser

	from gui_util import yes_no_dlg
	from util import isfiletype

	#gobject.threads_init()

	from PyQt5.QtWidgets import QTabWidget
	from ribbon import ribbon

	from error_dlg import error_dlg

	from cal_path import to_native_path
	from cal_path import get_sim_path
	from global_objects import global_object_run
	from check_sim_exists import check_sim_exists

from cal_path import get_base_material_path

from cal_path import get_base_emission_path
from cal_path import get_emission_path

from cal_path import get_base_shape_path
from cal_path import gpvdm_paths

from gpvdm_json import gpvdm_data
from const_ver import const_ver
from gpvdm_local import gpvdm_local

if running_on_linux()==True:
	import dbus
	from dbus.mainloop.glib import DBusGMainLoop
	DBusGMainLoop(set_as_default=True)
	#from dbus.mainloop.pyqt5 import DBusQtMainLoop
	#DBusQtMainLoop(set_as_default=True)

	if os.geteuid() == 0:
		exit(_("Don't run me as root!!"))
else:
	from windows_pipe import win_pipe


class gpvdm_main_window(QMainWindow):

	plot_after_run=False
	plot_after_run_file=""

	def adbus(self,bus, message):
		data=message.get_member()
		if data!=None:
			self.my_server.callback_dbus(data)
		
	def win_dbus(self, data):
		self.my_server.callback_dbus(data)


	def gui_sim_start(self):
		self.notebook_active_page=self.notebook.get_current_page()#self.tabText(i)#self.notebook.currentIndex()
		self.notebook.goto_page(_("Terminal"))

	def gui_sim_stop(self):
		message=""
		if self.notebook_active_page!=None:
			self.notebook.goto_page(self.notebook_active_page)
		global_object_run("display_recalculate")
	
	def callback_plot_after_run_toggle(self, widget, data):
		self.plot_after_run=data.get_active()
		self.config.set_value("#plot_after_simulation",data.get_active())


	def callback_set_plot_auto_close(self, widget, data):
		set_plot_auto_close(data.get_active())
		self.config.set_value("#one_plot_window",data.get_active())


	#def callback_run_scan(self, widget, data=None):
	#	if self.scan_window!=None:
	#		self.scan_window.callback_run_simulation()

	def callback_simulate(self):

		self.my_server.clear_cache()
		self.my_server.clear_jobs()
		self.my_server.add_job(get_sim_path(),"")
		self.my_server.start()


	def close_now(self):
		QApplication.quit()
		

	def closeEvent(self, event):
		print("closing")
		self.close_now()
		event.accept()

	def callback_last_menu_click(self, widget, data):
		#self.plot_open.set_sensitive(True)
		file_to_load=os.path.join(get_sim_path(),data.file0)
		plot_gen([file_to_load],[],"auto")
		self.plot_after_run_file=file_to_load



	def callback_new(self):
		from new_simulation import new_simulation
		help_window().help_set_help(["p3ht_pcbm.png",_("<big><b>New simulation!</b></big><br> Now selected the type of device you would like to simulate.")])

		dialog=new_simulation()
		dialog.exec_()
		ret=dialog.ret_path

		if ret!=None:
			self.change_dir_and_refresh_interface(dialog.ret_path)
			if gpvdm_data().sim.first_sim_message!="":
				msgBox = msg_dlg(title="www.gpvdm.com")
				msgBox.setText(gpvdm_data().sim.first_sim_message.replace("%DIR",dialog.ret_path))
				reply = msgBox.exec_()

	def update_interface(self):
		if self.notebook.is_loaded()==True:
			self.check_sim_exists.set_dir(get_sim_path())

			help_window().help_set_help(["media-playback-start",_("<big><b>Now run the simulation</b></big><br> Click on the play icon to start a simulation.")])

			if gpvdm_local().gui_config.enable_betafeatures==True:
				self.ribbon.simulations.qe.setVisible(True)
		else:
			self.check_sim_exists.set_dir("")
			help_window().help_set_help(["icon.png",_("<big><b>Hi!</b></big><br> I'm the on-line help system :).  If you have any questions or find any bugs please send them to <a href=\"mailto:"+get_lock().my_email+"\">"+get_lock().my_email+"</a>."),"document-new.png",_("Click on the new icon to make a new simulation directory.")])
			language_advert()

			if gpvdm_local().gui_config.enable_betafeatures==True:
				self.ribbon.simulations.qe.setVisible(True)

	def disable_interface(self):
		self.ribbon.file.setEnabled(False,do_all=True)

		self.ribbon.simulations.setEnabled(False)
		self.ribbon.database.setEnabled(False)
		#self.ribbon.device.setEnabled(False)
		self.ribbon.goto_page(_("File"))

		self.ribbon.electrical.setEnabled(False)
		self.ribbon.thermal.setEnabled(False)
		self.notebook.setEnabled(False)


	def enable_disable_buttons(self):
		self.ribbon.file.setEnabled(True)
		self.notebook.setEnabled(True)

		if self.notebook.is_loaded()==True:
			self.ribbon.file.setEnabled_other(True)
			self.ribbon.simulations.setEnabled(True)
			self.ribbon.database.setEnabled(True)
			#self.save_sim.setEnabled(True)
			#self.ribbon.device.setEnabled(True)

			#self.menu_import_lib.setEnabled(True)
			self.ribbon.electrical.setEnabled(True)
			self.ribbon.thermal.setEnabled(True)
			self.ribbon.goto_page(_("Home"))

		else:
			self.ribbon.file.setEnabled_other(False)

			self.ribbon.simulations.setEnabled(False)
			self.ribbon.database.setEnabled(False)
			#self.ribbon.device.setEnabled(False)
			self.ribbon.goto_page(_("File"))
			self.ribbon.thermal.setEnabled(False)
			self.ribbon.electrical.setEnabled(False)


	def change_dir_and_refresh_interface(self,new_dir):

		used_files_add(os.path.join(new_dir,"sim.gpvdm"))
		a=gpvdm_data()
		if inp().isfile(os.path.join(new_dir,"sim.json"))==True:
			a.load(os.path.join(new_dir,"sim.json"))
		elif inp().isfile(os.path.join(new_dir,"json.inp"))==True:
			a.load(os.path.join(new_dir,"json.inp"))
			a.f.file_name="sim.json"


		a.sim.version=const_ver()
		a.save()
		#get_watch().reset()
		self.splash.inc_value()

		self.splash.inc_value()

		set_sim_path(new_dir)
		self.splash.inc_value()

		#calculate_paths()
		self.splash.inc_value()

		epi=get_epi()

		self.statusBar().showMessage(get_sim_path())
		self.splash.inc_value()

		self.notebook.load()



		self.update_interface()
		self.enable_disable_buttons()

		self.splash.inc_value()

		if self.notebook.terminal!=None:
			self.my_server.set_terminal(self.notebook.terminal)

		if self.notebook.update_display_function!=None:
			self.my_server.set_display_function(self.notebook.update_display_function)

		self.ribbon.update()
		self.splash.inc_value()

		if self.notebook.is_loaded()==True:
			self.l.run()
			self.notebook.tab_main.three_d.update()

		self.ribbon.electrical.tb_solvers.changed.connect(self.notebook.update_circuit_window)
		try:
			self.ribbon.electrical.tb_solvers.changed.connect(self.notebook.tab_main.ribbon.callback_circuit_diagram)
		except:
			pass


	def load_sim(self,filename):
		new_path=os.path.dirname(filename)
		if filename.startswith(get_share_path())==True:
			error_dlg(self,_("You should not try to open simulations in the root gpvdm directory."))
			return

		self.change_dir_and_refresh_interface(new_path)

	def callback_open(self, widget, data=None):
		dialog = QFileDialog(self)
		dialog.setWindowTitle(_("Open an existing gpvdm simulation"))
		dialog.setNameFilter('Simulations - gpvdm (*.gpvdm *.opvdm)')
		dialog.setFileMode(QFileDialog.ExistingFile)
		if dialog.exec_() == QDialog.Accepted:
			filename = dialog.selectedFiles()[0]
			filename=to_native_path(filename)
			self.load_sim(filename)

	def callback_export(self, widget, data=None):
		types=[]
		dialog = QFileDialog(self)
		dialog.setDirectory(get_sim_path())
		dialog.selectFile(os.path.basename(get_sim_path()))
		dialog.setWindowTitle(_("Export the simulation"))
		dialog.setAcceptMode(QFileDialog.AcceptSave)
		types.append(_("Zip file")+" (*.zip)")

		dialog.setNameFilters(types)
		dialog.setFileMode(QFileDialog.ExistingFile)
		dialog.setAcceptMode(QFileDialog.AcceptSave)

		if dialog.exec_() == QDialog.Accepted:
			file_name = dialog.selectedFiles()[0]
			from util_zip import archive_add_dir
			if file_name.endswith(".zip")==False:
				file_name=file_name+".zip"
			archive_add_dir(file_name,get_sim_path(),get_sim_path())

	def callback_on_line_help(self):
		webbrowser.open("https://www.gpvdm.com")


	def callback_undo(self, widget, data=None):
		l=self.undo_list.get_list()
		if len(l)>0:
			value=l[len(l)-1][2]
			w_type=l[len(l)-1][3]

			if type(w_type)==QLineEdit:
				self.undo_list.disable()
				w_type.setText(value)
				self.undo_list.enable()

			l.pop()

	def sim_gone(self):
		error_dlg(self,_("The simulation directory has been deleted."))
		self.update_interface()
		self.enable_disable_buttons()

	def __init__(self):
		super(gpvdm_main_window,self).__init__()
		icon_init_db()
		self.splash=splash_window()

		self.splash.inc_value()
		process_events()
		process_events()

		do_import()

		if os.path.isdir(os.path.dirname(sys.argv[0]))==False:
			error_dlg(self,_("I can't run from inside a zip file!"))
			sys.exit()

		self.splash.inc_value()
		self.splash.inc_value()

		server_init()
		self.splash.inc_value()

		self.check_sim_exists=check_sim_exists()
		self.splash.inc_value()

		self.check_sim_exists.start_thread()
		self.splash.inc_value()

		self.check_sim_exists.sim_gone.connect(self.sim_gone)
		self.splash.inc_value()

		self.my_server=server_get()
		self.my_server.init(get_sim_path())
		self.splash.inc_value()


		self.undo_list=undo_list_class()
		self.splash.inc_value()

		self.ribbon=ribbon()
		self.splash.inc_value()

		self.notebook_active_page=None
		self.setAcceptDrops(True)
		#self.setGeometry(200, 100, 1300, 600)
		self.setWindowTitle("General-purpose Photovoltaic Device Model (https://www.gpvdm.com)")

		self.l=lock_gui()

		self.my_server.sim_started.connect(self.gui_sim_start)
		self.splash.inc_value()

		self.my_server.sim_finished.connect(self.gui_sim_stop)
		self.splash.inc_value()

		help_init()
		self.splash.inc_value()

		#help_window().help_set_help(["star.png",_("<big><b>Update available!</b></big><br>")])


		#self.show()

		if running_on_linux()==True:
			self.bus = dbus.SessionBus()
			self.bus.add_match_string_non_blocking("type='signal',interface='org.my.gpvdm'")
			self.bus.add_message_filter(self.adbus)
		else:
			self.win_pipe=win_pipe()
			self.win_pipe.new_data.connect(self.win_dbus)
			self.win_pipe.start()

		self.notebook=gpvdm_notebook()
		vbox=QVBoxLayout()
		self.splash.inc_value()

		vbox.addWidget(self.ribbon)
		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		vbox.addWidget(self.notebook)
		wvbox=QWidget()
		self.splash.inc_value()

		wvbox.setLayout(vbox)
		self.setCentralWidget(wvbox)

		self.splash.inc_value()

		self.statusBar()

		self.setWindowIcon(QIcon(os.path.join(get_image_file_path(),"image.jpg")))		
		self.splash.inc_value()

		self.show_tabs = True
		self.show_border = True

		self.ribbon.file.home_new.clicked.connect(self.callback_new)
		self.ribbon.file.home_open.clicked.connect(self.callback_open)
		self.ribbon.file.home_export.clicked.connect(self.callback_export)

		self.ribbon.file.used_files_click.connect(self.load_sim)
		#self.ribbon.home.undo.triggered.connect(self.callback_undo)
		self.ribbon.file.run.start_sim.connect(self.callback_simulate)
		self.splash.inc_value()

		#self.ribbon.home.stop.setEnabled(False)

		self.ribbon.file.scan.setEnabled(False)
		self.ribbon.thermal.setEnabled(False)

		
		#self.ribbon.home.help.triggered.connect(self.callback_on_line_help)

		resize_window_to_be_sane(self,0.7,0.75)

		self.change_dir_and_refresh_interface(get_sim_path())
		self.splash.inc_value()


		#self.ribbon.home.sun.changed.connect(self.notebook.update)
		self.ribbon.setAutoFillBackground(True)
		self.splash.inc_value()
		self.show()

		help_window().show()


		self.enable_disable_buttons()

		if gpvdm_paths.get_use_gpvdm_local()==True:
			coppied=False
			if os.path.isdir(get_materials_path())==False:
				clone_materials(get_materials_path(), get_base_material_path(),"material")
				coppied=True
			if os.path.isdir(get_emission_path())==False:
				clone_materials(get_emission_path(), get_base_emission_path(),"emission")
				coppied=True

			if os.path.isdir(gpvdm_paths.get_shape_path())==False:
				clone_materials(gpvdm_paths.get_shape_path(), get_base_shape_path(),"shape")
				coppied=True

			if os.path.isdir(get_scripts_path())==False:
				shutil.copytree(get_base_scripts_path(), get_scripts_path() ,symlinks=True)
				coppied=True

			if os.path.isdir(gpvdm_paths.get_filters_path())==False:
				clone_materials(gpvdm_paths.get_filters_path(), gpvdm_paths.get_base_filters_path(),"filter")
				coppied=True

			if os.path.isdir(get_spectra_path())==False:
				clone_spectras(get_spectra_path())
				coppied=True

			#coppied=True
			#if coppied==True:
			#	from video import video
			#	self.v=video()
			#	self.v.show()

		check_lib_in_bash_rc()

		#self.ribbon.optical.fx_box.cb.currentIndexChanged.connect(self.notebook.tab_main.three_d.fx_box_changed)

		#self.notebook.tab_main.three_d.fx_box=self.ribbon.optical.fx_box

		self.timer=QTimer()		
		self.timer.timeout.connect(gpvdm_data().check_reload)
		self.timer.start(1000)

		#from shape_editor import shape_editor
		#self.a=shape_editor("/home/rod/gpvdm_local/shape/morphology/3")
		#self.a.show()

	def dragEnterEvent(self, event):
		if event.mimeData().hasUrls:
			event.accept()
		else:
			event.ignore()

	def dropEvent(self, event):
		if event.mimeData().hasUrls:
			event.setDropAction(Qt.CopyAction)
			event.accept()
			links = []
			for url in event.mimeData().urls():
				links.append(str(url.toLocalFile()))
			if len(links)==1:
				file_name=links[0]
				if isfiletype(file_name,"gpvdm")==True:
					self.load_sim(file_name)
				elif os.path.isdir(file_name)==True:
					file_name=os.path.join(file_name,"sim.gpvdm")
					if os.path.isfile(file_name)==True:
						self.load_sim(file_name)
		else:
			event.ignore()
            
if __name__ == '__main__':
	
	app = QApplication(sys.argv)
	sys.excepthook = error_han

	ex = gpvdm_main_window()
	sys.exit(app.exec_())
