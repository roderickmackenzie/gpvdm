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

from gl import glWidget
from gl_fallback import gl_fallback
#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout, QLineEdit,QLabel
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import pyqtSignal

from icon_lib import icon_get

from cal_path import get_sim_path
from global_objects import global_object_register
from global_objects import global_object_run
from gpvdm_local import gpvdm_local
open_gl_working=False


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
		data=gpvdm_local()
		enable_3d=data.gui_config.enable_opengl
		
		#if enable_3d==True:
		self.display=glWidget(self)
		#self.update_ray_file()

		self.hbox.addWidget(self.display)

		self.setLayout(self.hbox)
		global_object_register("display_recalculate",self.recalculate)
		global_object_register("display_set_selected_obj",self.set_selected_obj)

	def fx_box_changed(self):
		self.update_ray_file()
		self.display.force_redraw()
		#print("rod",self.display.ray_file)
		


		
	def set_selected_obj(self,obj_id):
		if self.display.open_gl_working==True:
			self.display.gl_object_deselect_all()
			obj=self.display.gl_objects_find(obj_id)
			if obj!=None:
				obj.selected=True


	#This will reclaculate all the display elements in the display widget.
	def recalculate(self):
		self.fx_box.update()
		self.update_ray_file()
			
		self.display.force_redraw()
		
	def update(self):
		self.display.reset()


	#def callback_layer_editor(self):
	#	global_object_run("show_layer_editor")
