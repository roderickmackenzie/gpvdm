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

## @package optics_ribbon
#  The ribbon for the optics window
#


import os

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
from play import play
from QAction_lock import QAction_lock
from cal_path import get_sim_path
import webbrowser
from gpvdm_open import gpvdm_open
from inp import inp
from cal_path import gpvdm_paths
from help import QAction_help

class mode_button(QAction_lock):
	def __init__(self,image,text,s,name):
		QAction_lock.__init__(self,image,text,s,name)
		self.setCheckable(True)
		self.mode="full"



class script_ribbon(ribbon_base):

	def file(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(48, 48))

		self.tb_new = QAction_lock("document-new", _("New"), self,"ribbion_script_new")
		toolbar.addAction(self.tb_new)

		self.tb_rename = QAction_lock("rename", _("Rename"), self,"ribbion_script_rename")
		toolbar.addAction(self.tb_rename)

		self.run = play(self,"scripts_ribbon_run",run_text=wrap_text(_("Run"),3))
		toolbar.addAction(self.run)

		self.tb_save = QAction_lock("document-save", _("Save"), self,"ribbion_script_save")
		toolbar.addAction(self.tb_save)

		self.plot = QAction_lock("plot", _("Plot\nFile"), self,"ribbon_script_plot")
		self.plot.clicked.connect(self.callback_plot_select)
		toolbar.addAction(self.plot)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.hashtag = QAction(icon_get("json_python"), _("Data\nexplorer"), self)
		toolbar.addAction(self.hashtag)
		self.hashtag.triggered.connect(self.callback_hashtag_dict)

		self.hashtag = QAction(icon_get("json_file"), _("View raw\njson"), self)
		toolbar.addAction(self.hashtag)
		self.hashtag.triggered.connect(self.callback_view_json)

		self.help = QAction_help()
		toolbar.addAction(self.help)
		return toolbar

	def callback_plot_select(self):
		dialog=gpvdm_open(get_sim_path(),show_inp_files=False)
		dialog.show_directories=False
		ret=dialog.exec_()		

	def update(self):
		return

	def callback_about_dialog(self):
		dlg=about_dlg()
		dlg.exec_()

	def __init__(self):
		QTabWidget.__init__(self)
		#self.setStyleSheet("QWidget {	background-color:cyan; }")

		self.about = QToolButton(self)
		self.about.setText(_("About"))
		self.about.pressed.connect(self.callback_about_dialog)

		self.setCornerWidget(self.about)

		w=self.file()
		self.addTab(w,_("File"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

	def callback_hashtag_dict(self):
		from window_json_viewer import window_json_viewer
		self.w=window_json_viewer()
		self.w.show()

	def callback_view_json(self):
		f=inp()
		f.load(os.path.join(get_sim_path(),"sim.json"))
		f.save_as(os.path.join(gpvdm_paths.get_tmp_path(),"sim.json"),dest="file")
		webbrowser.open(os.path.join(gpvdm_paths.get_tmp_path(),"sim.json"))

