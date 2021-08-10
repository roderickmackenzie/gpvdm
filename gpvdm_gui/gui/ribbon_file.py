# -*- coding: utf-8 -*-
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

## @package ribbon_file
#  A ribbon for the file menu
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import  QAction, QMenu
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QToolButton
from PyQt5.QtWidgets import QTabWidget

import webbrowser
from lock import get_lock
from QAction_lock import QAction_lock
from used_files import used_files_load
from PyQt5.QtCore import pyqtSignal

from gpvdm_open import gpvdm_open
from cal_path import get_backup_path
from cal_path import get_sim_path

from util import wrap_text

from util_zip import write_lines_to_archive
from gui_util import dlg_get_text
from backup import backup
from scripts import scripts
from cite_me import cite_me
from ribbon_page import ribbon_page
from play import play
from server import server_get
from help import help_window

class ribbon_file(ribbon_page):
	used_files_click= pyqtSignal(str)
	def __init__(self):
		ribbon_page.__init__(self)
		self.scan_window=None
		self.fit_window=None

		self.myserver=server_get()

		self.home_new = QAction_lock("document-new", _("New simulation").replace(" ","\n"), self,"main_new")
		#self.home_new.setText(_("New\nsimulation"))
		self.addAction(self.home_new)

		self.old = QAction(icon_get("document-new"), _("New simulation").replace(" ","\n"), self)


		self.home_open = QAction_lock("document-open", _("Open\nsimulation"), self,"main_open")


		self.used_files_menu = QMenu(self)
		self.populate_used_file_menu()
		self.home_open.setMenu(self.used_files_menu)
		self.addAction(self.home_open)

		self.home_export = QAction_lock("zip", _("Export\nZip"), self,"main_zip")
		self.addAction(self.home_export)

		self.home_backup = QAction_lock("backup", _("Backup\nSimulaion"), self,"ribbion_db_backup")
		self.home_backup.clicked.connect(self.callback_home_backup)
		self.addAction(self.home_backup)

		self.tb_script_editor = QAction_lock("script", _("Script\nEditor"), self,"script_editor")
		self.tb_script_editor.clicked.connect(self.callback_script)
		self.addAction(self.tb_script_editor)

		self.addSeparator()

		self.run = play(self,"main_play_button",run_text=wrap_text(_("Run\nsimulation"),2))#QAction(icon_get("media-playback-start"), _("Run simulation"), self)
		server_get().sim_finished.connect(self.run.stop)
		server_get().sim_started.connect(self.run.start)

		self.addAction(self.run)

		self.scan = QAction_lock("scan", _("Parameter\nscan"), self,"ribbon_home_scan")
		self.scan.clicked.connect(self.callback_scan)
		self.addAction(self.scan)

		self.addSeparator()

		self.fit = QAction_lock("fit", _("Fit to\nexperimental data"), self,"ribbon_home_fit")
		self.fit.clicked.connect(self.callback_run_fit)
		self.addAction(self.fit)

		self.plot = QAction_lock("plot", _("Plot\nFile"), self,"ribbon_home_plot")
		self.plot.clicked.connect(self.callback_plot_select)
		self.addAction(self.plot)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)

		self.cite_me=cite_me()
		self.addWidget(self.cite_me)

		self.home_help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		self.addAction(self.home_help)


	def populate_used_file_menu(self):
		self.used_files_menu.clear()
		files=used_files_load()
		for f in files:
			f=QAction(f, self)
			f.triggered.connect(self.callback_menu)
			self.used_files_menu.addAction(f)

	def callback_menu(self):
		action = self.sender()
		self.used_files_click.emit(action.text())

	def update(self):
		if self.fit_window!=None:
			del self.fit_window
			self.fit_window=None

		self.fit.setVisible(True)

		if self.scan_window!=None:
			del self.scan_window
			self.scan_window=None

		self.populate_used_file_menu()


	def setEnabled(self,val,do_all=False):
		self.home_new.setEnabled(val)
		self.home_open.setEnabled(val)
		self.home_backup.setEnabled(val)
		self.home_export.setEnabled(val)
		self.plot.setEnabled(val)

	def setEnabled_other(self,val):
		self.home_backup.setEnabled(val)
		self.home_export.setEnabled(val)
		self.tb_script_editor.setEnabled(val)
		self.fit.setEnabled(val)
		self.scan.setEnabled(val)

	def on_new_backup(self):
		new_backup_name=dlg_get_text( _("New backup:"), _("New backup name"),"add_backup")
		new_backup_name=new_backup_name.ret
		if new_backup_name!=None:
			new_backup=os.path.join(self.dialog.viewer.path,new_backup_name)
			backup(new_backup,get_sim_path())
			self.dialog.viewer.fill_store()

	def callback_home_backup(self):
		backup_path=get_backup_path()
		if os.path.isdir(backup_path)==False:
			os.makedirs(backup_path)

		lines=[]
		lines.append("#gpvdm_file_type")
		lines.append("backup_main")
		lines.append("#end")

		write_lines_to_archive(os.path.join(backup_path,"sim.gpvdm"),"mat.inp",lines,mode="l",dest="file")

		self.dialog=gpvdm_open(backup_path,big_toolbar=True)
		self.new_backup = QAction_lock("add_backup", wrap_text(_("New backup"),2), self,"add_backup")
		self.new_backup.clicked.connect(self.on_new_backup)
		self.dialog.toolbar.addAction(self.new_backup)

		self.dialog.viewer.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_script(self):
		self.scripts=scripts()
		self.scripts.show()

	def callback_scan(self, widget):
		help_window().help_set_help(["scan.png",_("<big><b>The scan window</b></big><br> Very often it is useful to be able to systematically very a device parameter such as mobility or density of trap states.  This window allows you to do just that."),"list-add.png",_("Use the plus icon to add a new scan line to the list."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=cpkPht-CKeE\">Tutorial video</b></big><br>Using the parameter scan window.")])
		#self.tb_run_scan.setEnabled(True)

		if self.scan_window==None:
			from scan import scan_class
			self.scan_window=scan_class(server_get())


		if self.scan_window.isVisible()==True:
			self.scan_window.hide()
		else:
			self.scan_window.show()

	def callback_run_fit(self, widget):
		if self.fit_window==None:
			from fit_window import fit_window
			self.fit_window=fit_window("fit")

		help_window().help_set_help(["fit.png",_("<big><b>Fit window</b></big><br> Use this window to fit the simulation to experimental data.  Gpvdm uses advanced and optimized fitting algorithms to fit the model to your experimental data, so that material parameters such as mobilities and recombination rates can be extracted."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=61umU4hrsqk\">Watch the tutorial video 1</b></big><br>Fitting gpvdm to experimental data to extract mobility and recombination rate parameters."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=_cm3Cb3kzUg\">Watch the tutorial video 2</b></big><br>Fitting gpvdm to large area solar cells")])
		if self.fit_window.isVisible()==True:
			self.fit_window.hide()
		else:
			self.fit_window.show()

	def callback_plot_select(self):
		help_window().help_set_help(["dat_file.png",_("<big>Select a file to plot</big><br>Single clicking shows you the content of the file")])

		dialog=gpvdm_open(get_sim_path(),show_inp_files=False)
		dialog.show_directories=False
		ret=dialog.exec_()
		if ret==QDialog.Accepted:
			file_name=dialog.get_filename()

			if os.path.basename(dialog.get_filename())=="sim_info.dat":
				self.sim_info_window=window_json_ro_viewer(dialog.get_filename())
				self.sim_info_window.show()
				return
			from plot_gen import plot_gen
			plot_gen([dialog.get_filename()],[],"auto")

			#self.plotted_graphs.refresh()
			#self.plot_after_run_file=dialog.get_filename()



