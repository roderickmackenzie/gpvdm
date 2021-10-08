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

## @package gpvdm_notebook
#  The main gpvdm notebook for the main window.
#


import os
import random

#tabs
from tab_main import tab_main
from tab import tab_class

#paths
from cal_path import get_exe_args
from cal_path import get_html_path
from cal_path import get_exe_command

#qt
from PyQt5.QtWidgets import QTabWidget,QWidget

#window
from tab_terminal import tab_terminal


from help import help_window

import i18n
_ = i18n.language.gettext

from global_objects import global_object_register
from cal_path import get_sim_path

from tab_view import tab_view

from css import css_apply
from circuit_editor import circuit_editor
from display_mesh import display_mesh
from gpvdm_json import gpvdm_data
from gpvdm_local import gpvdm_local

if gpvdm_local().gui_config.enable_webbrowser==True:
	from information_webkit import information
else:
	from information_noweb import information

class gpvdm_notebook(QTabWidget):
	item_factory=None


	def __init__(self):
		QWidget.__init__(self)
		css_apply(self,"tab_default.css")
		self.terminal=None
		self.update_display_function=None
		self.currentChanged.connect(self.changed_click)
		global_object_register("notebook_goto_page",self.goto_page)
		self.state_loaded=False
		self.display_mesh=None



	def update(self):
		for i in range(0,self.count()):
			w=self.widget(i)
			w.update()

	def changed_click(self):

		if self.tabText(self.currentIndex()).strip()==_("Device structure"):
			help_window().help_set_help(["device.png",_("<big><b>The device structure tab</b></big><br> Use this tab to change the structure of the device, the layer thicknesses and to perform optical simulations.  You can also browse the materials data base and  edit the electrical mesh.")])

		if self.tabText(self.currentIndex()).strip()==_("Electrical parameters"):
			help_window().help_set_help(["tab.png",_("<big><b>The electrical parameters</b></big><br>This tab contains the electrical model parameters, such as mobility, tail slope energy, and band gap.")])

		if self.tabText(self.currentIndex()).strip()==_("Terminal"):
			help_window().help_set_help(["utilities-terminal.png",_("<big><b>The terminal window</b></big><br>The output of the model will be displayed in this window, watch this screen for debugging and convergence information.")])

		if self.tabText(self.currentIndex()).strip()==_("Tutorials/Documentation"):
			help_window().help_set_help(["help.png",_("<big><b>Tutorials/Documentation</b></big><br>Here you can find tutorials, videos, worksheets, and talks about gpvdm and simulating opto-electronic devices in general.  Although gpvdm is easy to use with it's friendly interface, under the hood gpvdm is a highly complex model and powerful model, it is worth taking time to read the documentation to understand it.  This will enable you to get the most out of gpvdm and your experimental data."),"youtube",_("<big><b><a href=\"https://youtu.be/XBbaogu61Ps\">Watch the video</a></b></big><br> Performing my first solar cell simulation")])

		if self.tabText(self.currentIndex()).strip()==_("Output"):
			help_window().help_set_help(["dat_file.png",_("<big><b>Output</b></big><br>This shows the root simulation directory, this is where the results are stored.  Double click on a file to see what is in it..")])

	def get_current_page(self):
		i=self.currentIndex()
		return self.tabText(i)


	def goto_page(self,page):
		self.blockSignals(True)
		for i in range(0,self.count()):
				if self.tabText(i)==page:
					self.setCurrentIndex(i)
					break
		self.blockSignals(False)

	def add_info_page(self):
		files=os.listdir(get_html_path())
		info_files=[]
		for i in range(0,len(files)):
			if files[i].startswith("info") and files[i].endswith("html"):
				info_files.append(files[i])
		file_name=random.choice(info_files)


		browser=information(file_name)

		self.addTab(browser,_("Information"))

	def add_docs_page(self):
		browser=information("docs.html")

		self.addTab(browser,_("Tutorials/Documentation"))

	def load(self):
		self.clear()
		self.last_page=0

		#self.setTabsClosable(True)
		self.setMovable(True)
		if (os.path.isfile(os.path.join(get_sim_path(),"sim.gpvdm"))==True):

			self.tab_main=tab_main()
			self.addTab(self.tab_main,_("Device structure"))

			self.update_circuit_window()

			self.update_display_function=self.tab_main.update
			#self.tab_main.three_d.display.force_redraw()

			self.terminal=tab_terminal()
			self.terminal.init()
			self.addTab(self.terminal,_("Terminal"))
			self.terminal.run(os.getcwd(),get_exe_command()+" --version2 "+get_exe_args())
			global_object_register("terminal",self.terminal)

			widget=tab_view()
			self.addTab(widget,_("Output"))
			self.add_docs_page()
			self.state_loaded=True

		else:
			self.add_info_page()
			self.goto_page(_("Information"))
			self.state_loaded=False

	def update_circuit_window(self):
		data=gpvdm_data()
		mesh_x=data.mesh.mesh_x
		mesh_z=data.mesh.mesh_z

		next_tab=None
		if data.electrical_solver.solver_type=="circuit":
			if  mesh_x.get_points()==1 and mesh_z.get_points()==1:
				next_tab=circuit_editor()
			else:
				next_tab=display_mesh()

		if self.display_mesh!=next_tab:
			if self.display_mesh!=None:
				index=self.indexOf(self.display_mesh)
				self.removeTab(index)
			self.display_mesh=next_tab
			if self.display_mesh!=None:
				self.insertTab(1,self.display_mesh,_("Circuit diagram"))

	def is_loaded(self):
		return self.state_loaded

	def setEnabled(self,val):
		for i in range(0,self.count()):
			w=self.widget(i)
			w.setEnabled(val)


