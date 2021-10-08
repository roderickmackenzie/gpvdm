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

## @package spinner
#  A spinner widget.
#

import sys
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QIcon,QPalette
from PyQt5.QtGui import QPainter,QFont,QColor,QPen,QPainterPath,QBrush
from PyQt5.QtCore import Qt, QTimer

from math import pi,acos,sin,cos,atan2
import random

class spinner(QWidget):
    
	def __init__(self):
		super().__init__()
		self.delta=0
		self.setFixedSize(30, 30)
		#self.setGeometry(300, 300, 300, 220)
		#self.setWindowTitle('Icon')
		#self.setWindowIcon(QIcon('web.png'))        
		#self.start()
		self.timer=None

	def start(self):
		if self.timer==None:
			self.timer=QTimer()
			self.timer.timeout.connect(self.update)
			self.timer.start(10)
			self.blue_target=255.0
			self.green_target=0.0
			self.red_target=0.0
		
	def stop(self):
		if self.timer!=None:
			self.timer.stop()
		self.timer=None

	def update(self):
		self.delta=self.delta+5
		if self.delta>360:
			self.delta=0

		self.blue_target=0#self.blue_target+20*random.random()-10
		self.green_target=0#self.green_target+20*random.random()-10
		self.red_target=0#self.red_target+20*random.random()-10
		if self.blue_target>255:
			self.blue_target=255

		if self.red_target>255:
			self.red_target=255

		if self.green_target>255:
			self.green_target=255

		if self.blue_target<0:
			self.blue_target=0

		if self.red_target<0:
			self.red_target=0

		if self.green_target<0:
			self.green_target=0
			
		self.repaint()
		
	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		self.drawWidget(qp)
		qp.end()
		
	def drawWidget(self, qp):
		color = self.palette().color(QPalette.Background)
		qp.setBrush(QColor(100,0,0))

		pen=QPen()
		pen.setWidth(self.width()/10)
		
		pen.setColor(QColor(0,0,255))
		pen.setCapStyle(Qt.RoundCap)

		w=self.width()/2
		x_shift=w+w*0.05
		y_shift=w+w*0.05
		r=0.35*w
		r1=w*0.8
		qp.setPen(pen)

		my_max=100
		p=[]
		c=[]
		for phi in range(0,360,30):
			p.append(phi)
			c.append(0)
		f=0
		for i in range(0,len(p)):
			if p[i]>self.delta:
				f=i
				break
		i=f
		m=1.0
		while(i>=0):
			c[i]=m
			m=m*0.7
			i=i-1
			
		i=len(c)-1
		
		while(i>f):
			c[i]=m
			m=m*0.7
			i=i-1

		for i in range(0,len(p)):
			self.pos=p[i]
			x = r *  cos( (2*pi)*self.pos/360 )
			y = r *  sin( (2*pi)*self.pos/360 )
		
			x1 = r1 *  cos( (2*pi)*self.pos/360 )
			y1 = r1 *  sin( (2*pi)*self.pos/360 )
			cb=self.blue_target*c[i]+color.blue()*(1.0-c[i])
			cg=self.green_target*c[i]+color.green()*(1.0-c[i])
			cr=self.red_target*c[i]+color.red()*(1.0-c[i])
			
			pen.setColor(QColor(cr,cg,cb))
			qp.setPen(pen)
			qp.drawLine(x+x_shift,y+y_shift,x1+x_shift,y1+y_shift)
        
