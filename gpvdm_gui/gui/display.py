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

## @package display
#  The display widget, this either displays the 3D OpenGL image of the device or the fallback non OpenGL widget.
#

import os

#inp
from inp import inp_get_token_value

#path
from cal_path import get_materials_path

from gl import glWidget
from gl_fallback import gl_fallback
#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit,QLabel
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import pyqtSignal

from icon_lib import icon_get

from help import help_window
from gl_cmp import gl_cmp

from str2bool import str2bool
from fx_selector import fx_selector

from cal_path import get_sim_path
from global_objects import global_object_register
from global_objects import global_object_run

open_gl_working=False

def is_open_gl_working():
	global open_gl_working
	if open_gl_working==True:
		return "yes"
	else:
		return "no"

class display_widget(QWidget):

	colors=[]

	def add_fallback(self):
		global open_gl_working
		open_gl_working=False
		self.tb_rotate.setEnabled(False)
		self.xy.setEnabled(False)
		self.yz.setEnabled(False)
		self.xz.setEnabled(False)

		self.display=gl_fallback()
		self.hbox.addWidget(self.display)
	
	def update_ray_file(self):
		self.display.ray_file=self.fx_box.get_file_name()

	def __init__(self):
		QWidget.__init__(self)
		self.complex_display=False

		self.hbox=QVBoxLayout()
		self.gl_cmp=gl_cmp(os.path.join(get_sim_path(),"snapshots"))
		
		toolbar=QToolBar()
		toolbar.setIconSize(QSize(42, 42))

		#self.tb_layer_editor = QAction(icon_get("layers"), _("Layer\neditor"), self)
		#self.tb_layer_editor.triggered.connect(self.callback_layer_editor)
		#toolbar.addAction(self.tb_layer_editor)
		


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

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
		
		self.tb_config = QAction(icon_get("preferences-system"), _("Configuration"), self)
		self.tb_config.triggered.connect(self.callback_configure)
		toolbar.addAction(self.tb_config)

		self.fx_box=fx_selector()
		self.fx_box.update()

		self.fx_box.cb.currentIndexChanged.connect(self.fx_box_changed)
		toolbar.addWidget(self.fx_box)

		self.hbox.addWidget(toolbar)
		
		enable_3d=inp_get_token_value(os.path.join(get_sim_path(),"config.inp") , "#gui_config_3d_enabled")
		if enable_3d==None:
			enable_3d="True"
		enable_3d=str2bool(enable_3d)
		
		#if enable_3d==True:
		self.display=glWidget(self)
		self.update_ray_file()

		self.hbox.addWidget(self.display)

			#self.display.setMinimumSize(800, 600)

		#	self.timer=QTimer()
		#	self.timer.setSingleShot(True)
		#	self.timer.timeout.connect(self.timer_update)
		#	self.timer.start(20000)
		#else:
		#	self.add_fallback()
			
		self.setLayout(self.hbox)
		global_object_register("display_recalculate",self.recalculate)
		global_object_register("display_set_selected_layer",self.set_selected_layer)

		

	def callback_xy(self):
		self.display.xy()

	def callback_yz(self):
		self.display.yz()
		
	def callback_xz(self):
		self.display.xz()

	def fx_box_changed(self):
		self.update_ray_file()
		self.display.force_redraw()
		#print("rod",self.display.ray_file)
		

	def tb_rotate_click(self):
		self.display.start_rotate()
		
	#def timer_update(self):
	#	global open_gl_working
		
	#	open_gl_working=not self.display.failed
		
	#	if open_gl_working==True:
	#		print("OpenGL is working")

	#		from lock import get_lock
	#		get_lock().open_gl_working=True

	#	else:
	#		print("OpenGL is not working going to fallback")
	#		from lock import get_lock
	#		get_lock().open_gl_working=False

	#		self.hbox.removeWidget(self.display)
	#		self.display.deleteLater()
	#		self.display = None
			#self.add_fallback()

	#		help_window().help_append(["warning.png",_("<big><b>OpenGL warning</b></big><br>It looks as if you don't have working 3D graphics acceleration on your computer.  gpvdm will therefore fallback to 2D mode. The model will still be fully functional, but not look quite so nice.")])

	def set_selected_layer(self,n):
		global open_gl_working
		if open_gl_working==True:
			self.display.selected_obj=self.display.gl_objects_find("layer:"+str(n))

	#This will reclaculate all the display elements in the display widget.
	def recalculate(self):
		self.fx_box.update()
		self.update_ray_file()
		self.display.graph_path=self.gl_cmp.slider.get_file_name()
		#self.display.graph_z_max=self.gl_cmp.slider.z_max
		#self.display.graph_z_min=self.gl_cmp.slider.z_min
			
		self.display.force_redraw()
		
	def update(self):
		self.display.reset()
#		print("recalculate")
	#	self.display.update()
	#	self.fx_box.update()

	def callback_configure(self):
		if self.gl_cmp.isVisible()==True:
			self.gl_cmp.hide()
		else:
			self.gl_cmp.show()

	#def callback_layer_editor(self):
	#	global_object_run("show_layer_editor")
