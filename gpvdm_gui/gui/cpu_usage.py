# -*- coding: utf-8 -*-
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

## @package cpu_usage
#  A CPU usage widget, which displays local CPU usage, local disk load, and remote CPU usage. 
#

import sys
from PyQt5.QtWidgets import QApplication, QWidget

import sys
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QIcon,QPalette
from PyQt5.QtGui import QPainter,QFont,QColor,QPen,QPainterPath,QBrush
from PyQt5.QtCore import Qt, QTimer

from psutil import cpu_percent
from psutil import cpu_times
from psutil import disk_io_counters
import random
from server import server_get

class store():
	load=0.0
	wait=0.0
	cluster=0.0
	color=[0,0,0]

class cpu_usage(QWidget):
    
	def __init__(self):
		super().__init__()
		self.server=server_get()
		self.load=[]

		self.wait_last=0.0
		self.setMinimumSize(40, 40)

		for i in range(0,1000):
			s=store()
			self.load.append(s)

		self.delta=0    
		self.start()
		
	def start(self):
		self.timer=QTimer()
		self.timer.timeout.connect(self.update)
		self.timer.start(100);

	def stop(self):
		self.timer.stop()
		
	def update(self):
		a=0
		tot=0
		s=store()
		s.load=cpu_percent()
		self.load.append(s)
		#for i in range(len(self.load)-1,len(self.load)):
		#	a=a+self.load[i]
		#	tot=tot+1.0
		#a=a/tot
		try:		#user reported bug, This is a problem with the underlying function.
			w_temp=disk_io_counters()[3]/1000
		except:
			w_temp=0

		w_delta=w_temp-self.wait_last
		self.wait_last=w_temp
		
		
		s.wait=int(w_delta)
		s.color=[255*(s.load/100.0),0,0]
		s.cluster=self.server.get_nodes_load()
		self.load[len(self.load)-1]=s
		self.load.pop(0)


		self.repaint()
		
	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		self.drawWidget(qp)
		qp.end()
		
	def drawWidget(self, qp):
		h=self.height()
		w=self.width()
		qp = QPainter()
		qp.begin(self)

		qp.setBrush(QColor(0,0,0))
		qp.setPen(QColor(0,0,0))
		qp.drawRect(0, 0, w, h)
		

		dy=h/len(self.load)

		for i in range(0,len(self.load)):
			#cpu
			qp.setBrush(QColor(self.load[i].color[0],self.load[i].color[1],self.load[i].color[2]))
			qp.setPen(QColor(self.load[i].color[0],self.load[i].color[1],self.load[i].color[2]))
		
			dx=self.load[i].load*w/100.0
			qp.drawRect(w, h-dy*i, -dx, dy)

			qp.setBrush(QColor(0,100,0))
			qp.setPen(QColor(0,100,0))

			dx=self.load[i].cluster*w/100.0
			qp.drawRect(w, h-dy*i, -dx, dy)

			qp.setBrush(QColor(0,0,255))
			qp.setPen(QColor(0,0,255))

			dx=self.load[i].wait*w/100.0
			if dx<=w:
				qp.drawRect(w, h-dy*i, -dx, dy)
			
			



