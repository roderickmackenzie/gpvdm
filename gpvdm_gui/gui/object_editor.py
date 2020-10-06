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

## @package object_editor
#  An editor for shape files
#

import os
from tab import tab_class
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget, QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#python modules
import webbrowser

from help import help_window
from QWidgetSavePos import QWidgetSavePos
from css import css_apply

from global_objects import global_object_run
from epitaxy import get_epi
from util import wrap_text
from inp import inp_copy_file

from shape import shape
from cal_path import get_sim_path
from inp import inp_ls_seq_files
from inp import inp_remove_file

from gui_util import dlg_get_text
from error_dlg import error_dlg
from gui_util import yes_no_dlg
from cal_path import get_default_material_path
from mesh import get_mesh
from tick_cross import tick_cross
from inp import inp
from str2bool import str2bool
from global_objects import global_object_run

articles = []
mesh_articles = []

class object_editor(QWidgetSavePos):

	def callback_help(self):
		webbrowser.open('http://www.gpvdm.com/man/index.html')

	def __init__(self):
		QWidgetSavePos.__init__(self,"shape_editor")
		self.setMinimumSize(600, 700)
		self.setWindowIcon(icon_get("shape"))

		self.setWindowTitle(_("Object editor")+"  (https://www.gpvdm.com)") 
		

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


		self.help = QAction(icon_get("help"), _("Help"), self)
		self.help.setStatusTip(_("Close"))
		self.help.triggered.connect(self.callback_help)
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

		print(ids[0])
		self.layer_index=self.epi.find_layer_by_id(ids[0])

		for id in ids:
			s=self.epi.find_shape_by_id(id)
			my_tab=tab_class(s.file_name+".inp")
			my_tab.changed.connect(self.callback_edit)
			if i==0:
				name=_("Layer: ")+s.name
			else:
				name=s.name

			self.notebook.addTab(my_tab,name)	
			i=i+1

	def callback_edit(self):
		tab = self.notebook.currentWidget()
		s=self.epi.find_shape_by_file_name(tab.file_name)
		s.do_load()
		global_object_run("gl_force_redraw")

	def callback_enable_disable(self):
		tab = self.notebook.currentWidget()
		if tab!=None:
			tab.setEnabled(self.enable.enabled)
			s=self.epi.find_shape_by_file_name(tab.file_name)
			if s!=None:
				tab.tab.f.replace("#shape_enabled",str(self.enable.enabled))
				s.shape_enabled=self.enable.enabled
				s.save()
				global_object_run("gl_force_redraw")
			#f=inp()
			#if f.load(tab.file_name)!=False:
			#	f.replace("#shape_enabled",str(self.enable.enabled))
			#	f.save()
			

	def changed_click(self):
		tab = self.notebook.currentWidget()
		if tab!=None:
			s=self.epi.find_shape_by_file_name(tab.file_name)
			if s!=None:
				tab.setEnabled(s.shape_enabled)
				self.enable.setState(s.shape_enabled)
				self.status_bar.showMessage(tab.file_name)

			if self.notebook.currentIndex()==0:
				self.tb_delete.setEnabled(False)
			else:
				self.tb_delete.setEnabled(True)

	def callback_add_shape(self):
		layer=get_epi().layers[self.layer_index]
		s=get_epi().new_shape_file(layer)
		layer.shapes.append(s)
		new_filename=s.file_name+".inp"
		get_epi().save()

		my_tab=tab_class(new_filename)
		self.notebook.addTab(my_tab,s.name)
		my_tab.changed.connect(self.callback_edit)
		global_object_run("gl_force_redraw")

	def callback_rename_shape(self):
		tab = self.notebook.currentWidget()
		s=self.epi.find_shape_by_file_name(tab.file_name)

		new_sim_name=dlg_get_text( "Rename the shape:", s.name,"rename.png")

		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			s.name=new_sim_name
			s.save()
			index=self.notebook.currentIndex() 
			self.notebook.setTabText(index, new_sim_name)

	def callback_clone_shape(self):
		tab = self.notebook.currentWidget()
		s=self.epi.find_shape_by_file_name(tab.file_name)
		name=s.name+"_new"

		new_sim_name=dlg_get_text( "Clone the shape:", name,"clone.png")
		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			old_name=os.path.join(get_sim_path(),tab.file_name)
			new_name=get_epi().new_electrical_file("shape")
			my_shape=shape()
			my_shape.load(old_name)
			my_shape.name=new_sim_name
			my_shape.x0=my_shape.x0-my_shape.dx
			my_shape.shape_electrical=get_epi().gen_new_electrical_file("electrical")
			my_shape.file_name=new_name
			my_shape.save()

			get_epi().layers[self.layer_index].shapes.append(my_shape)
			get_epi().save()

			my_tab=tab_class(my_shape.file_name+".inp")
			self.notebook.addTab(my_tab,my_shape.name)
			my_tab.changed.connect(self.callback_edit)
			global_object_run("gl_force_redraw")

	def callback_delete_shape(self):
		tab = self.notebook.currentWidget()
		s=self.epi.find_shape_by_file_name(tab.file_name)
		name=s.name
		
		response=yes_no_dlg(self,"Do you really want to delete the file: "+name)

		if response == True:
			inp_remove_file(os.path.join(get_sim_path(),tab.file_name))

			index=self.notebook.currentIndex() 
			self.notebook.removeTab(index)

			for i in range(0,len(get_epi().layers[self.layer_index].shapes)):
				if get_epi().layers[self.layer_index].shapes[i].file_name+".inp"==tab.file_name:
					get_epi().layers[self.layer_index].shapes.pop(i)
					get_epi().clean_unused_files()
					get_epi().save()
					break

		global_object_run("gl_force_redraw")

