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

## @package tab_view
#  A tab used to show the results of the simulation.
#

import math
from layer_widget import layer_widget
import os
from str2bool import str2bool
from tab_base import tab_base
from help import help_window

from PyQt5.QtWidgets import QWidget,QHBoxLayout,QSplitter
from PyQt5.QtCore import Qt

import i18n
_ = i18n.language.gettext

from gpvdm_viewer import gpvdm_viewer
from cal_path import get_sim_path
from server import server_get

class tab_view(QWidget,tab_base):

		
	def __init__(self):
		QWidget.__init__(self)

		hbox=QHBoxLayout(self)
		mainLayout = QSplitter(Qt.Horizontal)
		self.viewer=gpvdm_viewer(get_sim_path())
		self.viewer.allow_navigation=True
		self.viewer.set_directory_view(True)
		self.viewer.set_back_arrow(True)
		self.viewer.set_multi_select()
		
		hbox.addWidget(self.viewer)
		
		self.setLayout(hbox)
		server_get().sim_finished.connect(self.refresh)
		
	def refresh(self):
		self.viewer.fill_store()

	def help(self):
		help_window().help_set_help(["device.png",_("<big><b>The device structure tab</b></big>\n Use this tab to change the structure of the device, the layer thicknesses and to perform optical simulations.  You can also browse the materials data base and  edit the electrical mesh.")])

	def got_help(self,data):
		if data!="":
			help_window().help_append(["star.png",_("<big><b>Update available!</b></big><br>"+data)])
