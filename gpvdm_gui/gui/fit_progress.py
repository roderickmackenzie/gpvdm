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

## @package fit_progress
#  Widget to display the fit progress. 
#


import os
from fit_patch import fit_patch
import shutil

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar, QMenu, QTableWidget, QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon,QCursor

#windows
from open_save_dlg import save_as_filter
from plot_widget import plot_widget

from cal_path import get_sim_path

class fit_progress(QTabWidget):

	def update(self):
		for widget in self.plot_widgets:
			widget.reload()
			widget.do_plot()
	
	def update_for_fit(self):
		self.update()

	def __init__(self):
		QTabWidget.__init__(self)

		self.setMovable(True)
		self.plot_widgets=[]
		for file_name in ["fitlog.csv"]:	#,"fitlog_time_error.dat","fitlog_time_odes.dat"
			f_name=os.path.join(get_sim_path(),file_name)
			self.plot_widgets.append(plot_widget(enable_toolbar=False))
			self.plot_widgets[-1].set_labels([os.path.basename(f_name)])
			self.plot_widgets[-1].load_data([f_name])
			self.plot_widgets[-1].do_plot()

			self.addTab(self.plot_widgets[-1],file_name)
		

	def rename(self,tab_name):
		return
