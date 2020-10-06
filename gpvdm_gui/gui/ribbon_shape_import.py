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

## @package ribbon_shape_import
#  The ribbon for importaing shape data files.
#


import os

from dump_io import dump_io
from tb_item_sim_mode import tb_item_sim_mode
from tb_item_sun import tb_item_sun

from code_ctrl import enable_betafeatures
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar, QToolButton,QMenu
from PyQt5.QtWidgets import QTabWidget

from info import sim_info
from win_lin import desktop_open

#windows
from scan import scan_class 
from help import help_window


from global_objects import global_object_run
from util import isfiletype
from icon_lib import icon_get

from cal_path import get_sim_path
from util import wrap_text

from ribbon_base import ribbon_base
from QAction_lock import QAction_lock
from play import play

class ribbon_shape_import(ribbon_base):
	def file_toolbar(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.import_image= QAction_lock("import_image", wrap_text(_("Load new image"),7), self,"ribbon_shape_import_import_image")
		toolbar.addAction(self.import_image)

		self.save_data= QAction_lock("document-save-as", wrap_text(_("Import data"),4), self,"ribbon_shape_import_save")
		toolbar.addAction(self.save_data)

		#self.xy_triangles= QAction_lock("shape", wrap_text(_("xy triangles"),2), self,"ribbon_shape_xy_tri")
		#toolbar.addAction(self.xy_triangles)


		self.tb_script= QAction_lock("script", wrap_text(_("Generate\nImage"),2), self,"ribbon_shape_script")
		toolbar.addAction(self.tb_script)

		##########################
		self.tb_honeycomb= QAction_lock("honeycomb", wrap_text(_("Generate\nHoneycomb"),2), self,"ribbon_shape_honeycomb")

		self.tb_honeycomb_menu = QMenu(self)
		self.tb_honeycomb.setMenu(self.tb_honeycomb_menu)

		self.tb_honeycomb_menu_edit=QAction(_("Edit"), self)
		self.tb_honeycomb_menu.addAction(self.tb_honeycomb_menu_edit)

		toolbar.addAction(self.tb_honeycomb)
		##########################
		self.tb_gaus= QAction_lock("gauss", wrap_text(_("Generate\nGaussian"),2), self,"ribbon_shape_gaus")

		self.tb_gaus_menu = QMenu(self)
		self.tb_gaus.setMenu(self.tb_gaus_menu)

		self.tb_gaus_menu_edit=QAction(_("Edit"), self)
		self.tb_gaus_menu.addAction(self.tb_gaus_menu_edit)

		toolbar.addAction(self.tb_gaus)
		##########################

		self.tb_boundary= QAction_lock("boundary", wrap_text(_("Boundary"),2), self,"ribbon_shape_boundary")

		#self.tb_boundary_menu = QMenu(self)
		#self.tb_boundary.setMenu(self.tb_boundary_menu)

		#self.tb_boundary_menu_edit=QAction(_("Edit"), self)
		#self.tb_boundary_menu.addAction(self.tb_boundary_menu_edit)

		toolbar.addAction(self.tb_boundary)
		##########################

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.tb_help = QAction(icon_get("help"), _("Help"), self)
		self.tb_help.setStatusTip(_("Help"))
		toolbar.addAction(self.tb_help)


		return toolbar

	def mesh_toolbar(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		#mesh
		self.show_mesh= QAction_lock("mesh_tri", wrap_text(_("Show\nMesh"),2), self,"ribbon_shape_mesh")
		self.show_mesh.setCheckable(True)
		self.show_mesh.setChecked(True)
		toolbar.addAction(self.show_mesh)

		#self.menu_mesh = QMenu(self)
		#self.show_mesh.setMenu(self.menu_mesh)

		#self.edit_mesh=QAction(_("Edit mesh"), self)
		#self.menu_mesh.addAction(self.edit_mesh)

		self.mesh_edit= QAction_lock("mesh_tri_edit", wrap_text(_("Edit\nMesh"),2), self,"ribbon_shape_mesh")
		toolbar.addAction(self.mesh_edit)

		self.mesh_build = play(self,"main_play_button",run_text=wrap_text(_("Build\nMesh"),2))

		#self.mesh_build= QAction_lock("media-playback-start", wrap_text(_("Build\nMesh"),2), self,"ribbon_shape_mesh")
		toolbar.addAction(self.mesh_build)

		return toolbar

	def image_toolbar(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))


		#blur
		self.tb_blur= QAction_lock("blur", wrap_text(_("Blur"),2), self,"ribbon_shape_blur")
		self.tb_blur.setCheckable(True)
		#self.tb_blur.setChecked(True)

		self.menu_blur = QMenu(self)
		self.tb_blur.setMenu(self.menu_blur)

		self.edit_blur=QAction(_("Edit Gaussian"), self)
		self.menu_blur.addAction(self.edit_blur)

		toolbar.addAction(self.tb_blur)

		#norm y
		self.tb_norm_y= QAction_lock("plot_norm", wrap_text(_("Normalize\ny"),2), self,"ribbon_shape_norm_y")
		self.tb_norm_y.setCheckable(True)
		#self.tb_norm_y.setChecked(True)

		self.menu_norm_y = QMenu(self)
		self.tb_norm_y.setMenu(self.menu_norm_y)

		self.edit_norm_y=QAction(_("Edit histogram"), self)
		self.menu_norm_y.addAction(self.edit_norm_y)

		toolbar.addAction(self.tb_norm_y)

		#norm z
		self.tb_norm_z= QAction_lock("plot_norm", wrap_text(_("Normalize\nz"),2), self,"ribbon_shape_norm_z")
		self.tb_norm_z.setCheckable(True)
		#self.tb_norm_z.setChecked(True)
		toolbar.addAction(self.tb_norm_z)

		#rotate
		self.tb_rotate= QAction_lock("rotate_right", wrap_text(_("Rotate"),2), self,"ribbon_rotate_right")

		toolbar.addAction(self.tb_rotate)


		return toolbar
	def __init__(self):
		ribbon_base.__init__(self)
		self.setMaximumHeight(140)
		w=self.file_toolbar()
		self.addTab(w,_("File"))

		w=self.image_toolbar()
		self.addTab(w,_("Image"))

		w=self.mesh_toolbar()
		self.addTab(w,_("Mesh"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

