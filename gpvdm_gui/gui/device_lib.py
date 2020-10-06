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

## @package device_lib
#  A window to show the device lib.
#


import os
import glob
from cal_path import get_device_lib_path
from util_zip import read_lines_from_archive
from inp_util import inp_search_token_value_multiline
from util_zip import zip_lsdir
from util_zip import archive_add_file

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog, QFileDialog, QToolBar, QMessageBox, QLineEdit,QVBoxLayout, QTableWidget, QAbstractItemView

#gui_util

from gui_util import yes_no_dlg

class device_lib_class(QDialog):


	def create_model(self):

		self.tab.clear()
		self.tab.setColumnCount(3)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("File name"), _("Device type"), _("Description")])
		self.tab.setColumnWidth(0, 150);
		self.tab.setColumnWidth(2, 500);
		self.tab.verticalHeader().setDefaultSectionSize(80);
		files=glob.glob(os.path.join(get_device_lib_path(),"*.gpvdm"))
		for i in range(0,len(files)):
			print("working on",files[i],zip_lsdir(files[i]))
			lines=read_lines_from_archive(lines,files[i],"info.inp")
			if lines!=False:
				self.tab.add([os.path.basename(files[i]), " ".join(inp_search_token_value_multiline(lines,"#info_device_type"))," ".join(inp_search_token_value_multiline(lines,"#info_description"))])


	def __init__(self):
		self.file_path=""
		QWidget.__init__(self)
		self.main_vbox=QVBoxLayout()
		self.setFixedSize(800,400) 
		self.tab = QTableWidget()
		self.tab.setWordWrap(True)
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)
		self.create_model()

#		self.tab.cellChanged.connect(self.tab_changed)

		self.main_vbox.addWidget(self.tab)

		self.hwidget=QWidget()

		okButton = QPushButton("OK")
		cancelButton = QPushButton("Cancel")

		hbox = QHBoxLayout()
		hbox.addStretch(1)
		hbox.addWidget(okButton)
		hbox.addWidget(cancelButton)

		self.hwidget.setLayout(hbox)

		self.main_vbox.addWidget(self.hwidget)

		self.setLayout(self.main_vbox)

		okButton.clicked.connect(self.on_ok_clicked) 
		cancelButton.clicked.connect(self.close)
		
		return


	def on_ok_clicked(self):
		ret=self.tab.get_selected()
		if ret==False:
			error_dlg(self,_("You have not selected anything"))
		else:
			file_path=os.path.join(get_device_lib_path(),ret[0])
			md = yes_no_dlg(self,"You are about to import the simulation file "+file_path+" into this simulation.  The result will be that all model parameters will be overwritten.  Do you really want to do that?")

			if response == True:
				self.file_path=file_path
				self.close()
