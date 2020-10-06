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

#inp
from inp import inp_update_token_value
from inp import inp_load_file
from inp import inp_search_token_value

import i18n
_ = i18n.language.gettext

#epitaxy
from epitaxy import epitaxy_get_layers
from epitaxy import epitaxy_get_dos_file
from epitaxy import epitaxy_get_dy

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
from epitaxy import epitaxy_get_layer
from error_dlg import error_dlg

from file_watch import get_watch
from gpvdm_tab import gpvdm_tab
from epitaxy import get_epi

class doping_window(QWidgetSavePos):
	lines=[]


	def save_data(self):
		print("save")
		pos=-1
		for i in range(0,self.tab.rowCount()):
			pos=self.epi.get_next_dos_layer(pos)
			dos_file=epitaxy_get_dos_file(pos)+".inp"
			file_name=os.path.join(get_sim_path(),dos_file)

			inp_update_token_value(file_name, "#doping_start", self.tab.item(i, 2).text())
			inp_update_token_value(file_name, "#doping_stop", self.tab.item(i, 3).text())

			inp_update_token_value(file_name, "#ion_density", self.tab.item(i, 4).text())
			inp_update_token_value(file_name, "#ion_mobility", self.tab.item(i, 5).text())

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


		frequency, = self.ax1.plot(x_plot,self.doping, 'ro-', linewidth=3 ,alpha=1.0)
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

	def load(self):
		self.tab.blockSignals(True)
		self.tab.clear()
		self.tab.setHorizontalHeaderLabels([_("Layer"), _("Width"), _("Doping Start (m-3)"), _("Doping Stop (m-3)"), _("Mobile ion density (m-3)"), _("Mobile ion mobility (m-3)")])
		layers=epitaxy_get_layers()

		row=0
		for i in range(0,layers):
			dos_file=epitaxy_get_dos_file(i)
			if dos_file.startswith("dos")==True:
				row=row+1
		self.tab.setRowCount(row)

		row=0
		for i in range(0,layers):
			dos_file=epitaxy_get_dos_file(i)
			e=epitaxy_get_layer(i)
			dy=e.dy
			if dos_file.startswith("dos")==True:
				lines=[]
				print("loading",dos_file)
				file_name=os.path.join(get_sim_path(),dos_file+".inp")
				lines=inp_load_file(file_name)
				if lines!=False:
					doping_start=float(inp_search_token_value(lines, "#doping_start"))
					doping_stop=float(inp_search_token_value(lines, "#doping_stop"))

					ion_density=float(inp_search_token_value(lines, "#ion_density"))
					ion_mobility=float(inp_search_token_value(lines, "#ion_mobility"))

					item = QTableWidgetItem(e.name)
					item.setFlags(item.flags() ^ Qt.ItemIsEnabled)
					self.tab.setItem(row,0,item)

					item = QTableWidgetItem(str(dy))
					item.setFlags(item.flags() ^ Qt.ItemIsEnabled)
					self.tab.setItem(row,1,item)

					item = QTableWidgetItem(str(doping_start))
					self.tab.setItem(row,2,item)

					item = QTableWidgetItem(str(doping_stop))
					self.tab.setItem(row,3,item)

					item = QTableWidgetItem(str(ion_density))
					self.tab.setItem(row,4,item)

					item = QTableWidgetItem(str(ion_mobility))
					self.tab.setItem(row,5,item)

				row=row+1

		self.tab.blockSignals(False)

		return

	def project(self,col0,col1):
		mesh=get_mesh().y
		x,y =	mesh.calculate_points()
		lay=mesh.mesh_cal_epi_layers(self.epi)
		device_start=self.epi.get_device_start()
		line=0
		layer=self.epi.get_next_dos_layer(-1)

		for i in range(0,len(x)):
			if x[i]+device_start>self.epi.layers[layer].end:
				layer=layer+1
				line=line+1

			try:
				Nad0=float(self.tab.item(line, col0).text())
				Nad1=float(self.tab.item(line, col1).text())
			except:
				Nad0=0.0
				Nad1=0.0

			dy=self.epi.layers[layer].dy
			y[i]=Nad0+(Nad1-Nad0)*(x[i]-self.epi.layers[layer].start+device_start)/dy

		return x,y

	def build_mesh(self):

		self.x_pos,self.doping=self.project(2,3)
		self.x_pos,self.ions=self.project(4,4)

		return True


	def tab_changed(self, y,x):
		val=self.tab.get_value(y,x)
		try:
			val=float(val)
		except:
			error_dlg(self,_(val+" is not a valid number."))
			return
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def __init__(self):
		QWidgetSavePos.__init__(self,"doping")
		self.setMinimumSize(900, 600)
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

		self.tab = gpvdm_tab()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)

		self.tab.clear()
		self.tab.setColumnCount(6)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)

		self.load()
		self.build_mesh()

		self.tab.cellChanged.connect(self.tab_changed)

		self.tab.setColumnWidth(2, 150)
		self.tab.setColumnWidth(3, 150)
		self.tab.setColumnWidth(4, 180)
		self.tab.setColumnWidth(5, 180)


		self.main_vbox.addWidget(self.tab)


		self.draw_graph()

		self.setLayout(self.main_vbox)

		layers=epitaxy_get_layers()
		for i in range(0,layers):
			dos_file=epitaxy_get_dos_file(i)+".inp"
			if dos_file.startswith("dos")==True:
				get_watch().add_call_back(dos_file,self.load)

		return


