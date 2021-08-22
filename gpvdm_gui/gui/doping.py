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

## @package doping
#  The doping dialog.
#

import os
from numpy import *
import webbrowser
from icon_lib import icon_get

import i18n
_ = i18n.language.gettext

#epitaxy
from epitaxy import epitaxy_get_layers

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidgetItem,QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

from open_save_dlg import save_as_image
from QWidgetSavePos import QWidgetSavePos
from cal_path import get_sim_path

from mesh import get_mesh

from epitaxy import epitaxy_get_epi
from error_dlg import error_dlg

#from file_watch import get_watch
from gpvdm_tab2 import gpvdm_tab2
from epitaxy import get_epi
from gpvdm_json import gpvdm_data

class doping_window(QWidgetSavePos):


	def update(self):
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()


	def draw_graph(self):
		self.fig.clf()
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(left=0.1)
		self.ax1 = self.fig.add_subplot(111)
		self.ax1.ticklabel_format(useOffset=False)

		self.ax1.set_ylabel(_("Doping (m^{-3})"))
		x_plot=[]
		for i in range(0,len(self.x_pos)):
			x_plot.append(self.x_pos[i]*1e9)


		frequency, = self.ax1.plot(x_plot,self.doping_Na, 'ro-', linewidth=3 ,alpha=1.0)
		self.ax1.set_xlabel(_("Position (nm)"))

		frequency, = self.ax1.plot(x_plot,self.doping_Nd, 'go-', linewidth=3 ,alpha=1.0)
		self.ax1.set_xlabel(_("Position (nm)"))

		self.ax2 = self.ax1.twinx()
		self.ax2.set_ylabel(_("Mobile ion density (m^{-3})"))

		self.ax2.plot(x_plot,self.ions, 'bo-', linewidth=3 ,alpha=1.0)

	def save_image(self,file_name):
		self.fig.savefig(file_name)

	def callback_save(self, widget, data=None):
		file_name=save_as_image(self)
		if file_name!=None:
			if os.path.splitext(file_name)[1]:
				self.save_image(file_name)
			else:
				self.save_image(file_name+".png")


	def callback_help(self):
		webbrowser.open("https://www.gpvdm.com/man/index.html")

	def project(self,token0,token1):
		data=gpvdm_data()
		mesh=get_mesh().y
		x,y =	mesh.calculate_points()
		device_start=self.epi.get_device_start(data)

		layer=self.epi.get_next_dos_layer(-1)

		for i in range(0,len(x)):
			Nad0=getattr(self.epi.layers[layer].shape_dos,token0)
			Nad1=getattr(self.epi.layers[layer].shape_dos,token1)

			if x[i]+device_start>self.epi.layers[layer].end:
				layer=layer+1

			dy=self.epi.layers[layer].dy
			y[i]=Nad0+(Nad1-Nad0)*(x[i]-self.epi.layers[layer].start+device_start)/dy

		return x,y

	def build_mesh(self):

		self.x_pos,self.doping_Na=self.project("Na0","Na1")
		self.x_pos,self.doping_Nd=self.project("Nd0","Nd1")
		self.x_pos,self.ions=self.project("ion_density","ion_density")

		return True


	def __init__(self):
		QWidgetSavePos.__init__(self,"doping")
		self.setMinimumSize(1000, 600)
		self.setWindowIcon(icon_get("doping"))
		self.setWindowTitle(_("Doping/Mobilie ion profile editor")+" (https://www.gpvdm.com)")

		self.epi=get_epi()

		self.main_vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		self.save = QAction(icon_get("document-save-as"), _("Save"), self)
		self.save.triggered.connect(self.callback_save)
		toolbar.addAction(self.save)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)


		self.help = QAction(icon_get("help"), _("Help"), self)
		self.help.triggered.connect(self.callback_help)
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)

		self.fig = Figure(figsize=(5,4), dpi=100)
		self.ax1=None
		self.show_key=True
		canvas = FigureCanvas(self.fig)
		#canvas.set_background('white')
		#canvas.set_facecolor('white')
		canvas.figure.patch.set_facecolor('white')
		canvas.show()

		self.main_vbox.addWidget(canvas)

		#tab2
		self.tab2 = gpvdm_tab2()
		self.tab2.set_tokens(["shape_name","shape_dos.Na0","shape_dos.Na1","shape_dos.Nd0","shape_dos.Nd1","shape_dos.ion_density","shape_dos.ion_mobility"])
		self.tab2.set_labels([_("Layer"),"Na0 (m^{-3})","Na1 (m^{-3})","Nd0 (m^{-3})","Nd1 (m^{-3})","Nion(+) (m^{-3})","Nion mu (m2 V^{-1}s^{-1})"])
		self.tab2.json_search_path="gpvdm_data().epi.layers"
		self.tab2.setColumnWidth(1, 120)
		self.tab2.setColumnWidth(2, 120)
		self.tab2.setColumnWidth(3, 120)
		self.tab2.setColumnWidth(4, 120)
		self.tab2.setColumnWidth(5, 140)
		self.tab2.setColumnWidth(6, 240)
		self.tab2.menu_disabled=True
		self.tab2.check_enabled_callback=self.check_enabled
		self.tab2.populate()
		self.tab2.changed.connect(self.callback_save)


		self.main_vbox.addWidget(self.tab2)
		self.update()

		self.setLayout(self.main_vbox)

	def callback_save(self):
		self.update()
		gpvdm_data().save()

	def check_enabled(self,s,token):
		if s.shape_dos.enabled==False:
			return False
		if token=="shape_name":
			return False

		return True

