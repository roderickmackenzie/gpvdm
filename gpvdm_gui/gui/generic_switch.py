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

## @package generic_switch
#  A generic switch
#


import math
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter,QFont,QColor,QPen,QFontMetrics

from PyQt5.QtCore import pyqtSignal

class generic_switch(QWidget):

	changed = pyqtSignal()

	def __init__(self,state0="True",state1="False",state0_value=True,state1_value=False):      
		super(generic_switch, self).__init__()
		self.state0=state0
		self.state1=state1
		self.state0_value=state0_value
		self.state1_value=state1_value

		self.font = QFont('Sans', 11, QFont.Normal)
		fm = QFontMetrics(self.font)
		self.w0 = fm.width(self.state0)
		self.w1 = fm.width(self.state1)
		if self.w0>self.w1:
			self.my_width=self.w0
		else:
			self.my_width=self.w1

		#print(self.my_width)
		self.setMaximumSize(self.my_width+120,20)

		#self.setMinimumSize(1, 30)
		self.value = self.state0_value

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

		qp.setFont(self.font)

		pen = QPen(QColor(20, 20, 20), 1, Qt.SolidLine)
		
		qp.setPen(pen)
		qp.setBrush(Qt.NoBrush)

		if self.value==self.state0_value:
			qp.setBrush(QColor(95,163,235))
			qp.drawRoundedRect(0, 0.0, self.my_width+51,22.0,5.0,5.0)			
			qp.setBrush(QColor(230,230,230))
			qp.drawRoundedRect(self.my_width+15, 2, 30,18.0,5.0,5.0)
			qp.drawText(2, 17, self.state0)
		else:
			qp.setBrush(QColor(180,180,180))
			qp.drawRoundedRect(0, 0.0, self.my_width+51,22.0,5.0,5.0)			
			qp.setBrush(QColor(230,230,230))
			qp.drawRoundedRect(2, 2, 30,18.0,5.0,5.0)
			qp.drawText(40, 17, self.state1)

	def mouseReleaseEvent(self, QMouseEvent):
		if QMouseEvent.x()<160:
			if self.value== self.state0_value:
				self.value=self.state1_value
			else:
				self.value=self.state0_value

			self.repaint()
			self.changed.emit()
