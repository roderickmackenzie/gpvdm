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

## @package optics
#  The main optics window
#


import os
from inp import inp_update_token_value
from inp import inp_get_token_value
from plot_gen import plot_gen
from icon_lib import icon_get
import zipfile
import glob
from tab import tab_class
import webbrowser
from progress_class import progress_class
from help import my_help_class

#path
from cal_path import get_materials_path
from cal_path import get_exe_command

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QHBoxLayout,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu, QComboBox, QMenuBar, QLabel
from PyQt5.QtGui import QIcon

#windows
from band_graph import band_graph
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

		self.ribbon.optics.fx_box.cb.currentIndexChanged.connect(self.mode_changed)

		self.ribbon.optics.help.triggered.connect(self.callback_help)

		self.ribbon.tb_save.clicked.connect(self.callback_save)

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

		self.fig_photon_density = band_graph()
		self.fig_photon_density.set_data_file("light_1d_photons_tot_norm.dat")
		self.notebook.addTab(self.fig_photon_density,_("Photon density"))

		#self.fig_photon_abs = band_graph()
		#self.fig_photon_abs.set_data_file("light_1d_photons_tot_abs_norm.dat")
		#self.notebook.addTab(self.fig_photon_abs,_("Photon absorbed"))

		self.fig_gen_rate = band_graph()
		self.fig_gen_rate.set_data_file("light_1d_Gn.dat")
		self.notebook.addTab(self.fig_gen_rate,_("Generation rate"))



		self.fig_photon_density.draw_graph()
		#self.fig_photon_abs.draw_graph()
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
		widget=tab_class(os.path.join(get_sim_path(),"light.inp"))
		widget.show()

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
		inp_update_token_value("dump.inp", "#dump_optics",self.dump_optics)
		inp_update_token_value("dump.inp", "#dump_optics_verbose",self.dump_optics_verbose)
		self.force_redraw()

	def force_redraw(self):

		#self.fig_photon_density.my_figure.clf()
		self.fig_photon_density.draw_graph()
		#self.fig_photon_density.canvas.draw()

		#self.fig_photon_abs.my_figure.clf()
		#self.fig_photon_abs.draw_graph()
		#self.fig_photon_abs.canvas.draw()

		#self.fig_gen_rate.my_figure.clf()
		self.fig_gen_rate.draw_graph()
		#self.fig_gen_rate.canvas.draw()


		for i in range(0,len(self.plot_widgets)):
			self.plot_widgets[i].update()
			
		self.ribbon.update()
		
	def callback_run(self):
		self.my_server=server_get()
		self.dump_optics=inp_get_token_value("dump.inp", "#dump_optics")
		self.dump_optics_verbose=inp_get_token_value("dump.inp", "#dump_optics_verbose")

		inp_update_token_value("dump.inp", "#dump_optics","true")
		inp_update_token_value("dump.inp", "#dump_optics_verbose","true")
		#pwd=os.getcwd()
		#os.chdir(get_sim_path())
		#cmd = get_exe_command()+' --simmode opticalmodel@optics'
		#print(cmd)
		#ret= os.system(cmd)
		#os.chdir(pwd)
		self.my_server.clear_cache()
		self.my_server.add_job(get_sim_path(),"--simmode opticalmodel@optics")
		self.my_server.sim_finished.connect(self.optics_sim_finished)
		self.my_server.start()

		
		#inp_update_token_value("dump.inp", "#dump_optics","true")
		#inp_update_token_value("dump.inp", "#dump_optics_verbose","true")
		

	def mode_changed(self):
		cb_text=self.ribbon.optics.fx_box.get_text()
		if cb_text=="all":
			self.fig_photon_density.set_data_file("light_1d_photons_tot_norm.dat")
			self.fig_photon_density.draw_graph()
			#self.fig_photon_abs.set_data_file("light_1d_photons_tot_abs_norm.dat")
			#self.fig_photon_abs.draw_graph()
		else:
			self.fig_photon_density.set_data_file("light_1d_"+cb_text+"_photons_norm.dat")
			self.fig_photon_density.draw_graph()

			#self.fig_photon_abs.set_data_file("light_1d_"+cb_text+"_photons_abs.dat")
			#self.fig_photon_abs.draw_graph()

		#self.force_redraw()
		#self.update()


	def callback_help(self, widget, data=None):
		webbrowser.open('https://www.gpvdm.com/man/index.html')

