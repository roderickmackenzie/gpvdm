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


## @package gl_fallback
#  If OpenGL does not work fall back to this.
#

import math
import os
from epitaxy import epitaxy_get_name

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit,QLabel
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QPainter,QFont,QColor,QPen,QPainterPath,QBrush

import numpy as np
from math import pi,acos,sin,cos

from dat_file import dat_file
from global_objects import global_object_register
from cal_path import get_sim_path
from gl_base_widget import gl_base_widget
from epitaxy import get_epi

from gpvdm_json import gpvdm_data

class gl_fallback(QWidget,gl_base_widget):

	def __init__(self):
		QWidget.__init__(self)
		gl_base_widget.__init__(self)
		#self.setMinimumSize(600, 500)
		self.epi=get_epi()
		global_object_register("gl_force_redraw",self.force_redraw)
		
	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		self.drawWidget(qp)
		qp.end()


	def drawWidget(self, qp):
		data=gpvdm_data()
		font = QFont('Sans', 11, QFont.Normal)
		qp.setFont(font)
		epi=data.epi
		emission=False

		for l in epi.layers:
			if l.layer_type=="active":
				if l.shape_pl.pl_emission_enabled==True:
					emission=True

		tot=epi.ylen()

		pos=0.0
		total_layers=len(epi.layers)
		lines=[]

		for i in range(0,total_layers):
			thick=200.0*epi.layers[total_layers-1-i].dy/tot
			pos=pos+thick
			l=epi.layers[i]
			red=l.color_r
			green=l.color_g
			blue=l.color_b

			self.draw_box(qp,200,450.0-pos,thick*0.9,red,green,blue,total_layers-1-i)
		step=50.0

		self.suns=float(data.light.Psun)

		if self.suns<=0.01:
			step=200
		elif self.suns<=0.1:
			step=100
		elif self.suns<=1.0:
			step=50
		elif self.suns<=10.0:
			step=10
		else:
			step=5.0
		if self.suns!=0:
			for x in range(0,200,step):
				self.draw_photon(qp,210+x,100,False)

		if emission==True:
			for x in range(0,200,50):
				self.draw_photon(qp,240+x,140,True)

		self.draw_mode(qp,200,250)
		y=500
		qp.drawText(40, y+40, "You do not have working 3D graphics hardware acceleration on your PC.")
		qp.drawText(40, y+70, "If your PC is modern it is likely that you don't have the correct drivers installed")
		qp.drawText(40, y+100, "Gpvdm is using 2D fallback mode - the model will still work")
		qp.drawText(40, y+130, "It will just not look as pretty.")



	def draw_photon(self,qp,start_x,start_y,up):
		wx=np.arange(0, 100.0 , 0.1)
		wy=np.sin(wx*3.14159*0.1)*15

		pen=QPen()
		pen.setWidth(2)
		
		if up==True:
			pen.setColor(QColor(0,0,255))
		else:
			pen.setColor(QColor(0,255,0))

		qp.setPen(pen)

		for i in range(1,len(wx)):
			qp.drawLine((int)(start_x-wy[i-1]),(int)(start_y+wx[i-1]),(int)(start_x-wy[i]),(int)(start_y+wx[i]))

		if up==True:
			path=QPainterPath()

			path.moveTo (start_x-10, start_y);
			path.lineTo (start_x-10, start_y);

			path.lineTo (start_x+10,   start_y);

			path.lineTo (start_x, start_y-20);
			
			qp.fillPath (path, QBrush (QColor (0,0,255)))
		else:
			path=QPainterPath()

			path.moveTo (start_x-10, start_y+100.0);
			path.lineTo (start_x-10, start_y+100.0);

			path.lineTo (start_x+10,   start_y+100.0);

			path.lineTo (start_x, start_y+100.0+20);

			qp.setPen (Qt.NoPen);
			qp.fillPath (path, QBrush (QColor (0,255,0)))


	def draw_box(self,qp,x,y,h,r,g,b,layer):

		text=""
		w=200
		qp.setBrush(QColor(r*255,g*255,b*255))
		qp.drawRect(x, y, 200,h)
		data=gpvdm_data()
		epi=data.epi
		if epi.layers[layer].layer_type=="active":
			text=epitaxy_get_name(layer)+" (active)"
			qp.setBrush(QColor(0,0,0.7*255))
			qp.drawRect(x+w+5, y, 20,h)
		else:
			text=epitaxy_get_name(layer)

		qp.drawText(x+200+40, y+h/2, text)
		
		return

	def draw_mode(self,qp,start_x,start_y):
		t=[]
		s=[]
		z=[]
		x=start_x
		y=start_y
		pen=QPen()
		pen.setWidth(3)
		pen.setColor(QColor(0,0,0))

		qp.setPen(pen)
		data=dat_file()
		if data.load(os.path.join(get_sim_path(),"optical_output","light_1d_photons_tot_norm.dat"))==True:
			for i in range(1,data.y_len):
				
				x0=(start_x-data.data[0][0][i-1]*40-10)
				y0=(start_y+(200*(i-1)/data.y_len))
				x1=(start_x-data.data[0][0][i]*40-10)
				y1=(start_y+(200*i/data.y_len))
				if math.isnan(x0)==False and math.isnan(y0)==False :
					x0=(int)(x0)
					y0=(int)(y0)
					x1=(int)(x1)
					y1=(int)(y1)
					qp.drawLine(x0,y0,x1,y1)
		#else:
		#	print("no mode")


	def set_sun(self,suns):
		self.suns=suns

	def force_redraw(self):
		self.repaint()
