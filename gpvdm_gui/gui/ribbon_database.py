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

## @package ribbon_database
#  The database ribbon.
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from doping import doping_window
from contacts import contacts_window
from cal_path import get_materials_path
from gpvdm_open import gpvdm_open

from help import help_window
from cost import cost


from parasitic import parasitic
from cal_path import get_spectra_path

from util import wrap_text
from gui_util import dlg_get_text

from cal_path import get_emission_path

from error_dlg import error_dlg

from QAction_lock import QAction_lock
from cal_path import get_user_data_path
import webbrowser
from cal_path import gpvdm_paths

from lock import get_lock
from ribbon_page import ribbon_page
from gpvdm_json import gpvdm_data
from json_base import json_base
from cal_path import get_sim_path

class ribbon_database(ribbon_page):
	def __init__(self):
		ribbon_page.__init__(self)

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))
		
		self.materials = QAction_lock("organic_material", _("Materials\ndatabase"), self,"ribbion_db_materials")
		self.materials.clicked.connect(self.callback_view_materials)
		self.addAction(self.materials)

		self.spectra_file = QAction_lock("spectra_file", _("Optical\ndatabase"), self,"ribbion_db_spectra")
		self.spectra_file.clicked.connect(self.callback_view_optical)
		self.addAction(self.spectra_file)

		self.emission = QAction_lock("pl", _("Emission\ndatabase"), self,"ribbion_db_emission")
		self.emission.clicked.connect(self.callback_configure_emission)
		self.addAction(self.emission)

		self.user = QAction_lock("folder", _("User\ndata"), self,"ribbion_db_user_data")
		self.user.clicked.connect(self.callback_configure_user_data)
		self.addAction(self.user)

		self.shape = QAction_lock("shape", _("Shape\ndatabase"), self,"ribbion_db_shape")
		self.shape.clicked.connect(self.callback_configure_shape)
		self.addAction(self.shape)

		self.filters = QAction_lock("filter_wheel", _("Filters\ndatabase"), self,"ribbion_db_materials")
		self.filters.clicked.connect(self.callback_view_filters)
		self.addAction(self.filters)

		self.home_backup = QAction_lock("backup", _("Backup\nSimulaion"), self,"ribbion_db_backup")
		self.home_backup.clicked.connect(self.callback_home_backup)
		self.addAction(self.home_backup)

		self.solar = QAction_lock("weather-few-clouds", _("Solar spectrum\ngenerator"), self,"solar_spectrum_tool")
		self.solar.clicked.connect(self.callback_solar)
		if get_lock().is_gpvdm_next()==True:
			self.addAction(self.solar)

	def update(self):
		pass


	def setEnabled(self,val):
		self.materials.setEnabled(val)
		self.spectra_file.setEnabled(val)
		self.emission.setEnabled(val)
		self.user.setEnabled(val)
		self.shape.setEnabled(val)
		self.home_backup.setEnabled(val)


	def on_new_shape_clicked(self):
		self.dialog.viewer.new_shape()

	def callback_view_materials(self):
		self.dialog=gpvdm_open(get_materials_path(),big_toolbar=True)
		self.new_materials = QAction_lock("add_material", wrap_text(_("Add Material"),8), self,"add_material")
		self.new_materials.clicked.connect(self.dialog.viewer.new_material)
		self.dialog.toolbar.addAction(self.new_materials)

		self.dialog.show_inp_files=False
		self.dialog.menu_new_material_enabled=True
		ret=self.dialog.exec_()

	def callback_view_filters(self):
		self.dialog=gpvdm_open(gpvdm_paths.get_filters_path(),big_toolbar=True)
		self.new_materials = QAction_lock("add_filter", wrap_text(_("Add Filter"),8), self,"add_filter")
		self.new_materials.clicked.connect(self.dialog.viewer.new_filter)
		self.dialog.toolbar.addAction(self.new_materials)

		self.dialog.show_inp_files=False
		self.dialog.menu_new_material_enabled=True
		ret=self.dialog.exec_()

	def callback_view_optical(self):
		self.dialog=gpvdm_open(get_spectra_path(),big_toolbar=True)
		self.new_materials = QAction_lock("add_spectra", wrap_text(_("Add Spectra"),8), self,"add_spectra")
		self.new_materials.clicked.connect(self.dialog.viewer.new_spectra)
		self.dialog.toolbar.addAction(self.new_materials)
		self.dialog.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_update_window(self):
		webbrowser.open('http://www.gpvdm.com/download_materials.php')

	def callback_configure_emission(self):
		self.dialog=gpvdm_open(get_emission_path(),big_toolbar=True)
		self.new_emission = QAction_lock("add_emission", wrap_text(_("Add Spectra"),8), self,"add_emission")
		self.new_emission.clicked.connect(self.dialog.viewer.new_emission)
		self.dialog.toolbar.addAction(self.new_emission)
		self.dialog.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_configure_shape(self):
		self.dialog=gpvdm_open(gpvdm_paths.get_shape_path(),big_toolbar=True)
		self.new_shape = QAction_lock("add_shape", wrap_text(_("Add Shape"),8), self,"add_shape")
		self.new_shape.clicked.connect(self.on_new_shape_clicked)
		self.dialog.toolbar.addAction(self.new_shape)
		self.dialog.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_configure_user_data(self):
		self.dialog=gpvdm_open(get_user_data_path(),big_toolbar=True)
		ret=self.dialog.exec_()

	def callback_solar(self):
		from solar_spectrum_gen_window import solar_spectrum_gen_window
		self.solar_window=solar_spectrum_gen_window()

		self.solar_window.show()

	def callback_home_backup(self):
		from cal_path import get_backup_path
		backup_path=get_backup_path()
		if os.path.isdir(backup_path)==False:
			os.makedirs(backup_path)

		data=json_base("backup")
		data.include_name=False
		data.var_list.append(["icon","backup"])
		data.var_list.append(["item_type","backup_main"])
		data.var_list.append(["hidden","True"])
		data.var_list_build()

		data.save_as(os.path.join(backup_path,"data.json"))

		self.dialog=gpvdm_open(backup_path,big_toolbar=True)
		self.new_backup = QAction_lock("add_backup", wrap_text(_("New backup"),2), self,"add_backup")
		self.new_backup.clicked.connect(self.on_new_backup)
		self.dialog.toolbar.addAction(self.new_backup)

		self.dialog.viewer.show_inp_files=False
		ret=self.dialog.exec_()

	def on_new_backup(self):
		from backup import backup
		new_backup_name=dlg_get_text( _("New backup:"), _("New backup name"),"add_backup")
		new_backup_name=new_backup_name.ret
		if new_backup_name!=None:
			new_backup=os.path.join(self.dialog.viewer.path,new_backup_name)
			backup(new_backup,get_sim_path())
			self.dialog.viewer.fill_store()
