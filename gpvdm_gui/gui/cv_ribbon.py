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

## @package fx_ribbon
#  A ribbon for the fx editor window
#

import os

from dump_io import dump_io

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

from code_ctrl import enable_betafeatures

from tb_lasers import tb_lasers

from ribbon_base import ribbon_base
from order_widget import order_widget

class cv_ribbon(ribbon_base):
		
	def experiment(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.order_widget=order_widget()
		self.order_widget.new_text=_("New experiment")
		self.order_widget.delete_text=_("Delete experiment")
		self.order_widget.clone_text=_("Clone experiment")
		self.order_widget.rename_text=_("Rename experiment")
		self.order_widget.new_dlg_text=_("New experiment name:")
		self.order_widget.base_file_name=["cv","cv_fxdomain_data"]
		self.order_widget.new_tab_name=_("experiment ")
		self.order_widget.clone_dlg_text=_("Clone the current experiment to a new experiment called:")
		self.order_widget.rename_dlg_text=_("Rename the experiment to be called:")
		self.order_widget.delete_dlg_text=_("Should I remove the experiment file ")
		self.order_widget.postfix="@cv"
		self.order_widget.name_token="#sim_menu_name"
		self.order_widget.init()

		toolbar.addWidget(self.order_widget)



		#self.tb_save = QAction(icon_get(("document-save")), wrap_text(_("Save image"),3), self)
		#toolbar.addAction(self.tb_save)



		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.home_help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		toolbar.addAction(self.home_help)

		return toolbar


	def update(self):
		print("update")


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

		w=self.experiment()
		self.addTab(w,_("Experiment"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

