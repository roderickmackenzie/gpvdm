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

## @package fit_window_plot
#  Widget to plot the results of the fit.
#

import os
from numpy import *
from plot_widget import plot_widget
import i18n
_ = i18n.language.gettext


#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTabWidget
from PyQt5.QtGui import QPainter,QIcon

#from matplotlib_toolbar import NavigationToolbar
#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

from open_save_dlg import save_as_jpg

from cal_path import get_sim_path

class fit_window_plot(QWidget):
	file_name=""

	def update(self):
		self.draw_graph()

	def draw_graph(self):

		error_sim=os.path.join(self.path,"fit_error_sim.dat")
		error_exp=os.path.join(self.path,"fit_error_exp.dat")
		print("update",error_sim,error_exp)
		delta=os.path.join(self.path,"fit_error_delta.dat")
		self.plot.load_data([error_sim,error_exp,delta])
		if (len(self.plot.y1_y2)>=2):
			self.plot.y1_y2[2]="y2"
		self.plot.set_labels(["Simulation","Experiment","Delta"])

		self.plot.do_plot()

	def export_image(self):
		self.plot.callback_save_image()

	def export_csv(self):
		self.plot.callback_save_csv()

	def export_xls(self):
		self.plot.callback_save_xls()

	def export_gnuplot(self):
		self.plot.callback_save_gnuplot()

	def __init__(self,path):
		QWidget.__init__(self)
		self.vbox=QVBoxLayout()
		self.path=path

		self.plot=plot_widget(enable_toolbar=False)
		self.vbox.addWidget(self.plot)
		
		self.setLayout(self.vbox)
		
		self.draw_graph()
