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

## @package band_graph
#  Band diagram graph
#


import os
import io
import sys
from numpy import *

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

import zipfile
from dat_file import dat_file


from matplotlib.figure import Figure

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QActionGroup,QTabWidget,QMenu,QApplication
from PyQt5.QtGui import QIcon,QPixmap,QImage, QScreen, QBrush, QColor
from PyQt5.QtGui import QPainter,QFont,QColor,QPen,QFontMetrics,QPainterPath

#calpath
from icon_lib import icon_get

from open_save_dlg import save_as_filter
from cal_path import get_sim_path
from cal_path import get_default_material_path

from lock import get_lock

from epitaxy import get_epi
from PyQt5.QtCore import pyqtSignal
from json_material_db_item import json_material_db_item

from util import wavelength_to_rgb
from color_map import color_map
from math import log10
from util import time_with_units

svgwrite_enabled=False
try:
	import svgwrite
	svgwrite_enabled=True
except:
	pass



class gpvdm_graph(QWidget):
	changed = pyqtSignal()

	def __init__(self):
		QWidget.__init__(self)
		global svgwrite_enabled
		self.show_labels=True
		self.show_energies=True
		self.show_data=True
		self.show_axis=True
		self.normalize_x_axis=False
		self.active_layer_from_dos=True
		self.use_epi_for_x=False
		self.y_label=None
		self.x_label=None
		self.font = QFont('Sans', 11, QFont.Normal)
		fm = QFontMetrics(self.font)

		self.build_menu()
		self.data=dat_file()
		self.E_max=None
		self.E_min=None
		self.menu_copy.triggered.connect(self.do_clip)
		if svgwrite_enabled==True:
			self.menu_save_as_svg.triggered.connect(self.callback_save_as_svg)
		self.data_x0=None
		self.data_x1=None
		self.x0_mul=0.1
		self.enable_wavelength_to_rgb=False
		self.colors=color_map()
		self.colors_free=color_map(map_name="blues")
		self.show_title=False

	def callback_save_as_svg(self):
		response=save_as_filter(self,"svg (*.svg)")
		if response != None:
			dwg = svgwrite.Drawing(response, profile='tiny',size=(str(self.width())+'mm', str(self.height())+'mm'), viewBox=('0 0 '+str(self.width())+" "+str(self.height())))
			self.render_image(dwg,svg=True)
			dwg.save()

	def save_image(self):
		response=save_as_filter(self,"png (*.png);;jpg (*.jpg);;svg (*.svg)")
		if response != None:
			#print(response)
			self.savefig(response)

	def do_clip(self):
		screen = QApplication.primaryScreen()
		screenshot = screen.grabWindow( self.winId() )
		#p.save(filename, 'jpg')
		QApplication.clipboard().setImage(screenshot.toImage())

	def set_data_file(self,file):
		self.optical_mode_file=os.path.join(get_sim_path(),"optical_output",file)
		self.load_data(self.optical_mode_file)

	def build_menu(self):
		global svgwrite_enabled
		self.menu = QMenu(self)

		self.menu_show_labels=self.menu.addAction(_("Show labels"))
		self.menu_show_labels.triggered.connect(self.menu_toggle)
		self.menu_show_labels.setCheckable(True)
		self.menu_show_labels.setChecked(True)

		self.menu_show_data=self.menu.addAction(_("Show data"))
		self.menu_show_data.triggered.connect(self.menu_toggle)
		self.menu_show_data.setCheckable(True)
		self.menu_show_data.setChecked(True)

		self.menu_show_axis=self.menu.addAction(_("Show axis"))
		self.menu_show_axis.triggered.connect(self.menu_toggle)
		self.menu_show_axis.setCheckable(True)
		self.menu_show_axis.setChecked(True)

		self.menu_normalize_x_axis=self.menu.addAction(_("Normalize layer width"))
		self.menu_normalize_x_axis.triggered.connect(self.menu_toggle)
		self.menu_normalize_x_axis.setCheckable(True)
		self.menu_normalize_x_axis.setChecked(False)

		self.menu_copy=self.menu.addAction(_("Copy to clipboard"))
		if svgwrite_enabled==True:
			self.menu_save_as_svg=self.menu.addAction(_("Save as SVG"))
		self.menu_redraw=self.menu.addAction(_("Redraw"))
		self.menu_redraw.triggered.connect(self.menu_toggle)

		menu_ag= QMenu(_("Active layer"), self)
		ag = QActionGroup(menu_ag, exclusive=True)

		first=True
		self.ag_items=[_("From electrical parameters"),_("From material db")]
		for t in self.ag_items:
			action = QAction(t, menu_ag, checkable=True, checked=first)
			menu_ag.addAction(action)
			ag.addAction(action)
			first=False
		ag.triggered.connect(self.callback_ag)
		self.menu.addMenu(menu_ag)

	def callback_ag(self,action):
		self.active_layer_from_dos=(self.ag_items.index(action.text())==0)
		self.build_bands()
		self.changed.emit()

	def menu_toggle(self):
		action = self.sender()
		text=action.text()
		self.show_labels=self.menu_show_labels.isChecked()
		self.show_energies=self.menu_show_energies.isChecked()
		self.show_data=self.menu_show_data.isChecked()
		self.show_axis=self.menu_show_axis.isChecked()
		self.normalize_x_axis=self.menu_normalize_x_axis.isChecked()
		
		if text==_("Redraw"):
			self.build_bands()
			
		self.build_bands()

		self.changed.emit()

	def mouseReleaseEvent(self,event):
		self.menu.exec_(event.globalPos())

	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		qp.setRenderHints(QPainter.Antialiasing)
		self.drawWidget(qp)
		qp.end()


	def to_screen_x(self,x):
		return self.x0+(self.x1-self.x0)*(x-self.data_x0)/(self.data_x1-self.data_x0)

	def to_screen_y(self,y):
		div=self.data.data_max-self.data.data_min
		if div==0.0:
			return 0.0

		return self.y1-(self.y1-self.y0)*(y-self.data.data_min)/div

	def to_screen_y2(self,y):
		div=self.E_max-self.E_min
		if div==0.0:
			return 0.0

		return self.y1-(self.y1-self.y0)*(y-self.E_min)/div

	def txt_offset(self,text):
		return len(text)*4


	def draw_x_scale(self,qp):
		if self.data_x0==None:
			return

		xpoints=5.0
		xlen=self.data_x1-self.data_x0
		if xlen<100e-9:
			dx=10e-9
		elif xlen<1e-6:
			dx=100e-9
		else:
			dx=xlen/xpoints

		x=self.data_x0+dx/2.0
		while (x<self.data_x1):
			qp.drawLine(self.to_screen_x(x), self.y1, self.to_screen_x(x), self.y1+self.y1*0.02)
			qp.drawText(self.to_screen_x(x), self.y1+self.y1*0.1, str(int(x*1e9)))
			x=x+dx
		if self.x_label==None:
			txt=_("Position")
		else:
			txt=self.x_label
		
		txt=txt+" (nm)"

		qp.drawText(self.x0+(self.x1-self.x0)/2.0-self.txt_offset(txt), self.y1 +self.y1*0.17 , txt )

	def draw_y_scale(self,qp):
		if self.data.valid_data==False:
			return
		ypoints=5.0
	
		dy=(self.data.data_max-self.data.data_min)/ypoints

		y=self.data.data_min+dy/2
		while (y<self.data.data_max):
			qp.drawLine(self.x0, self.to_screen_y(y), self.x0*0.9, self.to_screen_y(y))
			qp.drawText(self.x0*0.3, self.to_screen_y(y), "{:.1e}".format(y).replace("+",""))
			y=y+dy

		qp.save()
		qp.translate( self.x0*0.2, self.y0 +(self.y1-self.y0)/2)
		qp.rotate(-90)
		if self.y_label==None:
			txt=self.data.data_label
		else:
			txt=self.y_label
		txt=txt+" ("+self.data.data_units+")"
		qp.drawText( -self.txt_offset(txt) , 0 , txt)
		#qp.rotate(-90)
		qp.restore()

	def draw_y2_scale(self,qp):
		if self.E_max==None:
			return

		ypoints=5.0
	
		dy=(self.E_max-self.E_min)/ypoints

		y=self.E_min+dy/2
		while (y<self.E_max):
			qp.drawLine(self.x1, self.to_screen_y2(y), self.x1*1.01, self.to_screen_y2(y))
			qp.drawText(self.x1*1.015, self.to_screen_y2(y), "{:.1f}".format(y).replace("+",""))
			y=y+dy

		qp.save()
		qp.translate( self.x1*1.07, self.y0 +(self.y1-self.y0)/2)
		qp.rotate(-90)
		txt=_("Energy")+" (eV)"
		qp.drawText( -self.txt_offset(txt) , 0 , txt)
		#qp.rotate(-90)
		qp.restore()

	def draw_graph_lines(self,qp):
		path = QPainterPath()
		path.moveTo(self.to_screen_x(self.data.y_scale[0]),self.to_screen_y(self.data.data[0][0][0]))
		for i in range(1,len(self.data.y_scale)):
			x0=self.to_screen_x(self.data.y_scale[i])
			y0=self.to_screen_y(self.data.data[0][0][i])
			#if self.enable_wavelength_to_rgb==True:
			#	r,g,b=wavelength_to_rgb(self.data.y_scale[i]*1e9)
			#	print(r,g,b)
			path.lineTo(x0, y0)

		pen = QPen(QColor(10, 10, 10,127), 4, Qt.SolidLine)
		qp.setPen(pen)
		qp.drawPath(path)
		pen = QPen(QColor(0, 0, 0), 1, Qt.SolidLine)
		qp.setPen(pen)

	def draw_trap_map(self,qp):

		qp.setPen(Qt.NoPen)

		for y in range(0,self.data.y_len):
			if y<self.data.y_len-1:
				dy=self.data.y_scale[y+1]-self.data.y_scale[y]
			else:
				dy=self.data.y_scale[y]-self.data.y_scale[y-1]

			#Ec
			x0=self.to_screen_x(self.data.y_scale[y])
			x1=self.to_screen_x(self.data.y_scale[y]+dy)

			y0=self.to_screen_y(self.data.Ec_f[0][0][y])
			y1=y0+10

			pos=int(255*(log10(self.data.nf[0][0][y])-self.data.trap_min)/(self.data.trap_max-self.data.trap_min))

			br = QBrush(QColor(self.colors.map[pos][0], self.colors.map[pos][1], self.colors.map[pos][2]))  
			qp.setBrush(br)
			
			qp.drawRect(x0, y0, 1+x1-x0, -1+y1-y0)

			#Ev
			x0=self.to_screen_x(self.data.y_scale[y])
			x1=self.to_screen_x(self.data.y_scale[y]+dy)

			y0=self.to_screen_y(self.data.Ev_f[0][0][y])-3
			y1=y0+10

			pos=int(255*(log10(self.data.pf[0][0][y])-self.data.trap_min)/(self.data.trap_max-self.data.trap_min))

			br = QBrush(QColor(self.colors.map[pos][0], self.colors.map[pos][1], self.colors.map[pos][2]))  
			qp.setBrush(br)
			
			qp.drawRect(x0, y0, 1+x1-x0, -1+y1-y0)
			for band in range(0,self.data.srh_bands):
				x0=self.to_screen_x(self.data.y_scale[y])
				x1=self.to_screen_x(self.data.y_scale[y]+dy)

				if band<self.data.srh_bands-1:
					dE=self.data.Ec[0][0][y][band+1]-self.data.Ec[0][0][y][band]
				else:
					dE=self.data.Ec[0][0][y][band]-self.data.Ec[0][0][y][band-1]

				y0=self.to_screen_y(self.data.Ec[0][0][y][band])+10
				y1=self.to_screen_y(self.data.Ec[0][0][y][band]+dE)+10

				pos=int(255*(log10(self.data.nt[0][0][y][band])-self.data.trap_min)/(self.data.trap_max-self.data.trap_min))

				br = QBrush(QColor(self.colors.map[pos][0], self.colors.map[pos][1], self.colors.map[pos][2]))  
				qp.setBrush(br)
				
				qp.drawRect(x0, y0, 1+x1-x0, -1+y1-y0)

				x0=self.to_screen_x(self.data.y_scale[y])
				y0=self.to_screen_y(self.data.Ev[0][0][y][band])
				y1=self.to_screen_y(self.data.Ev[0][0][y][band]+dE)

				pos=int(255*(log10(self.data.pt[0][0][y][band])-self.data.trap_min)/(self.data.trap_max-self.data.trap_min))
				br = QBrush(QColor(self.colors.map[pos][0], self.colors.map[pos][1], self.colors.map[pos][2]))  
				qp.setBrush(br)

				qp.drawRect(x0, y0, 1+x1-x0, -1+y1-y0)

		qp.setPen(Qt.SolidLine)

	def plot_data(self,qp):
		if self.data.valid_data==True:
			if len(self.data.y_scale)>1:
				if self.data.type!="trap_map":

					self.draw_graph_lines(qp)
				else:
					self.draw_trap_map(qp)

	def drawText(self,qp,x, y , text,r,g,b,svg=False):
		if svg==False:
			pen = QPen(QColor(r, g, b), 1, Qt.SolidLine)
			qp.setPen(pen)
			qp.drawText(x - len(text)*4, y , text)
			qp.setPen(Qt.NoPen)
		else:
			qp.add(qp.text(text,
				insert=(x,y),
				stroke='none',
				fill=svgwrite.rgb(r, g, b, '%'),
				font_size='15px')
			)

	def drawRect(self,qp,x,y,w,h,r,g,b,alpha,svg=False):
		if svg==False:
			if r==-1:
				qp.setBrush(Qt.NoBrush)
			else:
				qp.setBrush(QColor(r,g,b,alpha))
			qp.drawRect(x, y, w,h)
		else:
			if alpha==-1:
				qp.add(qp.rect((x,y), (w, h),stroke='none',fill=svgwrite.rgb(r, g, b, '%')))
			else:
				qp.add(qp.rect((x,y), (w, h),stroke='none',fill=svgwrite.rgb(r, g, b, '%'),opacity=alpha/255))

	def draw_title(self,qp):
		title=self.data.title
		if self.data.time!=-1.0 and self.data.Vexternal!=-1.0:
			mul,unit=time_with_units(self.data.time)
			title=title+" V="+str(self.data.Vexternal)+" "+_("time")+"="+str(self.data.time*mul)+" "+unit
			qp.drawText(self.width()/3, 20, title)

	def render_image(self,qp,svg=False):
		self.x0=self.width()*self.x0_mul
		if self.E_max==None:
			self.x1=self.width()*1.0
		else:
			self.x1=self.width()*0.9

		self.y0=self.height()*0.1
		self.y1=self.height()*0.85

		if svg==False:
			qp.setFont(self.font)
			pen = QPen(QColor(20, 20, 20), 1, Qt.SolidLine)
			qp.setPen(pen)
			qp.setBrush(Qt.NoBrush)


		if self.show_axis==True:
			self.drawRect(qp,self.x0, self.y0, self.x1-self.x0,self.y1-self.y0,-1,-1,-1,-1,svg=svg)


		if self.show_data==True:
			if svg==False:
				self.plot_data(qp)

		#x scale
		if self.show_axis==True:
			if svg==False:
				self.draw_title(qp)
				self.draw_x_scale(qp)
				self.draw_y_scale(qp)
				self.draw_y2_scale(qp)
		if svg==False:
			qp.setPen(Qt.NoPen)

		do_overlay=False
		try:
			self.draw_overlay
			do_overlay=True
		except:
			pass

		if do_overlay==True:
			self.draw_overlay(qp,svg=svg)

		return qp

	def drawWidget(self, qp):
		self.render_image(qp)

	def load_data(self,file_name):
		#print("reload",file_name)
		self.data=dat_file()
		self.data.load(file_name)
		if self.data.data!=None:
			[my_max,my_min] = self.data.max_min()

			self.data.data_min=my_min
			self.data.data_max=my_max

		if self.data.type=="trap_map":
			self.data.trap_min=log10(self.data.data_min)
			self.data.trap_max=log10(self.data.data_max)
			self.data.data_min=self.data.Ev_min
			self.data.data_max=self.data.Ec_max

		if self.use_epi_for_x==True:
			self.data_x0=0.0
			self.data_x1=get_epi().ylen()
		else:
			if len(self.data.y_scale)>0:
				self.data_x0=self.data.y_scale[0]
				self.data_x1=self.data.y_scale[len(self.data.y_scale)-1]


	def draw_graph(self):
		self.load_data(self.optical_mode_file)	
		self.repaint()
