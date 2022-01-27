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

## @package fxexperiment_mesh_tab
#  fx domain mesh editor
#

import os
from gui_util import dlg_get_text
import webbrowser
from util import fx_with_units
from icon_lib import icon_get

import i18n
_ = i18n.language.gettext

#inp

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QHBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView,QApplication
from PyQt5.QtGui import QPainter,QIcon


#windows
from open_save_dlg import save_as_jpg

from cal_path import get_sim_path
from gpvdm_tab2 import gpvdm_tab2

import matplotlib.cm as cm
import numpy as np
from json_fx_domain import json_fx_domain_mesh_segment
from gpvdm_json import gpvdm_data

class fxexperiment_mesh_tab(QWidget):

	def save_data(self):
		gpvdm_data().save()
		
	def update(self):
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()

	def draw_graph(self):
		if len(self.fx)==0:
			return

		my_max=self.fx[0][0]
		my_min=self.fx[0][0]

		for i in range(0,len(self.fx)):
			for ii in range(0,len(self.fx[i])):
				if self.fx[i][ii]>my_max:
					my_max=self.fx[i][ii]

				if self.fx[i][ii]<my_min:
					my_min=self.fx[i][ii]
	
		mul=1.0
		unit="Hz"

		fx=[]
		mag=[]
		for i in range(0,len(self.fx)):
			local_fx=[]
			for ii in range(0,len(self.fx[i])):
				local_fx.append(self.fx[i][ii]*mul)
				mag.append(1)
			fx.extend(local_fx)

		self.fig.clf()
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(left=0.1)
		self.ax1 = self.fig.add_subplot(111)

		cmap = cm.jet
		#self.ax1.clear()
		#self.ax1.scatter(self.x,self.mag ,c=c, cmap=cmap)
		#self.fig.canvas.draw()

		c = np.linspace(0, 10, len(fx))

		self.ax1 = self.fig.add_subplot(111)
		self.ax1.ticklabel_format(useOffset=False)

		self.ax1.set_ylabel(_("Magnitude")+" ("+_("Volts")+" )")
		self.ax1.get_yaxis().set_visible(False)
		self.ax1.spines['top'].set_visible(False)
		self.ax1.spines['right'].set_visible(False)
		#self.ax1.spines['bottom'].set_visible(False)
		self.ax1.spines['left'].set_visible(False)
		self.ax1.set_xscale("log")
		#print(fx,self.mag)
		self.ax1.scatter(fx,mag, c=c, cmap=cmap)
		
		self.ax1.set_xlabel(_("Frequency")+" ("+unit+")")


	def build_mesh(self):
		self.mag=[]
		self.fx=[]
		data=gpvdm_data().fx_domain.find_object_by_id(self.uid)

		for mesh_item in data.mesh.segments:
			local_mag=[]
			local_fx=[]
			start=mesh_item.start
			fx=start
			stop=mesh_item.stop
			max_points=mesh_item.points
			mul=mesh_item.mul
			pos=0
			if stop!=0.0 and max_points!=0.0 and mul!=0.0:
				if max_points==1:
					local_fx.append(fx)
					local_mag.append(1.0)
				else:
					fx_start=fx
					while(fx<stop):
						local_fx.append(fx)
						local_mag.append(1.0)
						if mul==1.0:
							fx=fx+(stop-fx_start)/max_points
						else:
							fx=fx*mul
						pos=pos+1
						if pos>max_points:
							break

			self.mag.append(local_mag)
			self.fx.append(local_fx)
			local_mag=[]
			local_fx=[]



	def redraw_and_save(self):
		self.update()
		self.save_data()

	def on_cell_edited(self):
		self.redraw_and_save()

	def save_image(self):
		file_name = save_as_jpg(self)
		if file_name !=None:
			self.fig.savefig(file_name)

	def __init__(self,uid):
		QWidget.__init__(self)
		self.uid=uid
		self.ax1=None

		self.fig = Figure(figsize=(5,4), dpi=100)
		self.canvas = FigureCanvas(self.fig)
		self.canvas.figure.patch.set_facecolor('white')

		self.main_vbox = QHBoxLayout()

		self.main_vbox.addWidget(self.canvas)
		self.canvas.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)


		#toolbar 2

		toolbar2=QToolBar()
		toolbar2.setIconSize(QSize(32, 32))

		tab_holder=QWidget()
		tab_vbox_layout= QVBoxLayout()
		tab_holder.setLayout(tab_vbox_layout)
		self.tab = gpvdm_tab2(toolbar=toolbar2)

		self.tab.set_tokens(["start","stop","points","mul"])
		self.tab.set_labels([_("Frequency start"),_("Frequency stop"), _("Max points"), _("Multiply")])

		self.tab.setColumnWidth(0, 200)
		self.tab.setColumnWidth(1, 200)

		data=gpvdm_data().fx_domain.find_object_by_id(self.uid)
		index=gpvdm_data().fx_domain.segments.index(data)
		self.tab.json_search_path="gpvdm_data().fx_domain.segments["+str(index)+"].mesh.segments"

		self.tab.populate()

		self.tab.new_row_clicked.connect(self.callback_new_row_clicked)
		self.tab.changed.connect(self.on_cell_edited)

		self.tab.setMinimumSize(self.width(), 120)

		tab_vbox_layout.addWidget(toolbar2)

		self.build_mesh()
		self.draw_graph()

		tab_vbox_layout.addWidget(self.tab)
		self.main_vbox.addWidget(tab_holder)

		self.setLayout(self.main_vbox)

	def callback_new_row_clicked(self,row):
		obj=json_fx_domain_mesh_segment()
		data=gpvdm_data().fx_domain.find_object_by_id(self.uid).mesh.segments.insert(row,obj)
		self.tab.insert_row(obj,row)


