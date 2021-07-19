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

## @package ribbon_file
#  A ribbon for the file menu
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import  QAction, QMenu
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QToolButton
from PyQt5.QtWidgets import QTabWidget

import webbrowser
from lock import get_lock
from QAction_lock import QAction_lock
from used_files import used_files_load
from PyQt5.QtCore import pyqtSignal

from gpvdm_open import gpvdm_open
from cal_path import get_backup_path
from cal_path import get_sim_path

from util import wrap_text

from util_zip import write_lines_to_archive
from gui_util import dlg_get_text
from backup import backup
from scripts import scripts
from cite_me import cite_me
from ribbon_page import ribbon_page

class ribbon_file(ribbon_page):
	used_files_click= pyqtSignal(str)
	def __init__(self):
		ribbon_page.__init__(self)
		
		self.home_new = QAction_lock("document-new", _("New simulation").replace(" ","\n"), self,"main_new")
		#self.home_new.setText(_("New\nsimulation"))
		self.addAction(self.home_new)

		self.old = QAction(icon_get("document-new"), _("New simulation").replace(" ","\n"), self)


		self.home_open = QAction_lock("document-open", _("Open\nsimulation"), self,"main_open")

		self.used_files_menu = QMenu(self)
		self.populate_used_file_menu()
		self.home_open.setMenu(self.used_files_menu)


		self.addAction(self.home_open)

		self.home_backup = QAction_lock("backup", _("Backup\nSimulaion"), self,"ribbion_db_backup")
		self.home_backup.clicked.connect(self.callback_home_backup)
		self.addAction(self.home_backup)

		self.tb_script_editor = QAction_lock("script", _("Script\nEditor"), self,"script_editor")
		self.tb_script_editor.clicked.connect(self.callback_script)
		self.addAction(self.tb_script_editor)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)

		self.cite_me=cite_me()
		self.addWidget(self.cite_me)

		self.home_help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		self.addAction(self.home_help)


	def populate_used_file_menu(self):
		self.used_files_menu.clear()
		files=used_files_load()
		for f in files:
			f=QAction(f, self)
			f.triggered.connect(self.callback_menu)
			self.used_files_menu.addAction(f)

	def callback_menu(self):
		action = self.sender()
		self.used_files_click.emit(action.text())

	def update(self):
		self.populate_used_file_menu()


	def setEnabled(self,val,do_all=False):
		self.home_new.setEnabled(val)
		self.home_open.setEnabled(val)
		self.home_backup.setEnabled(val)

	def setEnabled_other(self,val):
		self.home_backup.setEnabled(val)
		self.tb_script_editor.setEnabled(val)

	def on_new_backup(self):
		new_backup_name=dlg_get_text( _("New backup:"), _("New backup name"),"add_backup")
		new_backup_name=new_backup_name.ret
		if new_backup_name!=None:
			new_backup=os.path.join(self.dialog.viewer.path,new_backup_name)
			backup(new_backup,get_sim_path())
			self.dialog.viewer.fill_store()

	def callback_home_backup(self):
		backup_path=get_backup_path()
		if os.path.isdir(backup_path)==False:
			os.makedirs(backup_path)

		lines=[]
		lines.append("#gpvdm_file_type")
		lines.append("backup_main")
		lines.append("#end")

		write_lines_to_archive(os.path.join(backup_path,"sim.gpvdm"),"mat.inp",lines,mode="l",dest="file")

		self.dialog=gpvdm_open(backup_path,big_toolbar=True)
		self.new_backup = QAction_lock("add_backup", wrap_text(_("New backup"),2), self,"add_backup")
		self.new_backup.clicked.connect(self.on_new_backup)
		self.dialog.toolbar.addAction(self.new_backup)

		self.dialog.viewer.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_script(self):

		self.scripts=scripts()

		self.scripts.show()


