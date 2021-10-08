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

## @package scan_ribbon
#  The ribbon for the scan window.
#

import os

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from icon_lib import icon_get

from about import about_dlg

from util import wrap_text

from gpvdm_local import gpvdm_local

from ribbon_base import ribbon_base
from play import play

class scan_ribbon(ribbon_base):
		
	def scan(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.tb_new = QAction(icon_get("document-new"), wrap_text(_("New scan"),2), self)
		toolbar.addAction(self.tb_new)

		self.tb_delete = QAction(icon_get("edit-delete"), wrap_text(_("Delete scan"),3), self)
		toolbar.addAction(self.tb_delete)

		self.tb_clone = QAction(icon_get("clone"), wrap_text(_("Clone scan"),3), self)
		toolbar.addAction(self.tb_clone)

		self.tb_rename = QAction(icon_get("rename"), wrap_text(_("Rename scan"),3), self)
		toolbar.addAction(self.tb_rename)

		self.tb_clean = QAction(icon_get("clean"), wrap_text(_("Clean all"),4), self)
		toolbar.addAction(self.tb_clean)

		toolbar.addSeparator()

		self.tb_run_all =  play(self,"scan_play_all",play_icon="forward2",run_text=wrap_text(_("Run all scans"),5))#QAction(icon_get("forward2"), wrap_text(_("Run all scans"),3), self)
		toolbar.addAction(self.tb_run_all)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.home_help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		toolbar.addAction(self.home_help)

		return toolbar


	def advanced(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.menu_plot_fits = QAction(icon_get("scan2"), wrap_text(_("Plot fits"),5), self)
		toolbar.addAction(self.menu_plot_fits)

		self.sim_no_gen = QAction(icon_get("forward"), wrap_text(_("Run simulation no generation"),5), self)
		toolbar.addAction(self.sim_no_gen)

		self.single_fit = QAction(icon_get("forward"), wrap_text(_("Run single fit"),5), self)
		toolbar.addAction(self.single_fit)

		self.clean_unconverged = QAction(icon_get("clean"), wrap_text(_("Clean unconverged simulation"),5), self)
		toolbar.addAction(self.clean_unconverged)

		self.clean_sim_output = QAction(icon_get("forward"), wrap_text(_("Clean simulation output"),5), self)
		toolbar.addAction(self.clean_sim_output)

		self.push_unconverged_to_hpc = QAction(icon_get("forward"), wrap_text(_("Push unconverged to hpc"),5), self)
		toolbar.addAction(self.push_unconverged_to_hpc)

		self.change_dir = QAction(icon_get("forward"), wrap_text(_("Change dir"),5), self)
		toolbar.addAction(self.change_dir)

		self.report = QAction(icon_get("office-calendar"), wrap_text(_("Report"),5), self)
		toolbar.addAction(self.report)

		return toolbar

	def update(self):
		print("update")
		#self.device.update()
		#self.simulations.update()
		#self.configure.update()
		#self.home.update()

	def callback_about_dialog(self):
		dlg=about_dlg()
		dlg.exec_()

	def __init__(self):
		ribbon_base.__init__(self)
		self.setMaximumHeight(130)
		#self.setStyleSheet("QWidget {	background-color:cyan; }")

		self.about = QToolButton(self)
		self.about.setText(_("About"))
		self.about.pressed.connect(self.callback_about_dialog)

		self.setCornerWidget(self.about)

		w=self.scan()
		self.addTab(w,_("Scan"))
		
		#w=self.simulations()
		#self.addTab(w,_("Simulations"))


		w=self.advanced()
		if gpvdm_local().gui_config.enable_betafeatures==True:
			self.addTab(w,_("Advanced"))


		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

