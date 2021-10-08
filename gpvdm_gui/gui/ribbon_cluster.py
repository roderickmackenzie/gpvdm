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

## @package ribbon_cluster
#  A ribbon containing clustering commands.
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

#windows
from help import help_window
from error_dlg import error_dlg
from server import server_get

from global_objects import global_object_run

from util import wrap_text
from ribbon_page import ribbon_page

class ribbon_cluster(ribbon_page):
	def __init__(self):
		ribbon_page.__init__(self)
		self.myserver=server_get()

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))

		self.cluster_get_data = QAction(icon_get("server_get_data"), wrap_text(_("Cluster get data"),8), self)
		self.cluster_get_data.triggered.connect(self.callback_cluster_get_data)
		self.addAction(self.cluster_get_data)
		self.cluster_get_data.setEnabled(False)

		self.cluster_copy_src = QAction(icon_get("server_copy_src"), wrap_text(_("Copy src to cluster"),8), self)
		self.cluster_copy_src.triggered.connect(self.callback_cluster_copy_src)
		self.addAction(self.cluster_copy_src)
		self.cluster_copy_src.setEnabled(False)

		self.cluster_make = QAction(icon_get("server_make"), wrap_text(_("Make on cluster"),6), self)
		self.cluster_make.triggered.connect(self.callback_cluster_make)
		self.addAction(self.cluster_make)
		self.cluster_make.setEnabled(False)

		self.cluster_clean = QAction(icon_get("server_clean"), wrap_text(_("Clean cluster"),8), self)
		self.cluster_clean.triggered.connect(self.callback_cluster_clean)
		self.addAction(self.cluster_clean)
		self.cluster_clean.setEnabled(False)

		self.cluster_off = QAction(icon_get("off"), wrap_text(_("Kill all cluster code"),8), self)
		self.cluster_off.triggered.connect(self.callback_cluster_off)
		self.addAction(self.cluster_off)
		self.cluster_off.setEnabled(False)


		self.cluster_sync = QAction(icon_get("sync"),  _("Sync"), self)
		self.cluster_sync.triggered.connect(self.callback_cluster_sync)
		self.addAction(self.cluster_sync)
		self.cluster_sync.setEnabled(False)

		self.config_cluster = QAction(icon_get("server"), _("Cluster"), self)
		self.config_cluster.setStatusTip(_("Cluster"))
		self.config_cluster.triggered.connect(self.callback_configure_cluster)
		self.config_cluster.setEnabled(False)
		self.addAction(self.config_cluster)


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)
		
		self.help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		self.addAction(self.help)
		
	def callback_cluster_make(self):
		self.myserver.cluster_make()

	def callback_cluster_clean(self):
		self.myserver.cluster_clean()

	def callback_cluster_off(self):
		self.myserver.cluster_quit()
		self.update()

	def callback_cluster_sync(self):
		self.myserver.copy_src_to_cluster_fast()

	def callback_cluster_sleep(self):
		self.myserver.sleep()

	def callback_cluster_poweroff(self):
		self.myserver.poweroff()

	def callback_cluster_print_jobs(self):
		self.myserver.print_jobs()

	def callback_wol(self, widget, data):
		self.myserver.wake_nodes()

			
	def update(self):
		if self.myserver.cluster==True:
			self.cluster_clean.setEnabled(True)
			self.cluster_make.setEnabled(True)
			self.cluster_copy_src.setEnabled(True)
			self.cluster_get_data.setEnabled(True)
			self.cluster_off.setEnabled(True)
			self.cluster_sync.setEnabled(True)
			self.config_cluster.setEnabled(True)

		else:
			self.cluster_clean.setEnabled(False)
			self.cluster_make.setEnabled(False)
			self.cluster_copy_src.setEnabled(False)
			self.cluster_get_data.setEnabled(False)
			self.cluster_off.setEnabled(False)
			self.cluster_sync.setEnabled(False)
			self.config_cluster.setEnabled(False)

	def setEnabled(self,val):
		print("")
		#self.undo.setEnabled(val)

	def callback_cluster_get_data(self, widget, data=None):
		self.myserver.cluster_get_data()

	def callback_cluster_copy_src(self, widget, data=None):
		self.myserver.copy_src_to_cluster()

	def callback_configure_cluster(self):
		from cluster_config_window import cluster_config_window
		self.win=cluster_config_window(self)
		self.win.show()

