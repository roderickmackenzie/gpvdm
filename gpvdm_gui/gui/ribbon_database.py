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

## @package ribbon_database
#  The database ribbon.
#


import os
from icon_lib import icon_get

from dump_io import dump_io
from tb_item_sim_mode import tb_item_sim_mode
from tb_item_sun import tb_item_sun

from code_ctrl import enable_betafeatures
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
from clone_materials import clone_material

from cal_path import get_emission_path
from cal_path import get_base_emission_path

from cal_path import gpvdm_paths

from cal_path import get_base_spectra_path

from cal_path import get_shape_path

from clone import clone_spectra
from error_dlg import error_dlg

from lasers import lasers

from QAction_lock import QAction_lock
from cal_path import get_user_data_path
from ver import ver_ctrl
import webbrowser

class ribbon_database(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))
		
		self.materials = QAction_lock("organic_material", _("Materials\ndatabase"), self,"ribbion_db_materials")
		self.materials.clicked.connect(self.callback_view_materials)
		self.addAction(self.materials)

		self.spectra_file = QAction_lock("spectra_file", _("Optical\ndatabase"), self,"ribbion_db_spectra")
		self.spectra_file.clicked.connect(self.callback_view_optical)
		self.addAction(self.spectra_file)

		self.tb_update = QAction_lock("update", _("Download extra\nmaterials"), self,"ribbion_db_update")
		self.tb_update.clicked.connect(self.callback_update_window)
		if ver_ctrl.is_gpvdm_next()==True:
			self.addAction(self.tb_update)

		self.lasers = QAction_lock("lasers", _("Laser\ndatabase"), self,"ribbion_db_lasers")
		self.lasers.clicked.connect(self.callback_configure_lasers)
		self.addAction(self.lasers)

		self.emission = QAction_lock("pl", _("Emission\ndatabase"), self,"ribbion_db_emission")
		self.emission.clicked.connect(self.callback_configure_emission)
		self.addAction(self.emission)

		self.user = QAction_lock("folder", _("User\ndata"), self,"ribbion_db_user_data")
		self.user.clicked.connect(self.callback_configure_user_data)
		self.addAction(self.user)

		self.shape = QAction_lock("shape", _("Shape\ndatabase"), self,"ribbion_db_shape")
		self.shape.clicked.connect(self.callback_configure_shape)
		self.addAction(self.shape)

		self.lasers_window=None

	def update(self):
		if self.lasers_window!=None:
			del self.lasers_window
			self.lasers_window=None


	def setEnabled(self,val):
		self.materials.setEnabled(val)
		self.spectra_file.setEnabled(val)
		self.tb_update.setEnabled(val)
		self.lasers.setEnabled(val)
		self.emission.setEnabled(val)
		self.user.setEnabled(val)
		self.shape.setEnabled(val)



	def on_new_materials_clicked(self):
		new_sim_name=dlg_get_text( _("New material name:"), _("New material name"),"organic_material")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_material=os.path.join(self.dialog.viewer.path,new_sim_name)
			ret=clone_material(new_material,os.path.join(get_base_material_path(),"generic","air"))
			if ret==False:
				error_dlg(self,_("I cant write to:")+new_material+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))
			self.dialog.viewer.fill_store()

	def on_new_spectra_clicked(self):
		new_sim_name=dlg_get_text( _("New spectra name:"), _("New spectra name"),"add_spectra")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_spectra=os.path.join(self.dialog.viewer.path,new_sim_name)
			ret=clone_spectra(new_spectra,os.path.join(get_base_spectra_path(),"sun"))
			if ret==False:
				error_dlg(self,_("I cant write to:")+new_spectra+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))
			self.dialog.viewer.fill_store()

	def on_new_emission_clicked(self):
		new_sim_name=dlg_get_text( _("New emission spectra name:"), _("New emission spectra name"),"add_emission")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_emission=os.path.join(self.dialog.viewer.path,new_sim_name)
			ret=clone_material(new_emission,os.path.join(get_base_emission_path(),"Irppy3"))
			if ret==False:
				error_dlg(self,_("I cant write to:")+new_emission+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))
			self.dialog.viewer.fill_store()

	def on_new_shape_clicked(self):
		new_sim_name=dlg_get_text( _("New shape name:"), _("New shape name"),"add_shape")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_shape=os.path.join(self.dialog.viewer.path,new_sim_name)
			ret=clone_material(new_shape,gpvdm_paths.get_shape_template_path())
			if ret==False:
				error_dlg(self,_("I cant write to:")+new_shape+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))
			self.dialog.viewer.fill_store()

	def callback_view_materials(self):
		self.dialog=gpvdm_open(get_materials_path(),big_toolbar=True)
		self.new_materials = QAction_lock("add_material", wrap_text(_("Add Material"),8), self,"add_material")
		self.new_materials.clicked.connect(self.on_new_materials_clicked)
		self.dialog.toolbar.addAction(self.new_materials)

		self.dialog.show_inp_files=False
		self.dialog.menu_new_material_enabled=True
		ret=self.dialog.exec_()

	def callback_view_optical(self):
		self.dialog=gpvdm_open(get_spectra_path(),big_toolbar=True)
		self.new_materials = QAction_lock("add_spectra", wrap_text(_("Add Spectra"),8), self,"add_spectra")
		self.new_materials.clicked.connect(self.on_new_spectra_clicked)
		self.dialog.toolbar.addAction(self.new_materials)
		self.dialog.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_update_window(self):
		webbrowser.open('http://www.gpvdm.com/download_materials.php')


	def callback_configure_lasers(self):

		if self.lasers_window==None:
			self.lasers_window=lasers()

		help_window().help_set_help(["lasers.png",_("<big><b>Laser setup</b></big><br> Use this window to set up your lasers.")])
		if self.lasers_window.isVisible()==True:
			self.lasers_window.hide()
		else:
			self.lasers_window.show()

	def callback_configure_emission(self):
		self.dialog=gpvdm_open(get_emission_path(),big_toolbar=True)
		self.new_emission = QAction_lock("add_emission", wrap_text(_("Add Spectra"),8), self,"add_emission")
		self.new_emission.clicked.connect(self.on_new_emission_clicked)
		self.dialog.toolbar.addAction(self.new_emission)
		self.dialog.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_configure_shape(self):
		self.dialog=gpvdm_open(get_shape_path(),big_toolbar=True)
		self.new_shape = QAction_lock("add_shape", wrap_text(_("Add Shape"),8), self,"add_shape")
		self.new_shape.clicked.connect(self.on_new_shape_clicked)
		self.dialog.toolbar.addAction(self.new_shape)
		self.dialog.show_inp_files=False
		ret=self.dialog.exec_()

	def callback_configure_user_data(self):
		self.dialog=gpvdm_open(get_user_data_path(),big_toolbar=True)
		ret=self.dialog.exec_()

