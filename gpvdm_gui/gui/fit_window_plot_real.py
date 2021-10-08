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

## @package fit_window_plot_real
#  Another fit plotting window?
#

import os
#from numpy import *
from cal_path import get_image_file_path

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar, QMenu, QTableWidget, QAbstractItemView, QLabel
from PyQt5.QtGui import QPainter,QIcon,QCursor

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

from open_save_dlg import open_as_filter


from icon_lib import icon_get
from cal_path import get_sim_path

from plot_widget import plot_widget

class fit_window_plot_real(QWidget):

	def update(self):
		self.draw_graph()

	def draw_graph(self):

		plot_labels=["Experimental"]
		data_files=[os.path.join(get_sim_path(),self.data.import_config.data_file)]

		sim_data_file=os.path.join(get_sim_path(),"sim",self.data.config.fit_name,self.data.config.sim_data)	#[:-4]+".best"

		if os.path.isfile(sim_data_file)==True:
			plot_labels.append("Simulated")
			data_files.append(sim_data_file)


		self.plot.load_data(data_files)
		self.plot.set_labels(plot_labels)
		self.label.setText(_("Data imported from: ")+self.data.import_config.import_file_path+" col:"+str(self.data.import_config.import_x_spin)+" "+str(self.data.import_config.import_data_spin))
		self.plot.do_plot()


	def __init__(self,data):
		QWidget.__init__(self)

		self.data=data
		self.fig = Figure(figsize=(5,4), dpi=100)
		self.ax1=None
		self.show_key=True
		
		self.hbox=QVBoxLayout()

		self.label=QLabel()
		self.plot=plot_widget(enable_toolbar=False)
		self.draw_graph()

		self.hbox.addWidget(self.plot)
		self.hbox.addWidget(self.label)
		self.setLayout(self.hbox)
