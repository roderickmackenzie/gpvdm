# -*- coding: utf-8 -*-
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

## @package solar_main
#  Part of solar module - delete?
#
import sys
from PyQt5.QtWidgets import QMenuBar, QWidget, QCalendarWidget,QTimeEdit, QAction,QDesktopWidget,QTabWidget,QVBoxLayout,QHBoxLayout, QLineEdit, QLabel
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QTime

import os

from ribbon_solar import ribbon_solar

from icon_lib import icon_get
from PyQt5.QtCore import pyqtSignal
from plot_widget import plot_widget
from spectral2 import spectral2
from dat_file import dat_file
from cal_path import get_spectra_path
from inp import inp_update_token_value
from inp import inp_get_token_value

from code_ctrl import am_i_rod

class spectral2_gui(QWidget):


	def __init__(self):
		super().__init__()
		top_hbox=QHBoxLayout()
		top_widget=QWidget()
		#top_widget.setLayout(top_hbox)

		self.plot=plot_widget(enable_toolbar=False)
		#self.plot.load_data(["jv.dat"])
		#self.plot.set_labels(["jv.dat"])
		
		self.plot.do_plot()
		#self.plot.fig.canvas.draw()

		date_widget=QWidget()
		date_vbox=QVBoxLayout()
		self.cal = QCalendarWidget(self)
		self.cal.setGridVisible(True)
		self.cal.move(10, 20)
		self.time=QTimeEdit()
		time=QTime(12, 30);
		self.time.setTime(time);
		date_vbox.addWidget(self.cal)
		date_vbox.addWidget(self.time)

		self.lat_widget=QWidget()
		self.lat_layout=QHBoxLayout()
		self.lat_label=QLabel("Latitude")
		self.lat_edit=QLineEdit()
		self.lat_layout.addWidget(self.lat_label)
		self.lat_layout.addWidget(self.lat_edit)
		self.lat_widget.setLayout(self.lat_layout)
		date_vbox.addWidget(self.lat_widget)


		self.preasure_widget=QWidget()
		self.preasure_layout=QHBoxLayout()
		self.preasure_label=QLabel("Preasure (bar)")
		self.preasure_edit=QLineEdit()
		self.preasure_layout.addWidget(self.preasure_label)
		self.preasure_layout.addWidget(self.preasure_edit)
		self.preasure_widget.setLayout(self.preasure_layout)
		date_vbox.addWidget(self.preasure_widget)

		self.aod_widget=QWidget()
		self.aod_layout=QHBoxLayout()
		self.aod_label=QLabel("AOD (cm-1)")
		self.aod_edit=QLineEdit()
		self.aod_layout.addWidget(self.aod_label)
		self.aod_layout.addWidget(self.aod_edit)
		self.aod_widget.setLayout(self.aod_layout)
		date_vbox.addWidget(self.aod_widget)

		self.water_widget=QWidget()
		self.water_layout=QHBoxLayout()
		self.water_label=QLabel("Water (cm-1)")
		self.water_edit=QLineEdit()
		self.water_layout.addWidget(self.water_label)
		self.water_layout.addWidget(self.water_edit)
		self.water_widget.setLayout(self.water_layout)
		date_vbox.addWidget(self.water_widget)

		self.no2_widget=QWidget()
		self.no2_layout=QHBoxLayout()
		self.no2_label=QLabel("NO2 (cm-1)")
		self.no2_edit=QLineEdit()
		self.no2_layout.addWidget(self.no2_label)
		self.no2_layout.addWidget(self.no2_edit)
		self.no2_widget.setLayout(self.no2_layout)
		if am_i_rod()==True:
			date_vbox.addWidget(self.no2_widget)

		date_widget.setLayout(date_vbox)

		top_hbox.addWidget(self.plot)
		top_hbox.addWidget(date_widget)

		self.setLayout(top_hbox)

		#inp_get_token_value("spectral2.inp","#spectral2_day")
		#inp_get_token_value("spectral2.inp","#spectral2_hour")
		#inp_get_token_value("spectral2.inp","#spectral2_minute")

		self.water_edit.setText(inp_get_token_value("spectral2.inp","#spectral2_water"))
		self.aod_edit.setText(inp_get_token_value("spectral2.inp","#spectral2_aod"))
		self.preasure_edit.setText(inp_get_token_value("spectral2.inp","#spectral2_preasure"))
		self.lat_edit.setText(inp_get_token_value("spectral2.inp","#spectral2_lat"))
		self.no2_edit.setText(inp_get_token_value("spectral2.inp","#spectral2_no2"))


		self.calculate()

	def calculate(self):
		
		self.plot.data=[]
		self.water_edit.text()


		day=self.cal.selectedDate().dayOfYear()
		hour=self.time.time().hour()
		minute=self.time.time().minute()

		inp_update_token_value("spectral2.inp","#spectral2_day",str(day))
		inp_update_token_value("spectral2.inp","#spectral2_hour",str(hour))
		inp_update_token_value("spectral2.inp","#spectral2_minute",str(minute))

		inp_update_token_value("spectral2.inp","#spectral2_lat",self.lat_edit.text())
		inp_update_token_value("spectral2.inp","#spectral2_aod",self.aod_edit.text())
		inp_update_token_value("spectral2.inp","#spectral2_preasure",self.preasure_edit.text())
		inp_update_token_value("spectral2.inp","#spectral2_water",self.water_edit.text())
		inp_update_token_value("spectral2.inp","#spectral2_no2",self.no2_edit.text())


		s=spectral2()
		s.calc()

		am=dat_file()
		am.load(os.path.join(get_spectra_path(),"AM1.5G","spectra.inp"))
		am.key_text="AM1.5G"
		self.plot.data.append(am)

		self.plot.data.append(s.Iglobal)

		self.plot.data.append(s.Id)

		self.plot.data.append(s.Is)

		self.plot.norm_data()
		self.plot.do_plot()

