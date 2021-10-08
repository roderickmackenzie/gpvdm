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

## @package ribbon_spectra
#  A ribbon for the materials window.
#


import os

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from win_lin import desktop_open

#windows

from icon_lib import icon_get

from cal_path import get_sim_path
from util import wrap_text

from ribbon_base import ribbon_base

from QAction_lock import QAction_lock


class ribbon_spectra(ribbon_base):
	def main_toolbar(self):
		self.main_toolbar = QToolBar()
		self.main_toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.main_toolbar.setIconSize(QSize(42, 42))

		self.import_data= QAction_lock("import", _("From\nFile"), self,"ribbon_spectra_import")
		self.main_toolbar.addAction(self.import_data)

		self.tb_ref= QAction(icon_get("ref"), wrap_text(_("Insert reference information"),8), self)
		self.main_toolbar.addAction(self.tb_ref)

		#self.tb_save = QAction(icon_get("document-save-as"), _("Save image"), self)
		#self.main_toolbar.addAction(self.tb_save)


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.main_toolbar.addWidget(spacer)

		self.help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		self.main_toolbar.addAction(self.help)


		return self.main_toolbar


	def __init__(self):
		ribbon_base.__init__(self)

		w=self.main_toolbar()
		self.addTab(w,_("File"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

