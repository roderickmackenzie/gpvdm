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

## @package ribbon_simulations
#  The main time domain ribbon.
#


import os
from icon_lib import icon_get

from dump_io import dump_io
from tb_item_sim_mode import tb_item_sim_mode

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import   QAction
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QHBoxLayout,QToolBar, QToolButton,QDialog

from help import help_window

#experiments
from experiment import experiment
from fxexperiment import fxexperiment
from sunsvoc import sunsvoc
from sunsjsc import sunsjsc

from ray_trace_editor import ray_trace_editor

from qe import qe_window
from measure import measure
from cost import cost

from jvexperiment import jvexperiment
from plexperiment import plexperiment
from cv_editor import cv_editor

from util import wrap_text
from fdtd import fdtd
from global_objects import global_object_run
from PyQt5.QtCore import pyqtSignal
from QAction_lock import QAction_lock
from cal_path import gpvdm_paths

class ribbon_simulations(QToolBar):
	experiments_changed = pyqtSignal()

	def __init__(self):
		QToolBar.__init__(self)

		self.jvexperiment_window=None
		self.experiment_window=None
		self.fxexperiment_window=None
		self.capacitance_voltage_window=None

		self.sunsvocexperiment_window=None
		self.sunsjsc_experiment_window=None

		self.qe_window=None
		self.measure_window=None
		self.solar_spectrum_window=None
		self.cost_window=None
		self.ray_trace_window=None
		self.fdtd_window=None

		self.plexperiment_window=None

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))

		self.jv = QAction_lock("jv", _("Steady state\nsimulation editor"), self,"ribbon_simulations_jv")
		self.jv.clicked.connect(self.callback_jv_window)
		if gpvdm_paths.is_plugin("jv")==True:
			self.addAction(self.jv)

		self.time = QAction_lock("time", _("Time domain\nsimulation editor."), self,"ribbon_simulations_time")
		self.time.clicked.connect(self.callback_edit_experiment_window)
		if gpvdm_paths.is_plugin("pulse")==True:
			self.addAction(self.time )

		self.fx = QAction_lock("spectrum", _("Frequency domain\nsimulation editor"), self,"ribbon_simulations_spectrum")
		self.fx.clicked.connect(self.callback_fxexperiment_window)
		if gpvdm_paths.is_plugin("is")==True:
			self.addAction(self.fx)

		self.capacitance_voltage = QAction_lock("cv", _("Capacitance voltage\nsimulation editor"), self,"ribbon_capacitance_voltage")
		self.capacitance_voltage.clicked.connect(self.callback_capacitance_voltage)
		if gpvdm_paths.is_plugin("cv")==True:
			self.addAction(self.capacitance_voltage)

		self.sunsvoc = QAction_lock("sunsvoc", _("Suns Voc\nsimulation editor"), self,"ribbon_simulations_sunsvoc")
		self.sunsvoc.clicked.connect(self.callback_sunsvoc_window)
		if gpvdm_paths.is_plugin("sun_voc")==True:
			self.addAction(self.sunsvoc)

		self.sunsjsc = QAction_lock("sunsjsc", _("Suns Jsc\neditor"), self,"ribbon_simulations_sunsjsc")
		self.sunsjsc.clicked.connect(self.callback_sunsjsc_window)
		if gpvdm_paths.is_plugin("suns_jsc")==True:
			self.addAction(self.sunsjsc)

		self.ray_trace = QAction_lock("ray", wrap_text(_("Ray tracing\neditor"),8), self,"ribbon_simulations_ray")
		self.ray_trace.clicked.connect(self.callback_ray_tracing_window)
		if gpvdm_paths.is_plugin("trace")==True:
			self.addAction(self.ray_trace)

		self.pl = QAction_lock("pl", _("PL\neditor"), self,"ribbon_simulations_pl")
		self.pl.clicked.connect(self.callback_pl_window)
		if gpvdm_paths.is_plugin("pl_ss")==True:
			self.addAction(self.pl)

		self.qe = QAction_lock("qe", _("Quantum\nefficiency"), self,"ribbon_simulations_qe")
		self.qe.clicked.connect(self.callback_qe_window)
		if gpvdm_paths.is_plugin("eqe")==True:
			self.addAction(self.qe)
		#self.qe.setVisible(False)

		self.addSeparator()
		#self.mode=tb_item_sim_mode()
		#self.addWidget(self.mode)
		#self.addSeparator()

		self.fdtd = QAction_lock("fdtd", _("FDTD\nSimulation"), self,"ribbon_simulations_fdtd")
		self.fdtd.clicked.connect(self.callback_fdtd)
		self.addAction(self.fdtd)

		self.measure = QAction_lock("measure", _("Measure"), self,"ribbon_simulations_measure")
		self.measure.clicked.connect(self.callback_configure_measure)
		self.addAction(self.measure)

		self.tb_cost = QAction_lock("cost", _("Calculate\nthe cost"), self,"ribbon_simulations_cost")
		self.tb_cost.clicked.connect(self.callback_cost)
		self.addAction(self.tb_cost)

	def callback_experiments_changed(self):
		self.experiments_changed.emit()

	def update(self):
		if self.qe_window!=None:
			del self.qe_window
			self.qe_window=None

		if self.jvexperiment_window!=None:
			del self.jvexperiment_window
			self.jvexperiment_window=None

		if self.experiment_window!=None:
			del self.experiment_window
			self.experiment_window=None

		if self.fxexperiment_window!=None:
			del self.fxexperiment_window
			self.fxexperiment_window=None

		if self.capacitance_voltage_window!=None:
			del self.capacitance_voltage_window
			self.capacitance_voltage_window=None

		if self.solar_spectrum_window!=None:
			del self.solar_spectrum_window
			self.solar_spectrum_window=None

		if self.cost_window!=None:
			del self.cost_window
			self.cost_window=None


		#self.mode.update()

	def setEnabled(self,val):

		self.jv.setEnabled(val)
		self.time.setEnabled(val)
		self.fx.setEnabled(val)
		self.capacitance_voltage.setEnabled(val)

		self.qe.setEnabled(val)
		#self.mode.setEnabled(val)
		self.tb_cost.setEnabled(val)
		self.sunsvoc.setEnabled(val)
		self.sunsjsc.setEnabled(val)
		self.measure.setEnabled(val)
		self.fdtd.setEnabled(val)
		self.ray_trace.setEnabled(val)
		self.pl.setEnabled(val)

	def callback_edit_experiment_window(self):

		if self.experiment_window==None:
			self.experiment_window=experiment()
			self.experiment_window.changed.connect(self.callback_experiments_changed)
			
		help_window().help_set_help(["time.png",_("<big><b>The time mesh editor</b></big><br> To do time domain simulations one must define how voltage the light vary as a function of time.  This can be done in this window.  Also use this window to define the simulation length and time step.")])
		if self.experiment_window.isVisible()==True:
			self.experiment_window.hide()
		else:
			self.experiment_window.show()
 
	def callback_fxexperiment_window(self):

		if self.fxexperiment_window==None:
			self.fxexperiment_window=fxexperiment()
			
		help_window().help_set_help(["spectrum.png",_("<big><b>Frequency domain mesh editor</b></big><br> Some times it is useful to do frequency domain simulations such as when simulating impedance spectroscopy.  This window will allow you to choose which frequencies will be simulated.")])
		if self.fxexperiment_window.isVisible()==True:
			self.fxexperiment_window.hide()
		else:
			self.fxexperiment_window.show()
		
	def callback_capacitance_voltage(self):

		if self.capacitance_voltage_window==None:
			self.capacitance_voltage_window=cv_editor()
			
		help_window().help_set_help(["cv.png",_("<big><b>Capacitance voltage editor</b></big><br> Use this editor to change serup capacitance voltage simulation.")])
		if self.capacitance_voltage_window.isVisible()==True:
			self.capacitance_voltage_window.hide()
		else:
			self.capacitance_voltage_window.show()

	def callback_configure_measure(self):

		if self.measure_window==None:
			self.measure_window=measure()

		help_window().help_set_help(["measure.png",_("<big><b>Measure window</b></big><br>Use this window to set up measurement points.  If for example you want to extract the value of current density from jv.dat at 0.2 Volts, set a measurement point for jv.dat to 0.2 V.  This will work with any file")])
		if self.measure_window.isVisible()==True:
			self.measure_window.hide()
		else:
			self.measure_window.show()
			

	def callback_jv_window(self):

		if self.jvexperiment_window==None:
			self.jvexperiment_window=jvexperiment()
			#self.experiment_window.changed.connect(self.callback_experiments_changed)

		help_window().help_set_help(["jv.png",_("<big><b>JV simulation editor</b></big><br> Use this window to configure the Suns Voc simulations.")])
		if self.jvexperiment_window.isVisible()==True:
			self.jvexperiment_window.hide()
		else:
			self.jvexperiment_window.show()

	def callback_pl_window(self):

		if self.plexperiment_window==None:
			self.plexperiment_window=plexperiment()
			#self.experiment_window.changed.connect(self.callback_experiments_changed)

		help_window().help_set_help(["pl.png",_("<big><b>PL simulation editor</b></big><br> Use this window to configure the steady state photoluminescence simulation."),"youtube",_("<big><b><a href=\"https://www.youtube.com/watch?v=pgaJg6dErP4\">Watch the youtube video</a></b></big><br>Watch the video on simulating PL using gpvdm")])
		if self.plexperiment_window.isVisible()==True:
			self.plexperiment_window.hide()
		else:
			self.plexperiment_window.show()

	def callback_sunsvoc_window(self):

		if self.sunsvocexperiment_window==None:
			self.sunsvocexperiment_window=sunsvoc()

		help_window().help_set_help(["jv.png",_("<big><b>Suns voc simulation editor</b></big><br> Use this window to select the step size and parameters of the JV simulations.")])
		if self.sunsvocexperiment_window.isVisible()==True:
			self.sunsvocexperiment_window.hide()
		else:
			self.sunsvocexperiment_window.show()

	def callback_sunsjsc_window(self):

		if self.sunsjsc_experiment_window==None:
			self.sunsjsc_experiment_window=sunsjsc()

		help_window().help_set_help(["jv.png",_("<big><b>Suns Jsc simulation editor</b></big><br> Use this window to select the step size and parameters of the JV simulations.")])
		if self.sunsjsc_experiment_window.isVisible()==True:
			self.sunsjsc_experiment_window.hide()
		else:
			self.sunsjsc_experiment_window.show()

	def callback_ray_tracing_window(self):

		if self.ray_trace_window==None:
			self.ray_trace_window=ray_trace_editor()

		help_window().help_set_help(["ray.png",_("<big><b>The ray tracing editor</b></big><br> Use this window to configure ray tracing.")])
		if self.ray_trace_window.isVisible()==True:
			self.ray_trace_window.hide()
		else:
			self.ray_trace_window.show()



	def callback_fdtd(self):

		if self.fdtd_window==None:
			self.fdtd_window=fdtd()

		help_window().help_set_help(["fdtd.png",_("<big><b>FDTD</b></big><br> Use this window to setup a finite difference time domain simulation.")])
		if self.fdtd_window.isVisible()==True:
			self.fdtd_window.hide()
		else:
			self.fdtd_window.show()


			
	def callback_qe_window(self, widget):
		if self.qe_window==None:
			self.qe_window=qe_window()

		if self.qe_window.isVisible()==True:
			self.qe_window.hide()
		else:
			self.qe_window.show()

	def callback_cost(self):
		help_window().help_set_help(["cost.png",_("<big><b>Costs window</b></big>\nUse this window to calculate the cost of the solar cell and the energy payback time.")])

		if self.cost_window==None:
			self.cost_window=cost()

		if self.cost_window.isVisible()==True:
			self.cost_window.hide()
		else:
			self.cost_window.show()
