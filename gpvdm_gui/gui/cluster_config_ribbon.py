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

## @package cluster_config_ribbon
#  A ribbon used to configure the cluster.
#

import os

from code_ctrl import enable_betafeatures
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from icon_lib import icon_get

from about import about_dlg

from util import wrap_text

from ribbon_base import ribbon_base

from order_widget import order_widget

class cluster_config_ribbon(ribbon_base):
		
	def upload(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))


		self.generate_keys = QAction(icon_get("gnome-dialog-password"), _("Generate keys"), self)
		self.generate_keys.setStatusTip(_("Generate keys"))
		toolbar.addAction(self.generate_keys)

		self.install = QAction(icon_get("install-to-cluster"), _("Install to cluster"), self)
		self.install.setStatusTip(_("Install to cluster"))
		toolbar.addAction(self.install)

		self.boot = QAction(icon_get("run-cluster"), _("Boot cluster"), self)
		self.boot.setStatusTip(_("Boot cluster"))
		toolbar.addAction(self.boot)

		self.stop = QAction(icon_get("stop-cluster"), _("Stop cluster"), self)
		self.stop.setStatusTip(_("Stop cluster"))
		toolbar.addAction(self.stop)

		self.remove = QAction(icon_get("remove-from-cluster"), _("Remove from cluster"), self)
		self.remove.setStatusTip(_("Remove from cluster"))
		toolbar.addAction(self.remove)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.home_help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		toolbar.addAction(self.home_help)

		return toolbar

	def clusters(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.order_widget=order_widget()
		self.order_widget.new_text=_("New cluster configuration")
		self.order_widget.delete_text=_("Delete configuration")
		self.order_widget.clone_text=_("Clone configuration")
		self.order_widget.rename_text=_("Rename configuration")
		self.order_widget.new_dlg_text=_("New cluster configuration:")
		self.order_widget.base_file_name="cluster"
		self.order_widget.new_tab_name=_("Cluster")
		self.order_widget.clone_dlg_text=_("Clone the current cluster configuration to a cluster configuration called:")
		self.order_widget.rename_dlg_text=_("Rename the cluster configuration to be called:")
		self.order_widget.delete_dlg_text=_("Should I remove the cluster configuration ")
		self.order_widget.init()

		toolbar.addWidget(self.order_widget)

		return toolbar


	def __init__(self):
		ribbon_base.__init__(self)
		self.setMaximumHeight(130)
		#self.setStyleSheet("QWidget {	background-color:cyan; }")

		w=self.upload()
		self.addTab(w,_("SSH"))
		
		w=self.clusters()
		self.addTab(w,_("Clusters"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

