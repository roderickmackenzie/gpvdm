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

## @package gpvdm_open
#  A file viewer which uses the gpvdm_viewer widget.
#

import os
from dat_file import dat_file

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QMenu,QAbstractItemView,QAction,QToolBar,QDialog,QVBoxLayout,QDialog,QWidget,QLineEdit

#cal_path
from icon_lib import icon_get
from cal_path import get_ui_path

from help import help_window

from error_dlg import error_dlg

from gui_util import dlg_get_text
from gui_util import yes_no_dlg

from inp import inp_get_token_value
from util import isfiletype
from win_lin import desktop_open
from gpvdm_viewer import gpvdm_viewer

from util import wrap_text

COL_PATH = 0
COL_PIXBUF = 1
COL_IS_DIRECTORY = 2

import i18n
_ = i18n.language.gettext

#util
from util import latex_to_html

class gpvdm_open(QDialog):

	def __init__(self,path,show_inp_files=True,act_as_browser=True,big_toolbar=False,title=_("Open file")):
		QWidget.__init__(self)
		self.act_as_browser=act_as_browser
		self.show_inp_files=show_inp_files
		self.show_directories=True
		self.file_path=""
		self.vbox=QVBoxLayout()
		self.setLayout(self.vbox)

		self.toolbar=QToolBar()
		if big_toolbar==True:
			self.toolbar.setIconSize(QSize(42, 42))
			self.toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		#self.toolbar.setMaximumHeight(50)

		self.up = QAction(icon_get("go-up"), wrap_text(_("Back"),8), self)
		self.up.triggered.connect(self.on_up_clicked)
		self.toolbar.addAction(self.up)

		self.home = QAction(icon_get("user-home"), wrap_text(_("Home"),8), self)
		self.home.triggered.connect(self.on_home_clicked)
		self.toolbar.addAction(self.home)
	
		self.resize(800,500)
		self.path_widget=QLineEdit()
#		self.path_widget.setMinimumHeight(30)
#		self.path_widget.setStyleSheet("padding: 0; ")
		self.toolbar.addWidget(self.path_widget)



		self.setWindowTitle(title+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("folder"))
#		self.listwidget=QListWidget()
#		self.listwidget.setSelectionMode(QAbstractItemView.ExtendedSelection)
#		self.listwidget.setStyleSheet("margin: 0; padding: 0; ")
		self.vbox.addWidget(self.toolbar)

		if self.act_as_browser==False:
			self.viewer=gpvdm_viewer(path,open_own_files=False)
		else:
			self.viewer=gpvdm_viewer(path)

		self.viewer.set_directory_view(True)
		self.vbox.addWidget(self.viewer)

		self.root_dir= path

		self.path_widget.setText(path)

		self.viewer.path_changed.connect(self.change_path)

		if self.act_as_browser==False:
			self.viewer.accept.connect(self.callback_accept)

		self.change_path()
		self.show()

	def callback_accept(self):
		self.file_path=self.viewer.file_path
		self.accept()

	def get_icon(self, name):
		return icon_get(name+"_file")


	def get_filename(self):
		return self.file_path


	def on_home_clicked(self, widget):
		self.viewer.path = self.root_dir
		self.change_path()
		

	def change_path(self):
		self.path_widget.setText(self.viewer.path)

		self.viewer.fill_store()
		sensitive = True
		print(self.viewer.path,self.root_dir)
		if self.viewer.path == self.root_dir:
			sensitive = False

		self.up.setEnabled(sensitive)

	def on_up_clicked(self, widget):
		self.viewer.set_path(os.path.dirname(self.viewer.path))
		self.change_path()
		self.viewer.fill_store()

