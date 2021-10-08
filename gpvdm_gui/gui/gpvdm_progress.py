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

## @package gpvdm_progress
#  Progress Widget
#


from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication, QPushButton
from PyQt5.QtGui import QPainter,QColor,QPainterPath,QPolygonF
from PyQt5.QtWidgets import QWidget, QHBoxLayout
from PyQt5.QtCore import QRectF,QPoint
from PyQt5.QtCore import QTimer

class gpvdm_progress(QWidget):
	def update(self):
		self.setValue()
		self.repaint()
		
	def __init__(self):
		QWidget.__init__(self)
		self.setMaximumHeight(40)
		self.pulse_direction=True

		self.value=0.0
		self.enable_pulse=False


	def enablePulse(self,value):
		self.enable_pulse=value

	def setValue(self,v):
		self.enable_pulse=False
		self.value=v
		self.repaint()

	def pulse(self):
		self.enable_pulse=True
		
		if self.pulse_direction==True:
			self.value=self.value+0.1
		else:
			self.value=self.value-0.1

		if self.value>=1.0 or self.value<=0.0:
			self.pulse_direction= not self.pulse_direction
			if self.value>0.5:
				self.value=1.0

			if self.value<0.5:
				self.value=0.0
				
		self.repaint()

	def paintEvent(self, e):
		w=self.width()
		h=self.height()
		
		if self.enable_pulse==True:
			l=(w-3)*0.2
			bar_start=((w-3)-l)*self.value
		else:
			l=(w-3)*self.value
			bar_start=0
		
		qp = QPainter()
		qp.begin(self)

		color = QColor(0, 0, 0)
		color.setNamedColor('#d4d4d4')
		qp.setBrush(color)
		
		path=QPainterPath()
		path.addRoundedRect(QRectF(0, 0, w, h), 0, 0)
		qp.fillPath(path,QColor(206 , 206, 206));


		path=QPainterPath()
		path.addRoundedRect(QRectF(bar_start, 3, l, h-6), 5, 5)
		qp.fillPath(path,QColor(71 , 142, 216));

		
		#path.addPolygon(QPolygonF([QPoint(0,0), QPoint(10,0), QPoint(10,10), QPoint(10,0)]));
		#qp.setBrush(QColor(71 , 142, 216))

		#path.addPolygon(QPolygonF([QPoint(0,0), QPoint(50,0), QPoint(40,10), QPoint(10,0)]))
		#qp.setBrush(QColor(71 , 142, 216))
		#qp.setBrush(QColor(206 , 207, 206))




		qp.end()
