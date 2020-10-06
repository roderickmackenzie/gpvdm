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

## @package circuit
#  Widget to draw circuit diagram
#

import os
from cal_path import get_image_file_path
from tb_pulse_load_type import tb_pulse_load_type

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar
from PyQt5.QtGui import QPainter,QIcon,QPixmap
from ersatzschaltbild import ersatzschaltbild
from icon_lib import icon_get
import functools
from gui_util import yes_no_dlg
from inp import inp

class tool_item():
	def __init__(self,icon,name):
		self.icon=icon
		self.name=name

class circuit_editor(QWidget):

	def __init__(self):
		QWidget.__init__(self)

		vbox=QVBoxLayout()

		self.toolbar=QToolBar()
		self.toolbar.setIconSize(QSize(48, 48))

		items=[]
		items.append(tool_item("resistor", _("Resistor")))
		items.append(tool_item("capacitor", _("Capacitor")))
		items.append(tool_item("diode", _("Diode")))
		items.append(tool_item("wire", _("Wire")))
		items.append(tool_item("ground", _("Ground")))
		items.append("sep")
		items.append(tool_item("pointer", _("Pointer")))
		items.append(tool_item("clean", _("Clean")))
		items.append(tool_item("edit-undo", _("Restore to origonal")))


		for item in items:
			if item=="sep":
				self.toolbar.addSeparator()
			else:
				tool=QAction(icon_get(item.icon), item.name, self)
				self.toolbar.addAction(tool)
				tool.triggered.connect(functools.partial(self.callback_click,item.icon))

		#self.cap=QAction(icon_get("capacitor"), _("Capacitor"), self)
		#self.toolbar.addAction(self.cap)

		#self.wire=QAction(icon_get("wire"), _("Wire"), self)
		#self.toolbar.addAction(self.wire)

		#self.diode=QAction(icon_get("diode"), _("Diode"), self)
		#self.toolbar.addAction(self.diode)

		#self.pointer=QAction(icon_get("pointer"), _("Pointer"), self)
		#self.toolbar.addAction(self.pointer)

		#self.ground=QAction(icon_get("ground"), _("Ground"), self)
		#self.toolbar.addAction(self.ground)

		vbox.addWidget(self.toolbar)

		self.ersatzschaltbild = ersatzschaltbild()
		self.ersatzschaltbild.dx=80
		self.ersatzschaltbild.dy=80
		self.ersatzschaltbild.init()

		vbox.addWidget(self.ersatzschaltbild)

		self.setLayout(vbox)
		#self.setMinimumSize(800, 800)

	def callback_click(self,component):
		if component=="edit-undo":
			result=yes_no_dlg(self,_("Do you really which to reset the circuit to the default?"))
			if result == True:
				f=inp()
				f.set_file_name("diagram.inp")
				f.delete()
				self.ersatzschaltbild.objects_pop()
				self.ersatzschaltbild.repaint()
			return

		self.ersatzschaltbild.selected=component

