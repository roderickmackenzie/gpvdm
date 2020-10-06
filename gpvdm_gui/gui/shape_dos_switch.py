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

## @package gtkswitch
#  Package to provide an equivlent to the gnome switch
#


import math
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter,QFont,QColor,QPen

from PyQt5.QtCore import pyqtSignal
from epitaxy import get_epi
from newton_solver import newton_solver_get_type

class shape_dos_switch(QWidget):

	changed = pyqtSignal()

	def __init__(self):      
		super(shape_dos_switch, self).__init__()
		self.setMaximumSize(200,20)
		self.value = "none"

	def get_value(self):
		return self.value

	def set_value(self, value):

		self.value = value


	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		self.drawWidget(qp)
		qp.end()


	def drawWidget(self, qp):
		font = QFont('Sans', 11, QFont.Normal)
		qp.setFont(font)

		pen = QPen(QColor(20, 20, 20), 1, Qt.SolidLine)
		
		qp.setPen(pen)
		qp.setBrush(Qt.NoBrush)

		if self.value=="none":
			qp.setBrush(QColor(180,180,180))
			qp.drawRoundedRect(0, 0.0, 140.0,22.0,5.0,5.0)			
			qp.setBrush(QColor(230,230,230))
			qp.drawRoundedRect(2, 2, 30,18.0,5.0,5.0)
			qp.drawText(40, 17, _("Passive"))
		else:
			qp.setBrush(QColor(95,163,235))
			qp.drawRoundedRect(0, 0.0, 140.0,22.0,5.0,5.0)			
			qp.setBrush(QColor(230,230,230))
			qp.drawRoundedRect(105, 2, 30,18.0,5.0,5.0)
			qp.drawText(2, 17, _("Active"))

	def mouseReleaseEvent(self, QMouseEvent):
		epi=get_epi()

		if QMouseEvent.x()<160:
			if self.value== "none":
				if newton_solver_get_type()=="newton_simple":
					self.value=epi.add_new_electrical_to_shape(self.shape_file)
				else:
					self.value=epi.add_new_dos_to_shape(self.shape_file)
				
			else:
				if newton_solver_get_type()=="newton_simple":
					self.value=epi.del_electrical_shape(self.shape_file)
					self.value="none"
				else:
					self.value=epi.del_dos_shape(self.shape_file)
					self.value="none"

			self.repaint()
			self.changed.emit()
