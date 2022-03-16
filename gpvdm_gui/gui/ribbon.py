# -*- coding: utf-8 -*-
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

## @package ribbon
#  Main ribbon class for main window.
#


import os

from gpvdm_local import gpvdm_local

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QLabel,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

#from ribbon_device import ribbon_device
from ribbon_database import ribbon_database
from ribbon_simulations import ribbon_simulations
from ribbon_electrical import ribbon_electrical
from ribbon_optical import ribbon_optical
from ribbon_information import ribbon_information
from ribbon_sim_mode import ribbon_sim_mode
from icon_lib import icon_get

from about import about_dlg

from ribbon_cluster import ribbon_cluster
from css import css_apply

from status_icon import status_icon_stop
from global_objects import global_object_get
from server import server_get

from connect_to_cluster import connect_to_cluster
from ribbon_base import ribbon_base
from error_dlg import error_dlg
from ribbon_file import ribbon_file
from PyQt5.QtCore import pyqtSignal
import webbrowser

from help import help_window
from lock import get_lock
from ribbon_thermal import ribbon_thermal

class QLabel_click(QLabel):
	clicked=pyqtSignal()
	def __init(self, parent):
		QLabel.__init__(self, QMouseEvent)

	def mousePressEvent(self, ev):
		self.clicked.emit()

class ribbon(ribbon_base):


	def update(self):
		#self.device.update()
		self.database.update()
		self.simulations.update()
		self.electrical.update()
		self.optical.update()
		self.information.update()
		#self.home.update()
		self.ribbon_sim_mode.update()
		self.thermal.update()


	def callback_about_dialog(self):
		dlg=about_dlg()
		dlg.exec_()

	def callback_questions(self):
		webbrowser.open("https://www.gpvdm.com/contact.html")

	def __init__(self):
		ribbon_base.__init__(self)
		self.cluster_tab=None
		#self.setMaximumHeight(140)

		#self.setStyleSheet("QWidget {	background-color:cyan; }")

		self.myserver=server_get()

		self.holder=QWidget()
		self.hbox=QHBoxLayout()
		self.holder.setLayout(self.hbox)
		self.toolbar=QToolBar()
		self.toolbar.setIconSize(QSize(32, 32))

		self.help_message=QLabel_click(_(get_lock().question+" <a href=\""+get_lock().my_email+"\">"+get_lock().my_email+"</a>"))
		self.help_message.clicked.connect(self.callback_questions)
		self.about = QToolButton(self)
		self.about.setText(_("About"))
		self.about.pressed.connect(self.callback_about_dialog)

		self.cluster_button = QAction(icon_get("not_connected"), _("Connect to cluster"), self)
		self.cluster_button.triggered.connect(self.callback_cluster_connect)
		self.toolbar.addAction(self.cluster_button)
		
		self.hbox.addWidget(self.help_message)
		self.hbox.addWidget(self.toolbar)
		self.hbox.addWidget(self.about)

		self.setCornerWidget(self.holder)

		self.file=ribbon_file()
		self.addTab(self.file,_("File"))
		
		self.ribbon_sim_mode=ribbon_sim_mode()
		self.addTab(self.ribbon_sim_mode,_("Simulation type"))
		
		
		self.simulations=ribbon_simulations()
		self.addTab(self.simulations,_("Simulation Editors"))
		self.simulations.experiments_changed.connect(self.ribbon_sim_mode.update)

		self.electrical=ribbon_electrical()
		self.addTab(self.electrical,_("Electrical"))

		self.optical=ribbon_optical()
		self.addTab(self.optical,_("Optical"))

		self.thermal=ribbon_thermal()
		self.addTab(self.thermal,_("Thermal"))

		self.database=ribbon_database()
		self.addTab(self.database,_("Databases"))

		self.tb_cluster=ribbon_cluster()
		if gpvdm_local().gui_config.enable_betafeatures==True:

			self.addTab(self.tb_cluster,_("Cluster"))

		self.information=ribbon_information()
		self.addTab(self.information,_("Information"))

		#self.setStyleSheet("QWidget {	background-color:cyan; }") 
		css_apply(self,"style.css")
		#self.setStyleSheet("background-color: rgb(255,0,0); margin:5px; border:1px solid rgb(0, 255, 0); ")
		#self.setStyleSheet(" margin: 0px;  padding:0px; border:0px; background-color: rgb(255,0,0); border:1px solid rgb(0, 255, 0);")
		self.currentChanged.connect(self.changed_click)

	def callback_cluster_connect(self):
		dialog=connect_to_cluster()
		if dialog.exec_():
			self.cluster_tab=global_object_get("cluster_tab")
			global_object_get("notebook_goto_page")(_("Terminal"))
			if self.myserver.cluster==False:
				if self.myserver.connect()==False:
					error_dlg(self,_("Can not connect to cluster."))
			else:
				self.myserver.cluster_disconnect()
				print("Disconnected")

		print(self.myserver.cluster)

		self.tb_cluster.update()
		if self.myserver.cluster==True:
			self.cluster_button.setIcon(icon_get("connected"))
			status_icon_stop(True)
		else:
			status_icon_stop(False)
			self.cluster_button.setIcon(icon_get("not_connected"))

	def changed_click(self):

		if self.tabText(self.currentIndex()).strip()==_("Simulation Editors"):
			help_window().help_set_help(["sunsvoc.png",_("<big><b>Simulation Editors</b></big><br> Use this tab to edit the simulation you wish to perform, you can choose from steady state measurments such as JV curve/Suns-Voc, time domain or frequency domain.  You can also choose to use advanced optical models to understand your data.")])

		if self.tabText(self.currentIndex()).strip()==_("Configure"):
			help_window().help_set_help(["preferences-system.png",_("<big><b>Configure</b></big><br> Use this tab to control advanced features of the simulation, such as finite difference mesh, amount of data written to disk, and the configuration of the back end numerical solvers.")])
		if self.tabText(self.currentIndex()).strip()==_("Simulation type"):
			help_window().help_set_help(["jv.png",_("<big><b>Simulation type</b></big><br> Use this tab to select which simulation mode the model runs in, select between steady state, time domain, frequency domain and optical simulations.")])

		if self.tabText(self.currentIndex()).strip()==_("Databases"):
			help_window().help_set_help(["spectra_file.png",_("<big><b>Databases</b></big><br> Use this tab to explore the materials and optical data bases, you can add and download more materials/optical models using the tools here.")])

		if self.tabText(self.currentIndex()).strip()==_("Information"):
			help_window().help_set_help(["youtube.png",_("<big><b>Information</b></big><br> Access extra information about the model in this tab, there are lots of tutorial videos on YouTube, follow on Twitter for the latest updates.")])

