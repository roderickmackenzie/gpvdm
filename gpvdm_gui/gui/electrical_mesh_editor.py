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

## @package emesh
#  The main window to edit the electrical mesh, can select between a 1D, 2D and 3D mesh.
#

import os
from cal_path import get_exe_command
from numpy import *
from matplotlib.figure import Figure
from icon_lib import icon_get

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox,QVBoxLayout


#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

from global_objects import global_object_run
from QWidgetSavePos import QWidgetSavePos

from global_objects import global_object_register
from electrical_mesh_editor_tab import electrical_mesh_editor_tab
from config_window import class_config_window
from tab_view import tab_view

from gpvdm_json import gpvdm_data
from help import QAction_help

class electrical_mesh_editor(QWidgetSavePos):

	def save_image(self,file_name):
		self.fig.savefig(file_name)

	def update(self):
		self.emesh_editor_x.update()
		self.emesh_editor_y.update()
		self.emesh_editor_z.update()
		self.update_dim()

	def callback_dim_1d(self):
		self.emesh_editor_y.enable_dim()
		self.emesh_editor_x.disable_dim()
		self.emesh_editor_z.disable_dim()
		self.update_dim()
		
	def callback_dim_2d(self):
		self.emesh_editor_y.enable_dim()
		self.emesh_editor_x.enable_dim()
		self.emesh_editor_z.disable_dim()
		self.update_dim()

	def callback_dim_3d(self):
		self.emesh_editor_y.enable_dim()
		self.emesh_editor_x.enable_dim()
		self.emesh_editor_z.enable_dim()
		self.update_dim()


	def update_dim(self):
		mesh_x=gpvdm_data().mesh.mesh_x
		mesh_z=gpvdm_data().mesh.mesh_z

		if mesh_x.get_points()==1 and mesh_z.get_points()==1:
			self.one_d.setEnabled(False)
			self.two_d.setEnabled(True)
			self.three_d.setEnabled(True)
			self.emesh_editor_y.setEnabled(True)
			self.emesh_editor_x.setEnabled(False)
			self.emesh_editor_z.setEnabled(False)
			self.emesh_editor_y.show()
			self.emesh_editor_x.hide()
			self.emesh_editor_z.hide()


		if mesh_x.get_points()>1 and mesh_z.get_points()==1:
			self.one_d.setEnabled(True)
			self.two_d.setEnabled(False)
			self.three_d.setEnabled(True)
			self.emesh_editor_y.setEnabled(True)
			self.emesh_editor_x.setEnabled(True)
			self.emesh_editor_z.setEnabled(False)
			self.emesh_editor_y.show()
			self.emesh_editor_x.show()
			self.emesh_editor_z.hide()

		if mesh_x.get_points()>1 and mesh_z.get_points()>1:
			self.one_d.setEnabled(True)
			self.two_d.setEnabled(True)
			self.three_d.setEnabled(False)
			self.emesh_editor_y.setEnabled(True)
			self.emesh_editor_x.setEnabled(True)
			self.emesh_editor_z.setEnabled(True)
			self.emesh_editor_y.show()
			self.emesh_editor_x.show()
			self.emesh_editor_z.show()

		self.emit_now()

	def emit_now(self):
		global_object_run("gl_force_redraw")
		
	def __init__(self):
		QWidgetSavePos.__init__(self,"emesh")

		self.setMinimumSize(1200, 600)
		self.setWindowIcon(icon_get("mesh"))

		self.setWindowTitle(_("Electrical Mesh Editor")+" - (https://www.gpvdm.com)") 
		

		self.main_vbox = QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		self.one_d = QAction(icon_get("1d"), _("1D simulation"), self)
		self.one_d.triggered.connect(self.callback_dim_1d)
		toolbar.addAction(self.one_d)

		self.two_d = QAction(icon_get("2d"), _("2D simulation"), self)
		self.two_d.triggered.connect(self.callback_dim_2d)
		toolbar.addAction(self.two_d)

		self.three_d = QAction(icon_get("3d"), _("3D simulation"), self)
		self.three_d.triggered.connect(self.callback_dim_3d)
		toolbar.addAction(self.three_d)

		configure = QAction(icon_get("preferences-system",size=32),  _("Configure mesh"), self)
		configure.triggered.connect(self.on_configure_click)
		toolbar.addAction(configure)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.help = QAction_help()
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)
		

		widget=QWidget()
		mesh_hbox=QHBoxLayout()
		widget.setLayout(mesh_hbox)
	
		self.emesh_editor_x=electrical_mesh_editor_tab("x")
		self.emesh_editor_x.changed.connect(self.emit_now)
		
		self.emesh_editor_y=electrical_mesh_editor_tab("y")
		self.emesh_editor_y.changed.connect(self.emit_now)

		self.emesh_editor_z=electrical_mesh_editor_tab("z")
		self.emesh_editor_z.changed.connect(self.emit_now)


		mesh_hbox.addWidget(self.emesh_editor_x)
		mesh_hbox.addWidget(self.emesh_editor_y)
		mesh_hbox.addWidget(self.emesh_editor_z)

		self.main_vbox.addWidget(widget)

		self.update_dim()

		self.setLayout(self.main_vbox)
		self.mesh_config=None
		global_object_register("mesh_update",self.update)
	
	def on_configure_click(self):
		if self.mesh_config==None:
			data=gpvdm_data()
			self.mesh_config=class_config_window([data.mesh.config],[_("Thermal boundary conditions")],title=_("Configure mesh"),icon="mesh")
			self.mesh_config.show()
		self.mesh_config.show()
