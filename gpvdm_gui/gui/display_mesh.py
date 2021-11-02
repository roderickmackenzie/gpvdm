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

## @package display
#  The display widget, this either displays the 3D OpenGL image of the device or the fallback non OpenGL widget.
#

import os

from gl import glWidget
#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit,QLabel
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import pyqtSignal

from icon_lib import icon_get

from help import help_window

from str2bool import str2bool

from cal_path import get_sim_path
from global_objects import global_object_register
from global_objects import global_object_run

from dat_file import dat_file
from server import server_get
from gpvdm_json import gpvdm_data

class display_mesh(QWidget):


	def __init__(self):
		QWidget.__init__(self)
		self.complex_display=False

		self.hbox=QVBoxLayout()
		self.data=dat_file()
		self.my_server=server_get()
		data=gpvdm_data()

		if data.electrical_solver.solver_type=="circuit":
			toolbar=QToolBar()
			toolbar.setIconSize(QSize(42, 42))

			spacer = QWidget()
			spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
			toolbar.addWidget(spacer)

			self.tb_refresh = QAction(icon_get("view-refresh"), _("Refresh"), self)
			self.tb_refresh.triggered.connect(self.rebuild_mesh)
			toolbar.addAction(self.tb_refresh)


			self.xy = QAction(icon_get("xy"), _("xy"), self)
			self.xy.triggered.connect(self.callback_xy)
			toolbar.addAction(self.xy)

			self.yz = QAction(icon_get("yz"), _("yz"), self)
			self.yz.triggered.connect(self.callback_yz)
			toolbar.addAction(self.yz)

			self.xz = QAction(icon_get("xz"), _("xz"), self)
			self.xz.triggered.connect(self.callback_xz)
			toolbar.addAction(self.xz)
			
			self.tb_rotate = QAction(icon_get("rotate.png"), _("Rotate"), self)
			self.tb_rotate.triggered.connect(self.tb_rotate_click)
			toolbar.addAction(self.tb_rotate)
			self.tb_rotate.setEnabled(True)


			self.hbox.addWidget(toolbar)
			

			self.display=glWidget(self)
			self.display.draw_electrical_mesh=False
			self.display.view_options.draw_device=False
			self.display.enable_draw_ray_mesh=False
			self.display.view_options.draw_rays=False
			self.display.view_options.render_photons=False


			#self.display.force_redraw()
			#global_object_register("display_mesh_recalculate",self.recalculate)

		self.hbox.addWidget(self.display)
			
		self.setLayout(self.hbox)


	def callback_xy(self):
		self.display.plot.xy()

	def callback_yz(self):
		self.display.yz()
		
	def callback_xz(self):
		self.display.xz()

	def tb_rotate_click(self):
		self.display.start_rotate()

	def refresh_display(self):
		self.display.load_from_json(os.path.join(get_sim_path(),"electrical_mesh.dat"))
		self.display.force_redraw()

	def rebuild_mesh(self):
		try:
			self.my_server.sim_finished.disconnect(self.refresh_display)
		except:
			pass

		self.my_server.add_job(get_sim_path(),"--simmode circuit_mesh@mesh_gen_electrical")
		self.my_server.sim_finished.connect(self.refresh_display)
		self.my_server.print_jobs()
		self.my_server.start()

	def update(self):
		pass
		#self.display.reset()


