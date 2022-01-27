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

## @package optics_filters_tab
#  A mesh editor for the time domain mesh.
#


import os
from numpy import *
from gui_util import dlg_get_text
import webbrowser
from util import time_with_units
from cal_path import get_icon_path

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QLabel,QHBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenu
from PyQt5.QtGui import QPainter,QIcon

from gpvdm_tab2 import gpvdm_tab2
from gpvdm_json import gpvdm_data
from json_light import json_filter_spectrum
from global_objects import global_object_run

from plot_widget import plot_widget
from cal_path import get_sim_path

from icon_lib import icon_get
from tab import tab_class
from gpvdm_graph import gpvdm_graph
import i18n
_ = i18n.language.gettext


class optics_filters_tab(QWidget):
 
	def get_json_obj(self):
		data=gpvdm_data()
		data_obj=eval(self.serach_path).find_object_by_id(self.uid)
		return data_obj

	def __init__(self,search_path,uid,name):
		QWidget.__init__(self)
		self.main_vbox_y0 = QVBoxLayout()
		self.serach_path=search_path
		self.uid=uid

		label_left=QLabel(name)
		self.main_vbox_y0.addWidget(label_left)
		toolbar2=QToolBar()
		toolbar2.setIconSize(QSize(32, 32))

		self.main_vbox_y0.addWidget(toolbar2)
	

		self.tab_filters = gpvdm_tab2(toolbar=toolbar2)
		self.tab_filters.set_tokens(["filter_enabled","filter_material","filter_invert","filter_db"])
		self.tab_filters.set_labels([_("Enabled"),_("Filter"),_("Invert"),_("dB")])
		self.tab_filters.setColumnWidth(0, 100)
		self.tab_filters.setColumnWidth(1, 250)


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar2.addWidget(spacer)

		self.tab_filters.json_search_path=search_path
		self.tab_filters.uid=uid
		self.tab_filters.postfix="virtual_spectra.light_filters.segments"
		self.tab_filters.populate()

		self.tab_filters.new_row_clicked.connect(self.callback_new_row_clicked_filters)
		self.tab_filters.changed.connect(self.on_cell_edited)
		self.main_vbox_y0.addWidget(self.tab_filters)

		self.plot_widget=plot_widget(enable_toolbar=False,widget_mode="gpvdm_graph")
		self.plot_widget.set_labels([_("Attenuation")])
		plot_file=os.path.join(get_sim_path(),"optical_output","light_src_filter_"+self.uid+".csv")
		self.plot_widget.load_data([plot_file])
		self.plot_widget.canvas.x0_mul=0.2
		self.plot_widget.canvas.y_label="Transmission"
		self.plot_widget.canvas.x_label="Wavelength"
		self.plot_widget.canvas.enable_wavelength_to_rgb=True
		self.plot_widget.setMinimumHeight(250)

		self.update()

		self.main_vbox_y0.addWidget(self.plot_widget)
		self.setLayout(self.main_vbox_y0)


	def callback_new_row_clicked_filters(self,row):
		obj=json_filter_spectrum()
		self.get_json_obj().virtual_spectra.light_filters.segments.insert(row,obj)
		self.tab_filters.insert_row(obj,row)
		gpvdm_data().save()
		self.plot_widget.do_plot()

	def on_cell_edited(self):
		gpvdm_data().save()
		self.plot_widget.do_plot()
		global_object_run("gl_force_redraw")

	def update(self):
		self.plot_widget.reload()
		self.plot_widget.do_plot()



