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

## @package optics
#  The main optics window
#


import os
from plot_gen import plot_gen
from icon_lib import icon_get
import zipfile
import glob
from tab import tab_class
from progress_class import progress_class
from help import my_help_class

#path
from cal_path import get_exe_command

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QHBoxLayout,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu, QComboBox, QMenuBar, QLabel
from PyQt5.QtGui import QIcon

#windows
from band_graph2 import band_graph2

from plot_widget import plot_widget
from error_dlg import error_dlg

from server import server_get

from global_objects import global_object_run
from global_objects import global_object_delete
from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos

from optics_ribbon import optics_ribbon

from css import css_apply
from gui_util import yes_no_dlg
from gpvdm_json import gpvdm_data
from config_window import class_config_window

class class_optical(QWidgetSavePos):

	def __init__(self):
		QWidgetSavePos.__init__(self,"optics")

		self.setWindowIcon(icon_get("optics"))

		self.setMinimumSize(1000, 600)
		self.setWindowTitle(_("Optical simulation editor")+" (https://www.gpvdm.com)")    

		self.ribbon=optics_ribbon()

		self.edit_list=[]
		self.line_number=[]
		self.articles=[]
		input_files=[]
		input_files.append(os.path.join(get_sim_path(),"optical_output","light_2d_photons.dat"))
		input_files.append(os.path.join(get_sim_path(),"optical_output","light_2d_photons_asb.dat"))

		plot_labels=[]
		plot_labels.append(_("Photon distribution"))
		plot_labels.append(_("Photon distribution absorbed"))


		self.setWindowIcon(icon_get("optics"))

		self.main_vbox=QVBoxLayout()

		self.ribbon.optics.run.start_sim.connect(self.callback_run)

		self.ribbon.optics.configwindow.triggered.connect(self.callback_configwindow)

		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)

		self.main_vbox.addWidget(self.ribbon)


		self.progress_window=progress_class()

		self.notebook = QTabWidget()
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)


		self.plot_widgets=[]
		self.progress_window.start()
		for i in range(0,len(input_files)):
			self.plot_widgets.append(plot_widget(enable_toolbar=False))
			self.plot_widgets[i].hide_title=True
			self.plot_widgets[i].set_labels([plot_labels[0]])
			self.plot_widgets[i].load_data([input_files[i]])
			#self.plot_widgets[i].watermark_alpha=0.5
			self.plot_widgets[i].do_plot()
			#self.plot_widgets[i].show()
			self.notebook.addTab(self.plot_widgets[i],plot_labels[i])

		self.fig_gen_rate = band_graph2()
		self.fig_gen_rate.set_data_file("light_1d_Gn.dat")
		self.notebook.addTab(self.fig_gen_rate,_("Generation rate"))


		self.fig_gen_rate.draw_graph()
		self.progress_window.stop()

		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.main_vbox.addWidget(self.notebook)


		self.setLayout(self.main_vbox)

		if os.path.isfile(os.path.join(get_sim_path(),"optical_output","light_2d_photons.dat"))==False:
			response=yes_no_dlg(self,"You have not yet run a full optical simulation, to use this feature you need to.  Would you run one now?")
			if response == True:
				self.callback_run()
			else:
				self.close()

	def callback_configwindow(self):
		data=gpvdm_data()
		self.config_window=class_config_window([data.light],[_("Output files")])
		self.config_window.show()

	def callback_save(self):
		tab = self.notebook.currentWidget()
		tab.save_image()

	def onclick(self, event):
		for i in range(0,len(self.layer_end)):
			if (self.layer_end[i]>event.xdata):
				break
		pwd=get_sim_path()
		plot_gen([os.path.join(pwd,"materials",self.layer_name[i],"alpha.gmat")],[],None,"")


	def closeEvent(self, event):
		global_object_delete("optics_force_redraw")
		self.hide()
		event.accept()

	def optics_sim_finished(self):
		data=gpvdm_data()
		data.light.dump_verbosity=self.dump_verbosity
		data.save()
		self.force_redraw()

	def force_redraw(self):
		self.fig_gen_rate.build_bands()
		self.fig_gen_rate.draw_graph()

		#print("redraw optics3")
		for i in range(0,len(self.plot_widgets)):
			self.plot_widgets[i].update()
		#print("redraw optics4")			
		self.ribbon.update()
		
	def callback_run(self):
		data=gpvdm_data()
		self.my_server=server_get()
		self.dump_verbosity=data.light.dump_verbosity

		data.light.dump_verbosity=1
		data.save()

		self.my_server.clear_cache()
		self.my_server.clear_jobs()
		self.my_server.add_job(get_sim_path(),"--simmode opticalmodel@optics")
		self.my_server.sim_finished.connect(self.optics_sim_finished)
		self.my_server.start()


