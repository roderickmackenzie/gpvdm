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

## @package fit_window_plot_real
#  Another fit plotting window?
#

import os
#from numpy import *
from cal_path import get_image_file_path
from util import read_xyz_data

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar, QMenu, QTableWidget, QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon,QCursor

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

from open_save_dlg import open_as_filter

mesh_articles = []

from icon_lib import icon_get
from cal_path import get_sim_path

from plot_widget import plot_widget

class fit_window_plot_real(QWidget):
	lines=[]
	edit_list=[]

	line_number=[]
	save_file_name=""

	file_name=""
	name=""
	visible=1

	def update(self):
		self.draw_graph()

	def draw_graph(self):

		plot_labels="Experimental data"
		data_file=os.path.join(get_sim_path(),"fit_data"+str(self.index)+".inp")

		self.plot.load_data([data_file])
		self.plot.set_labels([plot_labels])

		self.plot.do_plot()


	def export_image(self):
		self.plot.callback_save_image()

	def export_csv(self):
		self.plot.callback_save_csv()

	def export_xls(self):
		self.plot.callback_save_xls()

	def export_gnuplot(self):
		self.plot.callback_save_gnuplot()

	def __init__(self,index):
		QWidget.__init__(self)

		self.index=index
		self.fig = Figure(figsize=(5,4), dpi=100)
		self.ax1=None
		self.show_key=True
		
		self.hbox=QVBoxLayout()
		self.edit_list=[]
		self.line_number=[]

		self.list=[]

		self.plot=plot_widget(enable_toolbar=False)

		self.draw_graph()

		self.hbox.addWidget(self.plot)

		self.setLayout(self.hbox)
