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

## @package object_editor
#  An editor for shape files
#

import os
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget, QStatusBar
from PyQt5.QtGui import QPainter,QIcon

from help import help_window
from QWidgetSavePos import QWidgetSavePos
from css import css_apply

from global_objects import global_object_run
from epitaxy import get_epi
from util import wrap_text

from shape import shape
from cal_path import get_sim_path

from gui_util import dlg_get_text
from error_dlg import error_dlg
from gui_util import yes_no_dlg
from tick_cross import tick_cross
from str2bool import str2bool
from global_objects import global_object_run
from json_viewer import json_viewer
from gpvdm_json import gpvdm_data
import copy
from help import QAction_help

class object_editor(QWidgetSavePos):


	def __init__(self,gl_forece_redraw):
		QWidgetSavePos.__init__(self,"shape_editor")
		self.setMinimumSize(600, 700)
		self.setWindowIcon(icon_get("shape"))

		self.setWindowTitle(_("Object editor")+"  (https://www.gpvdm.com)") 
		
		self.force_redraw=gl_forece_redraw
		self.main_vbox = QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(48, 48))

		self.tb_new = QAction(icon_get("document-new"), wrap_text("New shape",2), self)
		self.tb_new.triggered.connect(self.callback_add_shape)

		toolbar.addAction(self.tb_new)

		self.tb_delete = QAction(icon_get("edit-delete"), wrap_text("Delete shape",3), self)
		self.tb_delete.triggered.connect(self.callback_delete_shape)

		toolbar.addAction(self.tb_delete)

		self.tb_rename = QAction(icon_get("rename"), wrap_text("Rename shape",3), self)
		self.tb_rename.triggered.connect(self.callback_rename_shape)
		toolbar.addAction(self.tb_rename)

		self.tb_clone = QAction(icon_get("clone"), wrap_text("Clone shape",3), self)
		self.tb_clone.triggered.connect(self.callback_clone_shape)
		toolbar.addAction(self.tb_clone)

		self.enable=tick_cross(enable_text=_("Shape\nenabled"),disable_text=_("Shape\ndisabled"))
		self.enable.changed.connect(self.callback_enable_disable)
		toolbar.addAction(self.enable)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)


		self.help = QAction_help()
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)


		self.notebook = QTabWidget()
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		self.notebook.currentChanged.connect(self.changed_click)

		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)

		self.setLayout(self.main_vbox)
		self.epi=get_epi()

	def load(self,ids):
		i=0
		if type(ids)==str:
			ids=[ids]

		self.root_id=ids[0]

		for id in ids:
			s=gpvdm_data().find_object_by_id(id)
			my_tab=json_viewer()
			my_tab.populate(s)

			my_tab.changed.connect(self.callback_edit)

			name=s.shape_name

			self.notebook.addTab(my_tab,name)	
			i=i+1

	def callback_edit(self,item):
		data=gpvdm_data()
		tab = self.notebook.currentWidget()
		data.save()
		if item=="shape_type":
			self.force_redraw(level="reload_rebuild")
		else:
			self.force_redraw()

	def callback_enable_disable(self):
		data=gpvdm_data()
		tab = self.notebook.currentWidget()
		if tab!=None:
			tab.setEnabled(self.enable.enabled)
			s=tab.template_widget
			s.shape_enabled=self.enable.enabled
			data.save()
			self.force_redraw()
			

	def changed_click(self):
		tab = self.notebook.currentWidget()
		if tab!=None:
			s=tab.template_widget
			tab.setEnabled(s.shape_enabled)
			self.enable.setState(s.shape_enabled)
			self.status_bar.showMessage(s.shape_name)

			if self.notebook.currentIndex()==0:
				self.tb_delete.setEnabled(False)
				self.tb_clone.setEnabled(False)
			else:
				self.tb_delete.setEnabled(True)
				self.tb_clone.setEnabled(True)

	def callback_add_shape(self):
		data=gpvdm_data()
		obj=gpvdm_data().find_object_by_id(self.root_id)
		s=shape()
		s.dx=obj.dx/2.0
		s.dy=obj.dy/2.0
		s.dz=obj.dz/2.0
		s.moveable=True
		obj.shapes.append(s)
		my_tab=json_viewer()
		my_tab.populate(s)
		my_tab.changed.connect(self.callback_edit)
		self.notebook.addTab(my_tab,s.shape_name)
		my_tab.changed.connect(self.callback_edit)
		self.force_redraw(level="reload_rebuild")
		data.save()

	def callback_rename_shape(self):
		data=gpvdm_data()
		tab = self.notebook.currentWidget()

		new_sim_name=dlg_get_text( "Rename the shape:", tab.template_widget.shape_name,"rename.png")

		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			tab.template_widget.shape_name=new_sim_name
			index=self.notebook.currentIndex() 
			self.notebook.setTabText(index, new_sim_name)
			data.save()


	def callback_clone_shape(self):
		tab = self.notebook.currentWidget()
		name=tab.template_widget.shape_name+"_new"

		new_sim_name=dlg_get_text( "Clone the shape:", name,"clone.png")
		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			obj=gpvdm_data().find_object_by_id(self.root_id)
			for s in obj.shapes:
				if s.shape_name==tab.template_widget.shape_name:
					my_shape=copy.deepcopy(s)
					my_shape.shape_name=new_sim_name
					my_shape.update_random_ids()
					obj.shapes.append(my_shape)
					
					my_tab=json_viewer()
					my_tab.populate(my_shape)
					self.notebook.addTab(my_tab,my_shape.shape_name)
					my_tab.changed.connect(self.callback_edit)
					self.force_redraw()

	def callback_delete_shape(self):
		data=gpvdm_data()
		tab = self.notebook.currentWidget()
		name=tab.template_widget.shape_name
		
		response=yes_no_dlg(self,"Do you really want to the shape: "+name)

		if response == True:

			index=self.notebook.currentIndex() 
			self.notebook.removeTab(index)
			obj=gpvdm_data().find_object_by_id(self.root_id)
			for i in range(0,len(obj.shapes)):
				if obj.shapes[i].shape_name==tab.template_widget.shape_name:
					obj.shapes.pop(i)
					data.save()
					break

		self.force_redraw()

