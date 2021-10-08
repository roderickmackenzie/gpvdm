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

## @package radiobutton
#  A radiobuttion - needs improving.
#


import math
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QLinearGradient,QPainter,QFont,QColor,QPen

from PyQt5.QtCore import pyqtSignal

class radiobutton(QWidget):

	changed = pyqtSignal()

	def __init__(self):      
		super(radiobutton, self).__init__()
		self.setMinimumSize(85,30)

		self.value = False

	def get_value(self):
		if self.value==True:
			return "true"
		else:
			return "false"

	def set_value(self, value):

		self.value = value
		self.repaint()

	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		qp.setRenderHint(QPainter.Antialiasing)
		self.drawWidget(qp)
		qp.end()


	def drawWidget(self, qp):
		font = QFont('Sans', 11, QFont.Normal)
		qp.setFont(font)

		pen = QPen(QColor(20, 20, 20), 1, Qt.SolidLine)
		
		qp.setPen(pen)
		qp.setBrush(Qt.NoBrush)

		if self.value==True:
			qp.setBrush(QColor(72,72,72))
			qp.drawEllipse(0, 0, 45,45)

			qp.setBrush(QColor(140,140,140))
			qp.drawEllipse(2, 2, 41,41)

			qp.setBrush(QColor(43,69,201))
			qp.drawEllipse(7, 7, 31,31)

			qp.setBrush(QColor(70,140,200))
			qp.drawEllipse(9, 9, 27,27)


			qp.drawText(1, 60, _("Enabled"))
		else:
			qp.setBrush(QColor(72,72,72))
			qp.drawEllipse(0, 0, 45,45)

			qp.setBrush(QColor(140,140,140))
			qp.drawEllipse(2, 2, 41,41)
			qp.drawText(1, 60, _("Disabled"))

	def mouseReleaseEvent(self, QMouseEvent):
		if QMouseEvent.x()<80:
			self.value= True
			self.repaint()
			self.changed.emit()
