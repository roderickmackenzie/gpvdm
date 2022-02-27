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

## @package scan
#  Them main scan material parameter window.
#

import os
import shutil
from icon_lib import icon_get
from gui_util import dlg_get_text
from search import return_file_list
from search import find_fit_log
from inp import inp_update_token_value
from inp import inp_get_token_value
from gui_util import yes_no_dlg
from util import wrap_text

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#window
from scan_tab import scan_vbox
from QHTabBar import QHTabBar

from global_objects import global_object_get

from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos
from scan_ribbon import scan_ribbon
from css import css_apply
from error_dlg import error_dlg
from gpvdm_viewer import gpvdm_viewer
from scans_io import scans_io
from scan_io import scan_io
from server import server_get

class scan_class(QWidgetSavePos):

	def callback_report(self):
		tab = self.notebook.currentWidget()
		tab.gen_report()

	def callback_new_scan(self):
		new_sim_name=dlg_get_text( _("New simulation name:"), _("Simulation ")+str(self.notebook.count()+1),"document-new.png")
		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			scans=scans_io(self.sim_dir)
			scans.new(new_sim_name)
			self.viewer.fill_store()

	def callback_cluster_fit_log(self):
		tab = self.notebook.currentWidget()
		index=self.notebook.currentIndex()
		name=self.notebook.tabText(index)
		path=os.path.join(self.sim_dir,name)
		find_fit_log("./fit.dat",path)
		os.system("gnuplot -persist ./fit.dat &\n")


	def callback_clone(self):
		if len(self.viewer.selected)==1:
			self.viewer.clone()
			self.viewer.fill_store()

	def callback_run_simulation(self):
		tab = self.notebook.currentWidget()
		tab.simulate(True,False,"")

	def callback_build_scan(self):
		tab = self.notebook.currentWidget()
		tab.build_scan()

	def callback_scan_run(self):
		tab = self.notebook.currentWidget()
		tab.scan_run()

	def callback_scan_archive(self):
		tab = self.notebook.currentWidget()
		tab.scan_archive()

	def callback_run_single_fit(self):
		tab = self.notebook.currentWidget()
		tab.simulate(True,False,"--1fit")

	def callback_run_simulation_nogen(self):
		tab = self.notebook.currentWidget()
		tab.simulate(True,False,"")

	def callback_plot_fits(self):
		tab = self.notebook.currentWidget()
		tab.plot_fits()

	def callback_clean_all(self):
		s=scans_io(self.sim_dir)
		scans=s.get_scans()
		for scan in scans:
			scan.parent_window=self
			scan.clean_dir()

	def callback_clean_unconverged_simulation(self):
		tab = self.notebook.currentWidget()
		tab.scan_clean_unconverged()

	def callback_clean_simulation_output(self):
		tab = self.notebook.currentWidget()
		tab.scan_clean_simulation_output()


	def callback_push_unconverged_to_hpc(self):
		tab = self.notebook.currentWidget()
		tab.push_unconverged_to_hpc()

	def remove_invalid(self,input_name):
		return input_name.replace (" ", "_")

	def callback_selection_changed(self):
		if len(self.viewer.selected)==1:
			self.ribbon.tb_delete.setEnabled(True)
			self.ribbon.tb_clone.setEnabled(True)
			self.ribbon.tb_rename.setEnabled(True)
		else:
			self.ribbon.tb_delete.setEnabled(False)
			self.ribbon.tb_clone.setEnabled(False)
			self.ribbon.tb_rename.setEnabled(False)
		#print(self.viewer.selected)

	def callback_rename(self):
		if len(self.viewer.selected)==1:
			self.viewer.rename()
			self.viewer.fill_store()

	def callback_delete_page(self):
		if len(self.viewer.selected)==1:
			self.viewer.delete()
			self.viewer.fill_store()


	def callback_run_all_simulations(self):
		scansio=scans_io(self.sim_dir)
		scans=scansio.get_scans()
		for s in scans:
			s.parent_window=self
			s.myserver=server_get()
			s.set_base_dir(get_sim_path())
			s.run()

			#print(s.scan_dir)
			#tab = self.notebook.widget(i)
			#tab.simulate(True,True,"")

	def callback_stop_simulation(self,widget):
		tab = self.notebook.currentWidget()
		tab.stop_simulation()

	def callback_notes(self):
		tab = self.notebook.currentWidget()
		tab.callback_notes()

	def __init__(self,my_server):
		QWidgetSavePos.__init__(self,"scan_window")
		self.myserver=my_server
		self.setMinimumSize(1000,500)
		self.setWindowTitle(_("Parameter scan - gpvdm"))
		self.setWindowIcon(icon_get("scan"))

		self.rod=[]
		self.sim_dir=get_sim_path()

		self.main_vbox = QVBoxLayout()
		self.ribbon=scan_ribbon()
		self.main_vbox.addWidget(self.ribbon)

		self.ribbon.menu_plot_fits.triggered.connect(self.callback_plot_fits)

		self.ribbon.sim_no_gen.triggered.connect(self.callback_run_simulation_nogen)

		self.ribbon.single_fit.triggered.connect(self.callback_run_single_fit)

		self.ribbon.clean_unconverged.triggered.connect(self.callback_clean_unconverged_simulation)

		self.ribbon.clean_sim_output.triggered.connect(self.callback_clean_simulation_output)

		self.ribbon.push_unconverged_to_hpc.triggered.connect(self.callback_push_unconverged_to_hpc)

		
		self.ribbon.report.triggered.connect(self.callback_report)

		self.ribbon.tb_new.triggered.connect(self.callback_new_scan)

		self.ribbon.tb_delete.triggered.connect(self.callback_delete_page)

		self.ribbon.tb_clone.triggered.connect(self.callback_clone)

		self.ribbon.tb_rename.triggered.connect(self.callback_rename)
		
		#self.ribbon.tb_simulate.start_sim.connect(self.callback_run_simulation)

		#self.ribbon.tb_build.triggered.connect(self.callback_build_scan)

		#self.ribbon.tb_rerun.triggered.connect(self.callback_scan_run)

		#self.ribbon.tb_zip.triggered.connect(self.callback_scan_archive)

		self.ribbon.tb_run_all.triggered.connect(self.callback_run_all_simulations)

		#self.ribbon.tb_plot.triggered.connect(self.callback_plot)
	
		#self.ribbon.tb_plot_time.triggered.connect(self.callback_examine)

		#self.ribbon.tb_notes.triggered.connect(self.callback_notes)

		self.ribbon.tb_clean.triggered.connect(self.callback_clean_all)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)


		self.notebook = QTabWidget()
		self.notebook.setTabBar(QHTabBar())
		css_apply(self.notebook,"style_h.css")

		self.notebook.setTabPosition(QTabWidget.West)
		self.notebook.setMovable(True)


		#self.main_vbox.addWidget(self.notebook)

		self.viewer=gpvdm_viewer(self.sim_dir)
		self.viewer.allow_navigation=True
		self.viewer.set_directory_view(True)
		self.viewer.set_back_arrow(False)
		self.viewer.show_only=["scan_dir"]
		self.viewer.set_multi_select()
		self.viewer.selection_changed.connect(self.callback_selection_changed)
		self.callback_selection_changed()

		self.main_vbox.addWidget(self.viewer)

		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)		

		
		self.setLayout(self.main_vbox)


	def callback_plot(self):
		tab = self.notebook.currentWidget()
		tab.callback_gen_plot_command()
	
	def callback_examine(self):
		tab = self.notebook.currentWidget()
		tab.callback_examine()
		
	def callback_run_simulation(self):
		tab = self.notebook.currentWidget()
		tab.callback_run_simulation()

	def callback_stop_simulation(self):
		self.myserver.killall()
