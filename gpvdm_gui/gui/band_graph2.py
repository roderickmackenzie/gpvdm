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
from PyQt5.QtGui import QIcon,QPixmap,QImage, QScreen
from PyQt5.QtGui import QPainter,QFont,QColor,QPen,QFontMetrics,QPainterPath

#calpath
from icon_lib import icon_get

from open_save_dlg import save_as_filter
from cal_path import get_sim_path
from cal_path import get_materials_path
from cal_path import get_default_material_path

from lock import get_lock

from epitaxy import get_epi
from PyQt5.QtCore import pyqtSignal
from json_material_db_item import json_material_db_item

from gpvdm_graph import gpvdm_graph
from epitaxy import get_epi

class band():
	def __init__(self):
		self.Eg=0.0
		self.Xi=0.0
		self.dx=0.0
		self.E_min=0.0
		self.E_max=0.0
		self.r=1
		self.g=0
		self.b=0
		self.alpha=0
		self.material=""
		self.name=""

	def cal_min_max(self):
		self.E_min=self.Xi-self.Eg
		self.E_max=self.Xi

		if self.material=="metal":
			self.E_min=self.Xi-1.0

class band_graph2(gpvdm_graph):

	def __init__(self):
		gpvdm_graph.__init__(self)
		self.use_epi_for_x=True
		self.bands=[]
		self.build_bands()

		self.setFocusPolicy( Qt.ClickFocus )
		self.setFocus()

		self.menu_show_energies=self.menu.addAction(_("Show energies"))
		self.menu_show_energies.triggered.connect(self.menu_toggle)
		self.menu_show_energies.setCheckable(True)
		self.menu_show_energies.setChecked(True)
		self.epi=get_epi()

	def draw_graph(self):
		self.load_data(self.optical_mode_file)	
		self.repaint()

	def build_bands(self):
		x_pos=0.0
		self.epi=get_epi()
		epi_len=self.epi.ylen()

		self.bands=[]
		for layer in self.epi.layers:
			mat_file=os.path.join(get_materials_path(),layer.optical_material,'data.json')
			mat_db_item=json_material_db_item()
			mat_db_item.load(mat_file)

			if self.normalize_x_axis==False:
				dy=layer.dy
			else:
				dy=epi_len/len(self.epi.layers)
			read_from_db=True
			if layer.layer_type=="active":
				if self.active_layer_from_dos==True:
					read_from_db=False

			if read_from_db==True:
				if mat_db_item.electrical_constants.material_blend==False:
					b=band()
					b.Xi=mat_db_item.electrical_constants.Xi0
					b.Eg=mat_db_item.electrical_constants.Eg0
					b.dx=dy
					b.r=layer.color_r*255
					b.g=layer.color_g*255
					b.b=layer.color_b*255
					b.alpha=layer.color_alpha*255
					b.material=mat_db_item.material_type
					b.name=layer.shape_name
					b.cal_min_max()
					self.bands.append(b)
				else:
					b=band()
					b.Xi=mat_db_item.electrical_constants.Xi0
					b.Eg=mat_db_item.electrical_constants.Eg0
					b.dx=dy/2.0
					b.r=layer.color_r*255
					b.g=layer.color_g*255
					b.b=layer.color_b*255
					b.alpha=layer.color_alpha*255
					b.material=mat_db_item.material_type
					b.name=layer.shape_name
					b.cal_min_max()
					self.bands.append(b)

					b=band()
					b.Xi=mat_db_item.electrical_constants.Xi1
					b.Eg=mat_db_item.electrical_constants.Eg1
					b.dx=dy/2.0
					b.r=layer.color_b*255
					b.g=layer.color_r*255
					b.b=layer.color_g*255
					b.alpha=layer.color_alpha*255
					b.material=mat_db_item.material_type
					b.name=layer.shape_name
					b.cal_min_max()
					self.bands.append(b)
			else:
				b=band()
				b.Xi=-float(layer.shape_dos.Xi)
				b.Eg=float(layer.shape_dos.Eg)
				b.dx=dy
				b.r=layer.color_r*255
				b.g=layer.color_g*255
				b.b=layer.color_b*255
				b.alpha=layer.color_alpha*255
				b.material=mat_db_item.material_type
				b.name=layer.shape_name
				b.cal_min_max()
				self.bands.append(b)
		if len(self.bands)>0:
			self.E_max=self.bands[0].E_max
			self.E_min=self.bands[0].E_min

			for b in self.bands:
				if b.E_max>self.E_max:
					self.E_max=b.E_max

				if b.E_min<self.E_min:
					self.E_min=b.E_min

			self.E_min=self.E_min-1.0
			self.E_max=self.E_max+0.5

	def draw_overlay(self,qp,svg=False):
		x=0.0
		for b in self.bands:
			if b.material!="metal":

				x0=self.to_screen_x(x)
				dx=self.to_screen_x(x+b.dx)-x0
				y0=self.to_screen_y2(b.Xi)
				self.drawRect(qp,x0, y0, dx,12,b.r,b.g,b.b,b.alpha,svg=svg)		#LUMO

				if self.show_energies==True:
					self.drawText(qp, x0+dx/2, y0-10 , "{:.1f}".format(b.Xi)+" eV",20,20,20,svg=svg)

				x0=self.to_screen_x(x)
				dx=self.to_screen_x(x+b.dx)-x0
				y0=self.to_screen_y2(b.Xi-b.Eg)	#HOMO
				self.drawRect(qp,x0, y0, dx,12,b.r,b.g,b.b,b.alpha,svg=svg)

				if self.show_energies==True:
					self.drawText(qp, x0+dx/2, y0+30 , "{:.1f}".format(b.Xi-b.Eg)+" eV",20,20,20,svg=svg)

				if self.show_labels==True:
					self.drawText(qp, x0+dx/2, self.to_screen_y2(b.Xi-b.Eg/2.0) , b.name,20,20,20,svg=svg)
			else:
				x0=self.to_screen_x(x)
				dx=self.to_screen_x(x+b.dx)-x0
				y0=self.to_screen_y2(b.Xi)
				self.drawRect(qp,x0, y0, dx,5,b.r,b.g,b.b,b.alpha,svg=svg)

				if self.show_energies==True:
					self.drawText(qp, x0+dx/2, y0-10 , "{:.1f}".format(b.Xi)+" eV",20,20,20,svg=svg)


				if self.show_labels==True:
					self.drawText(qp, x0+dx/2, self.to_screen_y2(b.Xi-1.0) , b.name,20,20,20,svg=svg)

			x=x+b.dx
