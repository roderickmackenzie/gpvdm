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

from util import wrap_text

from gui_util import dlg_get_text
from scripts import scripts
from cite_me import cite_me
from ribbon_page import ribbon_page
from play import play
from server import server_get
from help import help_window
from global_objects import global_object_register
from gpvdm_local import gpvdm_local

class ribbon_file(ribbon_page):
	used_files_click= pyqtSignal(str)
	def __init__(self):
		ribbon_page.__init__(self)
		self.scan_window=None
		self.fit_window=None
		self.optics_window=None
		self.ml_window=None

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

		self.optics = QAction_lock("optics", _("Optical\nSimulation"), self,"ribbon_home_optics")
		self.optics.clicked.connect(self.callback_optics_sim)
		self.addAction(self.optics)

		self.addSeparator()
		self.tb_ml_build_vectors = QAction(icon_get("ml"), wrap_text(_("Machine\nLearning"),4), self)

		if get_lock().is_gpvdm_next()==True:
			self.addAction(self.tb_ml_build_vectors)
			self.tb_ml_build_vectors.triggered.connect(self.callback_ml)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)

		self.cite_me=cite_me()
		self.addWidget(self.cite_me)

		#self.home_help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		#self.addAction(self.home_help)
		self.tb_script_editor = QAction_lock("script", _("Script\nEditor"), self,"script_editor")
		self.tb_script_editor.clicked.connect(self.callback_script)
		self.addAction(self.tb_script_editor)

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

		if self.optics_window!=None:
			del self.optics_window
			self.optics_window=None

		if self.ml_window!=None:
			del self.ml_window
			self.ml_window=None


		self.populate_used_file_menu()


	def setEnabled(self,val,do_all=False):
		self.home_new.setEnabled(val)
		self.home_open.setEnabled(val)
		self.home_export.setEnabled(val)
		#self.plot.setEnabled(val)

	def setEnabled_other(self,val):
		self.home_export.setEnabled(val)
		self.tb_script_editor.setEnabled(val)
		self.fit.setEnabled(val)
		self.scan.setEnabled(val)
		self.tb_ml_build_vectors.setEnabled(val)
		self.run.setEnabled(val)
		self.optics.setEnabled(val)
		self.cite_me.setEnabled(val)

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


	#def callback_mb_build_vectors(self):
	#	tab = self.notebook.currentWidget()
	#	tab.scan_tab_ml_build_vector()


	def callback_optics_sim(self, widget, data=None):
		help_window().help_set_help(["optics.png",_("<big><b>The optical simulation window</b></big><br>Use this window to perform optical simulations.  Click on the play button to run a simulation."),"media-playback-start",_("Click on the play button to run an optical simulation.  The results will be displayed in the tabs to the right."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=A_3meKTBuWk\">Tutorial video</b></big><br>Designing optical filters and reflective coatings.")])


		if self.optics_window==None:
			from optics import class_optical
			self.optics_window=class_optical()
			#self.notebook.changed.connect(self.optics_window.update)

		if self.optics_window.isVisible()==True:
			self.optics_window.hide()
		else:
			global_object_register("optics_force_redraw",self.optics_window.force_redraw)
			self.optics_window.ribbon.update()
			self.optics_window.show()

	def callback_ml(self, widget, data=None):
		help_window().help_set_help(["ml.png",_("<big><b>Machine learning window</b></big><br>Use this window to generate machine learning vectors")])


		if self.ml_window==None:
			from window_ml import window_ml
			self.window_ml=window_ml()

		if self.window_ml.isVisible()==True:
			self.window_ml.hide()
		else:
			self.window_ml.show()
