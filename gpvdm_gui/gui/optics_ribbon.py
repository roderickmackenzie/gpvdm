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
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton, QMenu
from PyQt5.QtWidgets import QTabWidget

from icon_lib import icon_get

from about import about_dlg

from util import wrap_text
from ribbon_base import ribbon_base
from play import play
from QAction_lock import QAction_lock
from gui_util import dlg_get_text
from generation_rate_editor import generation_rate_editor
from str2bool import str2bool
from gpvdm_json import gpvdm_data
from help import QAction_help

class mode_button(QAction_lock):
	def __init__(self,image,text,s,name):
		QAction_lock.__init__(self,image,text,s,name)
		self.setCheckable(True)
		self.mode="full"


class ribbon_optics_files(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))
		
		self.run = play(self,"optics_ribbon_run",run_text=wrap_text(_("Run optical simulation"),5))
		self.addAction(self.run)


		self.configwindow = QAction(icon_get("preferences-system"), _("Configure"), self)
		self.addAction(self.configwindow)

		self.build_action_group()

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)


		self.help = QAction_help()
		self.addAction(self.help)

	def build_action_group(self):
		self.actions=[]
		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))
		
		a=mode_button("transfer_matrix", _("Transfer\nmatrix"), self,"optics_ribbon_export_image")
		a.mode="full"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("exp_light", _("Exponential\nprofile"), self,"optics_ribbon_mode_exp")
		a.mode="exp"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("flat_light", _("Flat\nprofile"), self,"optics_ribbon_mode_flat")
		a.mode="flat"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("file_light", _("From\nfile"), self,"optics_ribbon_mode_from_file")
		a.mode="fromfile"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("qe_light", _("From QE\nspectra"), self,"optics_ribbon_mode_qe")
		a.mode="qe"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("constant_light", _("Constant\nvalue"), self,"optics_ribbon_mode_constant")
		a.mode="constant"
		a.clicked.connect(self.callback_click)

		self.menu_set_constant_value = QMenu(self)
		#self.populate_used_file_menu()
		a.setMenu(self.menu_set_constant_value)

		f=QAction(_("Edit constant"), self)
		f.triggered.connect(self.callback_edit_constant)
		self.menu_set_constant_value.addAction(f)


		self.actions.append(a)

		for a in self.actions:
			self.addAction(a)

		self.set_mode()

	def callback_edit_constant(self):
		self.generation_rate_editor=generation_rate_editor()
		self.generation_rate_editor.show()

	def set_mode(self):
		self.blockSignals(True)
		data=gpvdm_data()
		used_model=data.light.light_model
		for a in self.actions:
			a.setChecked(False)
			if a.mode==used_model:
				a.setChecked(True)
				break

		self.blockSignals(False)

	def callback_click(self,w):
		self.blockSignals(True)
		data=gpvdm_data()

		for a in self.actions:
			a.setChecked(False)

		w.setChecked(True)

		data.light.light_model=w.mode
		data.save()

		self.blockSignals(False)

class optics_ribbon(ribbon_base):

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

		self.optics=ribbon_optics_files()
		self.addTab(self.optics,_("Optics"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

		self.update()
