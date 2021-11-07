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

## @package ribbon_electrical
#  The configure ribbon.
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from config_window import class_config_window

from help import help_window

from global_objects import global_object_register

from gpvdm_open import gpvdm_open
from QAction_lock import QAction_lock
from gui_util import dlg_get_text

from lock import get_lock
from gpvdm_json import gpvdm_data
from cal_path import gpvdm_paths
from ribbon_page import ribbon_page
from ribbon_electrical import ribbon_page2
from fx_selector import fx_selector
from util import wrap_text
from tb_item_sun import tb_item_sun

class ribbon_optical(ribbon_page2):
	def __init__(self):
		ribbon_page2.__init__(self)
		self.enabled=False
		self.light_sources_window=None
		self.ray_trace_window=None
		self.detectors_window=None
		self.fdtd_window=None

		pan=self.add_panel()
		self.light_sources = QAction_lock("lighthouse", _("Light\nSources"), self,"ribbon_home_light_sources")
		self.light_sources.clicked.connect(self.callback_light_sources)
		pan.addAction(self.light_sources)


		self.lasers = QAction_lock("lasers", _("Lasers\n(fs)"), self,"ribbion_db_lasers")
		self.lasers.clicked.connect(self.callback_configure_lasers)
		pan.addAction(self.lasers)

		self.sun=tb_item_sun()
		pan.addWidget(self.sun)


		pan.addSeparator()

		self.ray_trace = QAction_lock("ray", wrap_text(_("Ray tracing\neditor"),8), self,"ribbon_simulations_ray")
		self.ray_trace.clicked.connect(self.callback_ray_tracing_window)
		if gpvdm_paths.is_plugin("trace")==True:
			pan.addAction(self.ray_trace)

		self.detector = QAction_lock("ccd", _("Optical\nDetectors"), self,"ribbon_home_optics")
		self.detector.clicked.connect(self.callback_detector)
		pan.addAction(self.detector)

		self.fdtd = QAction_lock("fdtd", _("FDTD\nSimulation"), self,"ribbon_simulations_fdtd")
		self.fdtd.clicked.connect(self.callback_fdtd)
		pan.addAction(self.fdtd)

		self.fx_box=fx_selector()
		self.fx_box.update()
		global_object_register("main_fx_box",self.fx_box)

		pan.addWidget(self.fx_box)

		self.lasers_window=None

	def update(self):
		if self.light_sources_window!=None:
			del self.light_sources_window
			self.light_sources_window=None

		if self.lasers_window!=None:
			del self.lasers_window
			self.lasers_window=None

		self.sun.update()

	def setEnabled(self,val):
		self.light_sources.setEnabled(val)
		self.optics.setEnabled(val)
		self.ray_trace.setEnabled(val)
		self.sun.setEnabled(val)
		self.lasers.setEnabled(val)
		self.fdtd.setEnabled(val)

	def callback_configure_lasers(self):
		from lasers import lasers
		data=gpvdm_data()
		if self.lasers_window==None:
			self.lasers_window=lasers(data.lasers)

		help_window().help_set_help(["lasers.png",_("<big><b>Laser setup</b></big><br> Use this window to set up your lasers.")])
		if self.lasers_window.isVisible()==True:
			self.lasers_window.hide()
		else:
			self.lasers_window.show()

	def callback_light_sources(self, widget, data=None):
		help_window().help_set_help(["lighthouse.png",_("<big><b>The light sources window</b></big><br>Use this window to setup optical sources for the transfer matrix, ray tracing and FDTD simulations.")])


		if self.light_sources_window==None:
			from window_light_src import window_light_src
			self.light_sources_window=window_light_src()
			#self.notebook.changed.connect(self.optics_window.update)

		if self.light_sources_window.isVisible()==True:
			self.light_sources_window.hide()
		else:
			self.light_sources_window.show()

	def callback_ray_tracing_window(self):

		if self.ray_trace_window==None:
			from ray_trace_editor import ray_trace_editor
			self.ray_trace_window=ray_trace_editor()

		help_window().help_set_help(["ray.png",_("<big><b>The ray tracing editor</b></big><br> Use this window to configure ray tracing.")])
		if self.ray_trace_window.isVisible()==True:
			self.ray_trace_window.hide()
		else:
			self.ray_trace_window.show()

	def callback_detector(self):
		help_window().help_set_help(["ccd.png",_("<big><b>The detectors window</b></big><br>Use this window to setup optical detectors.  These can be used for the ray tracing simulations.")])


		if self.detectors_window==None:
			from detectors_editor import detectors_editor
			self.detectors_window=detectors_editor()

		if self.detectors_window.isVisible()==True:
			self.detectors_window.hide()
		else:
			self.detectors_window.ribbon.update()
			self.detectors_window.show()

	def callback_fdtd(self):
		from window_fdtd import window_fdtd
		if self.fdtd_window==None:
			self.fdtd_window=window_fdtd()

		help_window().help_set_help(["fdtd.png",_("<big><b>FDTD</b></big><br> Use this window to setup a finite difference time domain simulation.")])
		if self.fdtd_window.isVisible()==True:
			self.fdtd_window.hide()
		else:
			self.fdtd_window.show()


