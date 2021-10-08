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

## @package electrical_mesh_editor
#  Window to edit the electrical mesh.
#

import os
from icon_lib import icon_get

#qt
from PyQt5.QtWidgets import  QAction
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar, QMessageBox, QVBoxLayout, QGroupBox, QTableWidget,QAbstractItemView, QTableWidgetItem
from PyQt5.QtCore import pyqtSignal

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
import matplotlib.cm as cm
from matplotlib import rcParams

from mesh import get_mesh

import numpy as np

from util import distance_with_units

from leftright import leftright
from str2bool import str2bool

from cal_path import get_sim_path

from gpvdm_tab import gpvdm_tab
from gpvdm_json import gpvdm_data
from json_mesh import json_mesh_segment

class electrical_mesh_editor_tab(QGroupBox):

	changed = pyqtSignal()

	def insert_row(self,pos,thick,points,mul,left_right):

		item=QTableWidgetItem(str(thick))
		self.tab.setItem(pos,0,item)

		item=QTableWidgetItem(str(points))
		self.tab.setItem(pos,1,item)

		item=QTableWidgetItem(str(mul))
		self.tab.setItem(pos,2,item)

		self.item = leftright()
		self.item.set_value(str2bool(left_right))
		self.item.changed.connect(self.tab_changed)
		self.tab.setCellWidget(pos,3,self.item)
	
	def on_add_mesh_clicked(self):
		self.tab.blockSignals(True)
		index = self.tab.selectionModel().selectedRows()

		if len(index)>0:
			pos=index[0].row()+1
		else:
			pos = self.tab.rowCount()

		self.tab.insertRow(pos)
		self.insert_row(pos,100e-9,20,1.1,"left")

		self.update_json()
		self.tab.blockSignals(False)
		self.changed.emit()

	def on_remove_click(self):
		self.tab.blockSignals(True)
		index = self.tab.selectionModel().selectedRows()

		if len(index)>0:
			pos=index[0].row()
			self.tab.removeRow(pos)

		self.update_json()
		self.tab.blockSignals(False)
		self.changed.emit()

	def save(self):
		self.update_json()


	def redraw(self):
		self.ax1.clear()
		self.x=[]
		self.mag=[]
		pos=0
		ii=0
		tot=0.0
		for i in range(0,self.tab.rowCount()):
			tot=tot+float(self.tab.item(i, 0).text())
			
		mul,unit=distance_with_units(tot)
		x_ret,self.mag=self.mesh.calculate_points()

		self.x=[]
		for i in range(0,len(x_ret)):
			self.x.append(x_ret[i]*mul)
			#print(x_ret[i])
		c = np.linspace(0, 10, len(self.x))
		
		self.ax1.set_ylabel(_("Magnitude"))
		cmap = cm.jet
		self.ax1.clear()
		self.ax1.scatter(self.x,self.mag ,c=c, cmap=cmap)
		self.fig.canvas.draw()
		self.ax1.set_xlabel(_("Thickness")+" ("+unit+")")
		self.ax1.get_yaxis().set_visible(False)
		self.ax1.spines['top'].set_visible(False)
		self.ax1.spines['right'].set_visible(False)
		#self.ax1.spines['bottom'].set_visible(False)
		self.ax1.spines['left'].set_visible(False)

	def update(self):
		self.load()

	def disable_dim(self):
		self.tab.setItem(0,1,QTableWidgetItem("1"))
		self.redraw()
		self.save()

	def enable_dim(self):

		if int(self.tab.rowCount())==1:
			self.tab.setItem(0,1,QTableWidgetItem("10"))
			self.redraw()
			self.save()


	def load(self):
		self.tab.blockSignals(True)
		self.tab.clear()
		self.tab.setHorizontalHeaderLabels([_("Thicknes"), _("Mesh points"), _("Step multiply"), _("Left/Right")])

		self.tab.setRowCount(len(self.mesh.data.segments))

		pos=0
		for seg in self.mesh.data.segments:
			self.insert_row(pos,seg.len,seg.points,seg.mul,seg.left_right)
			pos=pos+1

		self.redraw()
		self.tab.blockSignals(False)
	
	def tab_changed(self):
		self.update_json()

	def update_json(self):
		data=gpvdm_data()
		self.mesh.data.segments=[]
		for i in range(0,self.tab.rowCount()):
			a=json_mesh_segment()
			a.len=float(self.tab.get_value(i, 0))
			a.points=float(self.tab.get_value(i, 1))
			a.mul=float(self.tab.get_value(i, 2))
			a.left_right=self.tab.get_value(i, 3)
			self.mesh.data.segments.append(a)

		self.redraw()
		self.changed.emit()
		data.save()

	def __init__(self,xyz):
		self.xyz=xyz
		data=gpvdm_data()
		QGroupBox.__init__(self)
		rcParams.update({'figure.autolayout': True})
		self.setTitle(self.xyz)
		self.setStyleSheet("QGroupBox {  border: 1px solid gray;}")
		vbox=QVBoxLayout()
		self.setLayout(vbox)

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		vbox.addWidget(toolbar)

		self.tab = gpvdm_tab(toolbar=toolbar)

		self.tab.tb_add.triggered.connect(self.on_add_mesh_clicked)
		self.tab.user_remove_rows.connect(self.on_remove_click)
		self.tab.tb_up.triggered.connect(self.tab_changed)
		self.tab.tb_down.triggered.connect(self.tab_changed)

		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)

		self.tab.clear()
		self.tab.setColumnCount(4)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)

		self.tab.cellChanged.connect(self.tab_changed)

		vbox.addWidget(self.tab)

		self.fig = Figure(figsize=(5,2), dpi=100)
		self.canvas = FigureCanvas(self.fig)
		self.canvas.figure.patch.set_facecolor('white')
		
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(left=0.1)
		self.ax1 = self.fig.add_subplot(111)
		self.ax1.ticklabel_format(useOffset=False)

		vbox.addWidget(self.canvas)

		if self.xyz=="y": 
			self.mesh=get_mesh().y
			if data.electrical_solver.solver_type=="circuit":
				self.tab.tb_add.setEnabled(False)
				self.tab.tb_remove.setEnabled(False)
				self.tab.tb_up.setEnabled(False)
				self.tab.tb_down.setEnabled(False)
				self.tab.setEnabled(False)
		elif self.xyz=="x":
			self.mesh=get_mesh().x
		elif self.xyz=="z":
			self.mesh=get_mesh().z



		self.load()


