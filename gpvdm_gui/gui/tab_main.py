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

## @package tab_main
#  The main tab in the gpvdm simulation window.
#

import math
from layer_widget import layer_widget
import os
from str2bool import str2bool
from tab_base import tab_base
from help import help_window
from display import display_widget
from win_lin import running_on_linux


from PyQt5.QtWidgets import QWidget,QHBoxLayout,QSplitter
from PyQt5.QtCore import Qt

from ribbon_device import ribbon_device

import i18n
_ = i18n.language.gettext

class tab_main(QWidget,tab_base):

	label_name="tab_main"

		
	def __init__(self):
		QWidget.__init__(self)
		self.sun=1
		hbox=QHBoxLayout(self)
		mainLayout = QSplitter(Qt.Horizontal)
		#splitter1 = QtGui.QSplitter(QtCore.Qt.Horizontal)
		self.three_d=display_widget()
		#self.three_d.show()

		self.ribbon=ribbon_device()
		self.ribbon.addWidget(self.three_d.display.toolbar0)
		self.ribbon.addWidget(self.three_d.display.toolbar1)

		mainLayout.addWidget(self.ribbon)
		self.ribbon.setMinimumSize(100, 0)
		mainLayout.addWidget(self.three_d)
		mainLayout.setStretchFactor ( 1, 1 )

		hbox.addWidget(mainLayout)
		
		self.setLayout(hbox)
		
	def help(self):
		help_window().help_set_help(["device.png",_("<big><b>The device structure tab</b></big>\n Use this tab to change the structure of the device, the layer thicknesses and to perform optical simulations.  You can also browse the materials data base and  edit the electrical mesh.")])

