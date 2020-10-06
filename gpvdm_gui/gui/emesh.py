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

## @package emesh
#  The main window to edit the electrical mesh, can select between a 1D, 2D and 3D mesh.
#

import os
from cal_path import get_exe_command
from numpy import *
from matplotlib.figure import Figure
from icon_lib import icon_get
import webbrowser
from electrical_mesh_editor import electrical_mesh_editor
from inp import inp_search_token_value
from epitaxy import epitaxy_get_dos_files

#inp
from inp import inp_sum_items

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox,QVBoxLayout

#windows
from mesh import get_mesh

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar

from global_objects import global_object_run
from QWidgetSavePos import QWidgetSavePos

from mesh_configure import mesh_configure
from global_objects import global_object_register

class tab_electrical_mesh(QWidgetSavePos):

	def save_image(self,file_name):
		self.fig.savefig(file_name)

	def update(self):
		self.emesh_editor_x.update()
		self.emesh_editor_y.update()
		self.emesh_editor_z.update()
		self.update_dim()

	def callback_help(self, widget, data=None):
		webbrowser.open('https://www.gpvdm.com/man/index.html')

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
		if get_mesh().get_xpoints()==1 and get_mesh().get_zpoints()==1:
			self.one_d.setEnabled(False)
			self.two_d.setEnabled(True)
			self.three_d.setEnabled(True)
			self.emesh_editor_y.setEnabled(True)
			self.emesh_editor_x.setEnabled(False)
			self.emesh_editor_z.setEnabled(False)
			self.emesh_editor_y.show()
			self.emesh_editor_x.hide()
			self.emesh_editor_z.hide()


		if get_mesh().get_xpoints()>1 and get_mesh().get_zpoints()==1:
			self.one_d.setEnabled(True)
			self.two_d.setEnabled(False)
			self.three_d.setEnabled(True)
			self.emesh_editor_y.setEnabled(True)
			self.emesh_editor_x.setEnabled(True)
			self.emesh_editor_z.setEnabled(False)
			self.emesh_editor_y.show()
			self.emesh_editor_x.show()
			self.emesh_editor_z.hide()

		if get_mesh().get_xpoints()>1 and get_mesh().get_zpoints()>1:
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

		self.undo = QAction(icon_get("help"), _("Help"), self)
		self.undo.setStatusTip(_("Close"))
		self.undo.triggered.connect(self.callback_help)
		toolbar.addAction(self.undo)

		self.main_vbox.addWidget(toolbar)
		

		widget=QWidget()
		mesh_hbox=QHBoxLayout()
		widget.setLayout(mesh_hbox)
	
		self.emesh_editor_x=electrical_mesh_editor("x")
		self.emesh_editor_x.changed.connect(self.emit_now)
		
		self.emesh_editor_y=electrical_mesh_editor("y")
		self.emesh_editor_y.changed.connect(self.emit_now)

		self.emesh_editor_z=electrical_mesh_editor("z")
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
			self.mesh_config=mesh_configure()
		self.mesh_config.show()
