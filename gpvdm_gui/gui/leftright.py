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

## @package leftright
#  A left right selector widget switch.
#


import math
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter,QFont,QColor,QPen

from PyQt5.QtCore import pyqtSignal

class leftright(QWidget):

	changed = pyqtSignal()
	
	def __init__(self):      
		super(leftright, self).__init__()
		self.setMaximumSize(100,30)
		self.initUI()

	def initUI(self):

		#self.setMinimumSize(1, 30)
		self.value = False


	def set_value(self, value):

		self.value = value

	def get_value(self):
		if self.value==True:
			return "right"
		else:
			return "left"
		
	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		self.drawWidget(qp)
		qp.end()


	def drawWidget(self, qp):
		font = QFont('Sans', 11, QFont.Normal)
		qp.setFont(font)

		size = self.size()
		w = size.width()
		h = size.height()

		#            qp.setPen(QColor(255, 255, 255))
		#           qp.setBrush(QColor(255, 255, 184))
		#          qp.drawRect(0, 0, till, h)


		pen = QPen(QColor(20, 20, 20), 1, Qt.SolidLine)
		
		qp.setPen(pen)
		qp.setBrush(Qt.NoBrush)

		if self.value==True:
			qp.setBrush(QColor(95,163,235))
			qp.drawRoundedRect(0, 0.0, 100.0,22.0,5.0,5.0)
			qp.setBrush(QColor(230,230,230))
			qp.drawRoundedRect(50, 2, 48,18.0,5.0,5.0)

			qp.drawText(8, 17, _("Right"))
		else:
			qp.setBrush(QColor(180,180,180))
			qp.drawRoundedRect(0, 0.0, 100.0,22.0,5.0,5.0)			
			qp.setBrush(QColor(230,230,230))
			qp.drawRoundedRect(2, 2, 48,18.0,5.0,5.0)
			qp.drawText(55, 17, _("Left"))

	def mouseReleaseEvent(self, QMouseEvent):
		if QMouseEvent.x()<100:
			self.value= not self.value
			self.repaint()
			self.changed.emit()



