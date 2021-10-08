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

## @package fit_tab
#  A tab containing a fit. 
#

import os
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

from import_data_json import import_data_json

from css import css_apply
from inp import inp
from util import strextract_interger
from gpvdm_json import gpvdm_data

class fit_tab(QTabWidget):

	def update(self,force=False):
		data=gpvdm_data()
		data_obj=data.fits.fits.find_object_by_id(self.uid)
		self.config_tab.tab.template_widget=data_obj.config
		self.config_tab.tab.update_values()

		if data_obj.config.enabled==True or force==True:
			self.tmesh_real.update()
			self.tmesh.update()

	def __init__(self,uid):
		self.uid=uid
		QTabWidget.__init__(self)

		data=gpvdm_data()
		data_obj=data.fits.fits.find_object_by_id(self.uid)

		css_apply(self,"tab_default.css")

		#self.setTabsClosable(True)
		#self.setMovable(True)

		self.tmesh_real = fit_window_plot_real(data_obj)
		self.addTab(self.tmesh_real,_("Experimental data"))

		self.tmesh = fit_window_plot(os.path.join(get_sim_path(),"sim",data_obj.config.fit_name))
		self.addTab(self.tmesh,_("Delta=Experiment - Simulation"))

		self.config_tab=tab_class(data_obj.config)
		self.addTab(self.config_tab,_("Configure fit"))

		self.fit_patch = fit_patch(data_obj.fit_patch)
		self.addTab(self.fit_patch, _("Fit patch"))

	def import_data(self):
		data=gpvdm_data()
		data_obj=data.fits.fits.find_object_by_id(self.uid)

		self.im=import_data_json(data_obj.import_config,export_path=get_sim_path())
		self.im.run()
		self.update(force=True)

	def get_tab_text(self):
		data=gpvdm_data()
		data_obj=data.fits.fits.find_object_by_id(self.uid)
		if data_obj.config.enabled==True:
			tick_cross="(\u2705)"
		else:
			tick_cross="(\u274C)"

		return data_obj.config.fit_name+" "+str(tick_cross)

	def rename(self,tab_name):
		data=gpvdm_data()
		data_obj=data.fits.fits.find_object_by_id(self.uid)
		tab = self.currentWidget()
		data_obj.config.fit_name=tab_name
		data.save()

