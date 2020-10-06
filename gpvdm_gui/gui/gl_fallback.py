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


## @package gl_fallback
#  If OpenGL does not work fall back to this.
#

import math
import os
from cal_path import get_materials_path
from inp import inp_load_file
from inp import inp_search_token_value
from str2bool import str2bool
from tab_base import tab_base
from epitaxy import epitaxy_get_layers
from epitaxy import epitaxy_get_dy
from epitaxy import epitaxy_get_dos_file
from help import my_help_class
from epitaxy import epitaxy_get_pl_file
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
from inp import inp_search_token_array
from cal_path import get_sim_path
from gl_base_widget import gl_base_widget
from epitaxy import get_epi

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

		font = QFont('Sans', 11, QFont.Normal)
		qp.setFont(font)

		emission=False
		lines=[]
		for i in range(0,epitaxy_get_layers()):
			if epitaxy_get_pl_file(i)!="none":
				lines=inp_load_file(epitaxy_get_pl_file(i)+".inp")
				if lines!=False:
					if str2bool(lines[1])==True:
						emission=True

		tot=0
		for i in range(0,epitaxy_get_layers()):
			tot=tot+epitaxy_get_dy(i)

		pos=0.0
		l=epitaxy_get_layers()-1
		lines=[]

		for i in range(0,len(self.epi.layers)):
			
			red=0.0
			green=0.0
			blue=0.0

			thick=200.0*epitaxy_get_dy(l-i)/tot
			pos=pos+thick
			path=os.path.join(get_materials_path(),self.epi.layers[l-i].optical_material,"mat.inp")
			lines=inp_load_file(path)
			if lines!=False:
				ret=inp_search_token_array(lines, "#red_green_blue")
				if ret!=False:
					red=float(ret[0])
					green=float(ret[1])
					blue=float(ret[2])

			self.draw_box(qp,200,450.0-pos,thick*0.9,red,green,blue,l-i)
		step=50.0

		lines=inp_load_file(os.path.join(get_sim_path(),"light.inp"))

		if lines!=False:
			self.suns=float(inp_search_token_value(lines, "#Psun"))

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
		qp.drawText(40, 540+40, "No OpenGL support, using 2D fallback mode")


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
		
		if epitaxy_get_dos_file(layer).startswith("dos")==True:
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
