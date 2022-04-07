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

## @package gpvdm_viewer_new
#  Make new files/directories
#


import os

#qt
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QMenu,QAbstractItemView,QListWidgetItem,QPushButton,QListView,QWidget,QListWidget,QAction,QDialog
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import pyqtSignal

from gui_util import dlg_get_text
from clone_materials import clone_material
from cal_path import gpvdm_paths
from cal_path import get_base_emission_path
from cal_path import get_base_spectra_path
from clone import clone_spectra

class gpvdm_viewer_new(QWidget):
	def new_shape(self):
		from shape_editor_io import shape_editor_io
		new_sim_name=dlg_get_text( _("New shape name:"), _("New shape name"),"add_shape")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_shape=os.path.join(self.path,new_sim_name)
			ret=clone_material(new_shape,gpvdm_paths.get_shape_template_path())
			a=shape_editor_io()
			a.save_as(os.path.join(new_shape,"data.json"))
			if ret==False:
				error_dlg(self,_("I cant write to:")+new_shape+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))
			self.fill_store()

	def new_emission(self):
		new_sim_name=dlg_get_text( _("New emission spectra name:"), _("New emission spectra name"),"add_emission")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_emission=os.path.join(self.path,new_sim_name)
			ret=clone_material(new_emission,os.path.join(get_base_emission_path(),"Irppy3"))
			if ret==False:
				error_dlg(self,_("I cant write to:")+new_emission+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))

			from json_emission_db_item import json_emission_db_item
			a=json_emission_db_item()
			a.save_as(os.path.join(new_emission,"data.json"))

			self.fill_store()

	def new_spectra(self):
		new_sim_name=dlg_get_text( _("New spectra name:"), _("New spectra name"),"add_spectra")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_spectra=os.path.join(self.path,new_sim_name)
			ret=clone_spectra(new_spectra,os.path.join(get_base_spectra_path(),"sun"))
			if ret==False:
				error_dlg(self,_("I cant write to:")+new_spectra+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))

			from json_spectra_db_item import json_spectra_db_item
			a=json_spectra_db_item()
			a.save_as(os.path.join(new_spectra,"data.json"))

			self.fill_store()

	def new_material(self):
		new_sim_name=dlg_get_text( _("New material name:"), _("New material name"),"organic_material")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_material=os.path.join(self.path,new_sim_name)
			#ret=clone_material(new_material,os.path.join(gpvdm_paths.get_inp_template_path(),"material"))
			#if ret==False:
			#	error_dlg(self,_("I cant write to:")+new_material+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))
			try:
				os.makedirs(new_material)
			except:
				error_dlg(self,_("I cant write to: ")+new_material+" "+_("This means either the disk is full or your system administrator has not given you write permissions to that location."))
			from json_material_db_item import json_material_db_item
			a=json_material_db_item()
			a.save_as(os.path.join(new_material,"data.json"))

			self.fill_store()

	def new_filter(self):
		new_sim_name=dlg_get_text( _("New filter name:"), _("New filter name"),"filter_wheel")
		new_sim_name=new_sim_name.ret
		if new_sim_name!=None:
			new_filter=os.path.join(self.path,new_sim_name)
			os.makedirs(new_filter)
			from json_filter_db_item import json_filter_db_item
			a=json_filter_db_item()
			a.save_as(os.path.join(new_filter,"data.json"))

			self.fill_store()
