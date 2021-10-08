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

## @package circuit
#  Widget to draw circuit diagram
#

import os
from cal_path import get_image_file_path
from tb_pulse_load_type import tb_pulse_load_type

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QHBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar
from PyQt5.QtGui import QPainter,QIcon,QPixmap
from ersatzschaltbild import ersatzschaltbild
from icon_lib import icon_get
import functools
from gui_util import yes_no_dlg
from inp import inp
from epitaxy import get_epi
from cal_path import get_sim_path
from json_circuit import json_component
from gpvdm_json import gpvdm_data

class tool_item(QAction):
	def __init__(self,icon_name,name,s):
		self.icon=icon_name
		self.name=name
		QAction.__init__(self,icon_get(icon_name), name,s)
		self.setCheckable(True)

class circuit_editor(QWidget):

	def __init__(self):
		QWidget.__init__(self)

		vbox=QHBoxLayout()

		self.toolbar=QToolBar()
		self.toolbar.setOrientation(Qt.Vertical)
		self.toolbar.setIconSize(QSize(48, 48))

		self.buttons=[]
		self.buttons.append(tool_item("resistor", _("Resistor"),self))
		#self.buttons.append(tool_item("capacitor", _("Capacitor"),self))
		self.buttons.append(tool_item("diode", _("Diode"),self))
		self.buttons.append(tool_item("wire", _("Wire"),self))
		self.buttons.append(tool_item("ground", _("Ground"),self))
		self.buttons.append(tool_item("bat", _("Voltage source"),self))
		self.buttons.append("sep")
		self.buttons.append(tool_item("pointer", _("Pointer"),self))
		self.buttons.append(tool_item("clean", _("Clean"),self))

		self.buttons[1].setChecked(True)

		for item in self.buttons:
			if item=="sep":
				self.toolbar.addSeparator()
			else:
				self.toolbar.addAction(item)
				item.triggered.connect(functools.partial(self.callback_click,item))

		vbox.addWidget(self.toolbar)

		self.ersatzschaltbild = ersatzschaltbild()
		self.ersatzschaltbild.dx=80
		self.ersatzschaltbild.dy=80
		self.ersatzschaltbild.init()

		vbox.addWidget(self.ersatzschaltbild)

		self.setLayout(vbox)
		#self.setMinimumSize(800, 800)
		self.build_from_epi()

		gpvdm_data().add_call_back(self.ersatzschaltbild.load)
		self.destroyed.connect(self.doSomeDestruction)

	def doSomeDestruction(self):
		gpvdm_data().remove_call_back(self.ersatzschaltbild.load)

	def build_from_epi(self):
			data=gpvdm_data()
			if data.circuit.enabled==False:
				epi=get_epi()
				pos=3

				a=json_component()
				a.x0=pos
				a.y0=3
				a.x1=pos+1
				a.y1=3
				a.name="bat"
				self.ersatzschaltbild.add_object(a)

				pos=pos+1
				
				for l in epi.layers:
					if l.layer_type=="active":# or l.layer_type=="contact":
						component=l.shape_electrical.electrical_component
						if component=="resistance":
							a=json_component()
							a.x0=pos
							a.y0=3
							a.x1=pos+1
							a.y1=3
							a.name="resistor"
							self.ersatzschaltbild.add_object(a)
						if component=="diode":
							a=json_component()
							a.x0=pos
							a.y0=3
							a.x1=pos+1
							a.y1=3
							a.name="diode"
							self.ersatzschaltbild.add_object(a)
						pos=pos+1

				a=json_component()
				a.x0=pos
				a.y0=3
				a.x1=pos+1
				a.y1=3
				a.name="ground"
				self.ersatzschaltbild.add_object(a)

				self.ersatzschaltbild.objects_push()
			else:
				self.ersatzschaltbild.load()

	def callback_click(self,widget):
		for item in self.buttons:
			if item!="sep":
				item.setChecked(False)

		widget.setChecked(True)

		self.ersatzschaltbild.selected=widget.icon

