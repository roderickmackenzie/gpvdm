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

## @package ribbon_simulations
#  The main time domain ribbon.
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import   QAction
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QSizePolicy,QHBoxLayout,QToolBar, QToolButton,QDialog

from help import help_window

#experiments
from sunsvoc import sunsvoc
from sunsjsc import sunsjsc

from window_eqe import window_eqe
from cost import cost

from util import wrap_text
from global_objects import global_object_run
from PyQt5.QtCore import pyqtSignal
from QAction_lock import QAction_lock
from cal_path import gpvdm_paths
from lock import get_lock
from gpvdm_json import gpvdm_data
from ribbon_page import ribbon_page

class ribbon_simulations(ribbon_page):
	experiments_changed = pyqtSignal()

	def __init__(self):
		ribbon_page.__init__(self)

		self.jvexperiment_window=None
		self.exciton_window=None
		self.experiment_window=None
		self.fxexperiment_window=None
		self.capacitance_voltage_window=None

		self.sunsvocexperiment_window=None
		self.sunsjsc_experiment_window=None
		self.ce_experiment_window=None

		self.qe_window=None
		self.solar_spectrum_window=None
		self.cost_window=None

		self.plexperiment_window=None
		self.spm_window=None
		self.server_config_window=None

		self.jv = QAction_lock("jv", _("JV\neditor"), self,"ribbon_simulations_jv")
		self.jv.clicked.connect(self.callback_jv_window)
		if gpvdm_paths.is_plugin("jv")==True:
			self.addAction(self.jv)

		self.time = QAction_lock("time", _("Time domain\neditor."), self,"ribbon_simulations_time")
		self.time.clicked.connect(self.callback_edit_experiment_window)
		if gpvdm_paths.is_plugin("time_domain")==True:
			self.addAction(self.time )

		self.fx = QAction_lock("spectrum", _("FX domain\neditor"), self,"ribbon_simulations_spectrum")
		self.fx.clicked.connect(self.callback_fxexperiment_window)
		if gpvdm_paths.is_plugin("fx_domain")==True:
			self.addAction(self.fx)

		self.capacitance_voltage = QAction_lock("cv", _("CV\neditor"), self,"ribbon_capacitance_voltage")
		self.capacitance_voltage.clicked.connect(self.callback_capacitance_voltage)
		if gpvdm_paths.is_plugin("cv")==True:
			if get_lock().is_gpvdm_next()==True:
				self.addAction(self.capacitance_voltage)

		self.sunsvoc = QAction_lock("sunsvoc", _("Suns Voc\neditor"), self,"ribbon_simulations_sunsvoc")
		self.sunsvoc.clicked.connect(self.callback_sunsvoc_window)
		if gpvdm_paths.is_plugin("suns_voc")==True:
			self.addAction(self.sunsvoc)

		self.sunsjsc = QAction_lock("sunsjsc", _("Suns Jsc\neditor"), self,"ribbon_simulations_sunsjsc")
		self.sunsjsc.clicked.connect(self.callback_sunsjsc_window)
		if gpvdm_paths.is_plugin("suns_jsc")==True:
			self.addAction(self.sunsjsc)

		self.ce = QAction_lock("ce", _("CE\neditor"), self,"ribbon_simulations_ce")
		self.ce.clicked.connect(self.callback_ce_window)
		if gpvdm_paths.is_plugin("ce")==True:
			self.addAction(self.ce)


		self.pl = QAction_lock("pl", _("PL\neditor"), self,"ribbon_simulations_pl")
		self.pl.clicked.connect(self.callback_pl_window)
		if gpvdm_paths.is_plugin("pl_ss")==True:
			self.addAction(self.pl)

		self.exciton = QAction_lock("exciton", _("Exciton\neditor"), self,"ribbon_simulations_exciton")
		self.exciton.clicked.connect(self.callback_exciton_window)
		if gpvdm_paths.is_plugin("exciton")==True:
			self.addAction(self.exciton)

		self.qe = QAction_lock("qe", _("Quantum\nefficiency"), self,"ribbon_simulations_qe")
		self.qe.clicked.connect(self.callback_qe_window)
		if gpvdm_paths.is_plugin("eqe")==True:
			self.addAction(self.qe)

		#self.qe.setVisible(False)

		self.addSeparator()

		self.tb_cost = QAction_lock("cost", _("Calculate\nthe cost"), self,"ribbon_simulations_cost")
		self.tb_cost.clicked.connect(self.callback_cost)
		self.addAction(self.tb_cost)

		self.addSeparator()

		self.spm = QAction_lock("spm", _("scanning probe\nmicroscopy"), self,"ribbon_simulations_spm")
		self.spm.clicked.connect(self.callback_spm_window)
		if gpvdm_paths.is_plugin("spm")==True:
			self.addAction(self.spm)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)

		self.server_config = QAction_lock("cpu", _("Simulation\nHardware"), self,"server_config")
		self.server_config.clicked.connect(self.callback_server_config)
		self.addAction(self.server_config)

	def callback_experiments_changed(self):
		self.experiments_changed.emit()

	def update(self):
		if self.qe_window!=None:
			del self.qe_window
			self.qe_window=None

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

		if self.spm_window!=None:
			del self.spm_window
			self.spm_window=None

		if self.jvexperiment_window!=None:
			del self.jvexperiment_window
			self.jvexperiment_window=None

		if self.exciton_window!=None:
			del self.exciton_window
			self.exciton_window=None

		#self.mode.update()

	def setEnabled(self,val):

		self.time.setEnabled(val)
		self.fx.setEnabled(val)
		self.capacitance_voltage.setEnabled(val)

		self.qe.setEnabled(val)
		#self.mode.setEnabled(val)
		self.tb_cost.setEnabled(val)
		self.sunsvoc.setEnabled(val)
		self.sunsjsc.setEnabled(val)
		self.ce.setEnabled(val)
		self.pl.setEnabled(val)
		self.spm.setEnabled(val)
		self.jv.setEnabled(val)
		self.exciton.setEnabled(val)
		self.server_config.setEnabled(val)

	def callback_edit_experiment_window(self):
		from time_domain_experiment import time_domain_experiment
		if self.experiment_window==None:
			self.experiment_window=time_domain_experiment()
			self.experiment_window.changed.connect(self.callback_experiments_changed)
			
		help_window().help_set_help(["time.png",_("<big><b>The time mesh editor</b></big><br> To do time domain simulations one must define how voltage the light vary as a function of time.  This can be done in this window.  Also use this window to define the simulation length and time step.")])
		if self.experiment_window.isVisible()==True:
			self.experiment_window.hide()
		else:
			self.experiment_window.show()
 
	def callback_fxexperiment_window(self):
		from fxexperiment import fxexperiment
		if self.fxexperiment_window==None:
			self.fxexperiment_window=fxexperiment()

		help_window().help_set_help(["spectrum.png",_("<big><b>Frequency domain mesh editor</b></big><br> Some times it is useful to do frequency domain simulations such as when simulating impedance spectroscopy.  This window will allow you to choose which frequencies will be simulated.")])
		if self.fxexperiment_window.isVisible()==True:
			self.fxexperiment_window.hide()
		else:
			self.fxexperiment_window.show()
		
	def callback_capacitance_voltage(self):
		from cv_experiment import cv_experiment
		if self.capacitance_voltage_window==None:
			self.capacitance_voltage_window=cv_experiment()
			
		help_window().help_set_help(["cv.png",_("<big><b>Capacitance voltage editor</b></big><br> Use this editor to change serup capacitance voltage simulation.")])
		if self.capacitance_voltage_window.isVisible()==True:
			self.capacitance_voltage_window.hide()
		else:
			self.capacitance_voltage_window.show()


	def callback_spm_window(self):
		from spm_experiment import spm_experiment
		if self.spm_window==None:
			self.spm_window=spm_experiment()

		help_window().help_set_help(["spm.png",_("<big><b>Scanning probe microscopy</b></big><br> Use this window to configure the scanning probe microscopy simulations.")])
		if self.spm_window.isVisible()==True:
			self.spm_window.hide()
		else:
			self.spm_window.show()

	def callback_pl_window(self):
		from pl_experiment import pl_experiment
		if self.plexperiment_window==None:
			self.plexperiment_window=pl_experiment()
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

	def callback_ce_window(self):
		from window_ce import window_ce
		if self.ce_experiment_window==None:
			self.ce_experiment_window=window_ce()

		help_window().help_set_help(["ce.png",_("<big><b>Charge Extraction editor</b></big><br> This performs charge extraction experiments in time domain, accounting for recombination losses.  If you don’t mind about accounting for recombination losses on extraction just run a JV curve or a Suns-Voc simulation and use the charge.dat file.")])
		if self.ce_experiment_window.isVisible()==True:
			self.ce_experiment_window.hide()
		else:
			self.ce_experiment_window.show()
	
	def callback_qe_window(self, widget):
		if self.qe_window==None:
			self.qe_window=window_eqe()

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


	def callback_jv_window(self):
		from jv_experiment import jv_experiment
		if self.jvexperiment_window==None:
			self.jvexperiment_window=jv_experiment()
			#self.experiment_window.changed.connect(self.callback_experiments_changed)

		help_window().help_set_help(["jv.png",_("<big><b>JV simulation editor</b></big><br> Use this window to configure JV simulations.")])
		if self.jvexperiment_window.isVisible()==True:
			self.jvexperiment_window.hide()
		else:
			self.jvexperiment_window.show()

	def callback_exciton_window(self):
		from window_exciton import window_exciton
		if self.exciton_window==None:
			self.exciton_window=window_exciton()
			#self.experiment_window.changed.connect(self.callback_experiments_changed)

		help_window().help_set_help(["exciton.png",_("<big><b>Exciton simulation editor</b></big><br> Use this window to configure exciton simulations.")])
		if self.exciton_window.isVisible()==True:
			self.exciton_window.hide()
		else:
			self.exciton_window.show()

	def callback_server_config(self):
		help_window().help_set_help(["cpu.png",_("<big><b>Simulation hardware</b></big><br>Use this window to change how the model uses the computer's hardware.")])


		if self.server_config_window==None:
			from server_config import server_config
			self.server_config_window=server_config()

		if self.server_config_window.isVisible()==True:
			self.server_config_window.hide()
		else:
			self.server_config_window.show()
