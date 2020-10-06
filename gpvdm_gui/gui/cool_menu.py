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

## @package cool_menu
#  This was meant to be a drop down menu for the ribbon, but it is not finished. 
#

import os
from icon_lib import icon_get

from code_ctrl import enable_betafeatures
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon,QCursor
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice,QEvent
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget


class cool_menu(QToolBar):

	def __init__(self,parent):
		QToolBar.__init__(self)
		
		#aaa=self.readStyleSheet(os.path.join(get_css_path(),"menu.css"))
		#aaa=str(aaa,'utf-8')
		#self.setStyleSheet(aaa)
		
		self.setToolButtonStyle( Qt.ToolButtonTextBesideIcon)
		self.setOrientation(Qt.Vertical)
		#self.setWindowFlags(Qt.FramelessWindowHint|Qt.WindowStaysOnTopHint|Qt.WindowStaysOnTopHint)
		self.setIconSize(QSize(42, 42))

		self.configure_configwindow = QAction(icon_get("help"), _("Help window"), self)
		self.addAction(self.configure_configwindow)

		self.configure_configwindow = QAction(icon_get("help"), _("Manual"), self)
		self.addAction(self.configure_configwindow)

		self.configure_configwindow = QAction(icon_get("help"), _("License"), self)
		self.addAction(self.configure_configwindow)

		self.configure_configwindow = QAction(icon_get("help"), _("Youtube"), self)
		self.addAction(self.configure_configwindow)
		
		self.configure_configwindow = QAction(icon_get("help"), _("Citing the model"), self)
		self.addAction(self.configure_configwindow)

		self.configure_configwindow = QAction(icon_get("help"), _("About"), self)
		self.addAction(self.configure_configwindow)

		l=self.layout()
		for i in range(0,l.count()):
			l.itemAt(i).setAlignment(Qt.AlignLeft)
    

		self.installEventFilter(self)
		self.setWindowFlags(Qt.Popup)
		
		self.move(self.mapFromGlobal(QCursor.pos()))

	def eventFilter(self, object, event):
		if event.type()== QEvent.WindowDeactivate:
			self.setVisible(False)
			
		return False
			

