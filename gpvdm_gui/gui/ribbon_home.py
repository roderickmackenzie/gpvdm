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

## @package ribbon_home
#  The home ribbon.
#


import os
from tb_item_sun import tb_item_sun

#qt
from PyQt5.QtWidgets import QTextEdit, QAction
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QToolButton,QDialog

from plot_gen import plot_gen
from window_json_ro_viewer import window_json_ro_viewer

#windows
from scan import scan_class
from help import help_window
from gpvdm_open import gpvdm_open
from error_dlg import error_dlg
from server import server_get
from fit_window import fit_window

from global_objects import global_object_run
from util import isfiletype
from icon_lib import icon_get

from cal_path import get_sim_path
from inp import inp
from play import play
from util import wrap_text

import webbrowser
from global_objects import global_object_register

from lock import get_lock
from QAction_lock import QAction_lock
from ribbon_page import ribbon_page

class ribbon_home(ribbon_page):

	def __init__(self):
		ribbon_page.__init__(self)
		self.myserver=server_get()

		self.scan_window=None
		self.fit_window=None
		self.optics_window=None
		self.simulation_notes_window=None
		self.server_config_window=None
		self.undo = QAction(icon_get("edit-undo"), _("Undo"), self)
		self.addAction(self.undo)

		self.addSeparator()

		self.run = play(self,"main_play_button",run_text=wrap_text(_("Run\nsimulation"),2))#QAction(icon_get("media-playback-start"), _("Run simulation"), self)
		server_get().sim_finished.connect(self.run.stop)
		server_get().sim_started.connect(self.run.start)

		self.addAction(self.run)

		#self.stop = QAction(icon_get("media-playback-pause"), _("Stop\nsimulation"), self)
		#self.stop.triggered.connect(self.callback_simulate_stop)
		#self.addAction(self.stop)

		self.addSeparator()

		self.scan = QAction_lock("scan", _("Parameter\nscan"), self,"ribbon_home_scan")
		self.scan.clicked.connect(self.callback_scan)
		self.addAction(self.scan)

		self.addSeparator()

		self.fit = QAction_lock("fit", _("Fit to\nexperimental data"), self,"ribbon_home_fit")
		self.fit.clicked.connect(self.callback_run_fit)
		self.addAction(self.fit)

		self.addSeparator()

		self.plot = QAction_lock("plot", _("Plot\nFile"), self,"ribbon_home_plot")
		self.plot.clicked.connect(self.callback_plot_select)
		self.addAction(self.plot)

		self.addSeparator()

		self.optics = QAction_lock("optics", _("Optical\nSimulation"), self,"ribbon_home_optics")
		self.optics.clicked.connect(self.callback_optics_sim)
		self.addAction(self.optics)



		self.sun=tb_item_sun()
		#self.sun.changed.connect(self.callback_sun)
		self.addWidget(self.sun)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)

		self.server_config = QAction_lock("cpu", _("Simulation\nHardware"), self,"server_config")
		self.server_config.clicked.connect(self.callback_server_config)
		self.addAction(self.server_config)

		self.simulation_notes = QAction_lock("text-x-generic", _("Simulation\nNotes"), self,"ribbon_home_notes")
		self.simulation_notes.clicked.connect(self.callback_simulation_notes)
		self.addAction(self.simulation_notes)

		self.help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		self.addAction(self.help)



	#def callback_sun(self):
	#	global_object_run("gl_force_redraw")

	def update(self):
		if self.scan_window!=None:
			del self.scan_window
			self.scan_window=None

		if self.fit_window!=None:
			del self.fit_window
			self.fit_window=None

		if self.optics_window!=None:
			del self.optics_window
			self.optics_window=None

		self.sun.update()

		self.fit.setVisible(True)

	def setEnabled(self,val):
		self.undo.setEnabled(val)
		self.run.setEnabled(val)
		#self.stop.setEnabled(val)
		self.scan.setEnabled(val)
		self.plot.setEnabled(val)
		self.sun.setEnabled(val)
		self.help.setEnabled(val)
		self.fit.setEnabled(val)
		self.optics.setEnabled(val)
		self.server_config.setEnabled(val)

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

			plot_gen([dialog.get_filename()],[],"auto")

			#self.plotted_graphs.refresh()
			#self.plot_after_run_file=dialog.get_filename()

	def callback_scan(self, widget):
		help_window().help_set_help(["scan.png",_("<big><b>The scan window</b></big><br> Very often it is useful to be able to systematically very a device parameter such as mobility or density of trap states.  This window allows you to do just that."),"list-add.png",_("Use the plus icon to add a new scan line to the list."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=cpkPht-CKeE\">Tutorial video</b></big><br>Using the parameter scan window.")])
		#self.tb_run_scan.setEnabled(True)

		if self.scan_window==None:
			self.scan_window=scan_class(server_get())


		if self.scan_window.isVisible()==True:
			self.scan_window.hide()
		else:
			self.scan_window.show()

	def callback_simulate_stop(self):
		server_get().killall()

	def callback_run_fit(self, widget):
		if self.fit_window==None:
			self.fit_window=fit_window("fit")

		help_window().help_set_help(["fit.png",_("<big><b>Fit window</b></big><br> Use this window to fit the simulation to experimental data.  Gpvdm uses advanced and optimized fitting algorithms to fit the model to your experimental data, so that material parameters such as mobilities and recombination rates can be extracted."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=61umU4hrsqk\">Watch the tutorial video 1</b></big><br>Fitting gpvdm to experimental data to extract mobility and recombination rate parameters."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=_cm3Cb3kzUg\">Watch the tutorial video 2</b></big><br>Fitting gpvdm to large area solar cells")])
		if self.fit_window.isVisible()==True:
			self.fit_window.hide()
		else:
			self.fit_window.show()

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

	def callback_simulation_notes(self, widget, data=None):
		help_window().help_set_help(["si.png",_("<big><b>Record notes about the simulation here</b></big><br>Use this window to make notes about the simulation.")])


		if self.simulation_notes_window==None:
			from window_simulation_notes import window_simulation_notes
			self.simulation_notes_window=window_simulation_notes()
			#self.notebook.changed.connect(self.optics_window.update)

		if self.simulation_notes_window.isVisible()==True:
			self.simulation_notes_window.hide()
		else:
			self.simulation_notes_window.show()

	def callback_server_config(self):
		help_window().help_set_help(["cpu.png",_("<big><b>Simulation hardware</b></big><br>Use this window to change how the model uses the computer's hardware.")])


		if self.server_config_window==None:
			from server_config import server_config
			self.server_config_window=server_config()
			#self.notebook.changed.connect(self.optics_window.update)

		if self.server_config_window.isVisible()==True:
			self.server_config_window.hide()
		else:
			self.server_config_window.show()

