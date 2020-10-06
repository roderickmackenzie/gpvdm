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

## @package fit_tab
#  A tab containing a fit. 
#

import os
from inp import inp_load_file
from inp import inp_search_token_value
from inp import inp_update_token_value
from tab import tab_class
from fit_patch import fit_patch
import shutil

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar, QMenu, QTableWidget, QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon,QCursor

#windows
from fit_window_plot import fit_window_plot
from fit_window_plot_real import fit_window_plot_real

from open_save_dlg import save_as_filter
from cal_path import get_sim_path
from str2bool import str2bool

from matlab_editor import matlab_editor

from import_data import import_data

from css import css_apply
from order_widget import extract_number_from_file_name
from inp import inp

mesh_articles = []

class fit_tab(QTabWidget):

	def set_enable(self,state):
		f=inp()
		f.load(self.get_file_name())
		f.replace("#enabled",state)
		f.save()
		

	def is_enabled(self):
		f=inp()
		f.load(self.get_file_name())
		return str2bool(f.get_token("#enabled"))

	def get_file_name(self):
		return os.path.join(get_sim_path(),"fit"+str(self.index)+".inp")

	def update(self):
		lines=inp_load_file(self.get_file_name())
		if lines!=False:
			enabled=str2bool(inp_search_token_value(lines, "#enabled"))
			if enabled==True:
				self.tmesh_real.update()
				self.tmesh.update()

	def __init__(self,file_name):
		QTabWidget.__init__(self)
		self.file_name=file_name
		css_apply(self,"tab_default.css")
		lines=[]
		self.index=int(extract_number_from_file_name(file_name))
		lines=inp_load_file(os.path.join(get_sim_path(),"fit"+str(self.index)+".inp"))
		if lines!=False:
			self.tab_name=inp_search_token_value(lines, "#fit_name")
		else:
			self.tab_name=""

		#self.setTabsClosable(True)
		#self.setMovable(True)

		self.tmesh_real = fit_window_plot_real(self.index)
		self.addTab(self.tmesh_real,_("Experimental data"))

		self.tmesh = fit_window_plot(self.index)
		self.addTab(self.tmesh,_("Delta=Experiment - Simulation"))

		config=tab_class(os.path.join(get_sim_path(),"fit"+str(self.index)+".inp"))
		self.addTab(config,_("Configure fit"))

		self.fit_patch = fit_patch(self.index)
		self.addTab(self.fit_patch, _("Fit patch"))

		self.matlab_editor = matlab_editor(self.index)
		self.addTab(self.matlab_editor, _("MATLAB code"))
		
	def init(self,index):
		return

	def import_data(self):
		target=os.path.join(get_sim_path(),"fit_data"+str(self.index)+".inp")
		config=os.path.join(get_sim_path(),"fit_import_config"+str(self.index)+".inp")
		self.im=import_data(target,config)
		self.im.run()
		self.update()

	def get_tab_text(self):
		enabled=self.is_enabled()
		if enabled==True:
			enabled="(\u2705)"
		else:
			enabled="(\u274C)"

		return self.tab_name+" "+enabled

	def rename(self,tab_name):
		tab = self.currentWidget()

		inp_update_token_value(os.path.join(get_sim_path(),"fit"+str(self.index)+".inp"), "#fit_name", self.tab_name)

	def callback_export_image(self):
		tab = self.currentWidget()
		if (type(tab) != fit_window_plot_real) and (type(tab) != fit_window_plot):
			return

		tab.plot.callback_save_image()

	def callback_export_csv(self):
		tab = self.currentWidget()
		if (type(tab) != fit_window_plot_real) and (type(tab) != fit_window_plot):
			return

		tab.plot.callback_save_csv()

	def callback_export_xls(self):
		tab = self.currentWidget()
		if (type(tab) != fit_window_plot_real) and (type(tab) != fit_window_plot):
			return

		tab.plot.callback_save_xls()

	def callback_export_gnuplot(self):
		tab = self.currentWidget()
		if (type(tab) != fit_window_plot_real) and (type(tab) != fit_window_plot):
			return

		tab.plot.callback_save_gnuplot()


