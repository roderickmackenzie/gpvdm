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

## @package qe
#  The quantum efficiency window - not used.
#


import os
from cal_path import get_exe_command
from numpy import *

#matplot lib
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
import matplotlib.pyplot as plt

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar
from PyQt5.QtGui import QPainter,QIcon

from icon_lib import icon_get

import i18n
_ = i18n.language.gettext

class qe_window(QWidget):

	def update_graph(self):
		cmd = get_exe_command()+' --onlypos'
#		ret= os.system(cmd)
		self.fig.clf()
		self.draw_graph()
		self.fig.canvas.draw()

	def draw_graph(self):
		self.fig.clf()
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(left=0.1)
		self.ax1 = self.fig.add_subplot(111)


		self.ax1.set_ylabel(_("Energy")+" (eV)")
		self.ax1.set_xlabel(_("Position")+" (nm)")
		try:
			t,s = loadtxt("Ec.dat", unpack=True)
			t=t*1e9
			Ec, = self.ax1.plot(t,s, 'ro-', linewidth=3 ,alpha=0.5)

			t,s = loadtxt("Ev.dat", unpack=True)
			t=t*1e9
			Ev,=self.ax1.plot(t,s, 'go-', linewidth=3 ,alpha=0.5)

			t,s = loadtxt("Fi.dat", unpack=True)
			t=t*1e9
			Fi,=self.ax1.plot(t,s, 'bo-', linewidth=3 ,alpha=0.5)

			if self.show_key==True:
				self.fig.legend((Ec, Ev, Fi), ('LUMO', 'HOMO', 'Fi'), 'upper right')
			else:
				self.ax1.legend_ = None
			self.fig.canvas.draw()

		except:
			print(_("No file")+"\n")


	def save_image(self,file_name):
		self.fig.savefig(file_name)

	def callback_refresh(self):
		self.update_graph()

	def callback_close(self):
		self.hide()
		return True

	def callback_save(self):
		file_name = save_as_jpg(self)
		if file_name !=None:
			self.save_image(file_name)

	def __init__(self):
		QWidget.__init__(self)
		self.setWindowTitle(_("Quantum Efficiency calculator")+" - (www.gpvdm.com)")
		self.setWindowIcon(icon_get("qe"))

		self.fig = Figure(figsize=(5,4), dpi=100)
		self.ax1=None
		self.show_key=True

		self.draw_graph()
		canvas = FigureCanvas(self.fig)
		#canvas.set_background('white')
		#canvas.set_facecolor('white')
		canvas.figure.patch.set_facecolor('white')
		#canvas.set_size_request(500, 150)
		#canvas.show()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		self.tb_save = QAction(icon_get("document-save-as"), _("Save graph"), self)
		self.tb_save.triggered.connect(self.callback_save)
		toolbar.addAction(self.tb_save)

		self.tb_refresh = QAction(icon_get("media-playback-start"), _("Run"), self)
		self.tb_refresh .triggered.connect(self.callback_refresh)
		toolbar.addAction(self.tb_refresh )

		nav_bar=NavigationToolbar(canvas,self)
		toolbar.addWidget(nav_bar)

		
		window_main_vbox=QVBoxLayout()
		window_main_vbox.addWidget(toolbar)
		window_main_vbox.addWidget(canvas)

		self.setLayout(window_main_vbox)
