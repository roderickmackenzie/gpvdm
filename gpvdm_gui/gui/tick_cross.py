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

## @package play
#  A play button
#

import os

#qt
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QAction

from icon_lib import icon_get
from help import help_window

from util import wrap_text


class tick_cross(QAction):

	changed = pyqtSignal()

	def setState(self,state):

		self.enabled=state
		if self.enabled==True:
			self.setIcon(icon_get("tick"))
			self.setText(self.enable_text)
		else:
			self.setIcon(icon_get("cross"))
			self.setText(self.disable_text)
		

	def do_emit(self):
		self.setState(not self.enabled)
		self.changed.emit()

	def __init__(self,enable_text=_("Enabled"),disable_text=_("Disabled")):
		QAction.__init__(self)
		self.enabled=True
		self.enable_text=enable_text
		self.disable_text=disable_text

		self.setState(self.enabled)

		self.triggered.connect(self.do_emit)



