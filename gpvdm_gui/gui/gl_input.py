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

## @package gl_input
#  The mouse and keyboard input to the OpenGL display.
#

import sys
open_gl_ok=False

import sys
from math import fabs
from PyQt5.QtWidgets import QApplication
from PyQt5.QtGui import QCursor
try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb
	from gl_color import color
	from gl_lim import gl_obj_id_starts_with

except:
	pass

from PyQt5.QtCore import QTimer
from PyQt5.QtCore import Qt
from PyQt5 import QtGui
import time


from epitaxy import get_epi
from gl_lib import gl_obj_id_starts_with

class mouse_event():
	def __init__(self):
		self.time=0
		self.x=0
		self.y=0

	def delta_time(self):
		return time.time()-self.time

class gl_input():

	def __init__(self):
		self.cursor=None

	def keyPressEvent(self, event):
		if type(event) == QtGui.QKeyEvent:
			if event.text()=="f":
				self.showFullScreen()
			if event.text()=="r":
				if self.timer==None:
					self.start_rotate()
				else:
					self.timer.stop()
					self.timer=None
			if event.text()=="z":
				if self.timer==None:
					self.start_rotate()
					if self.view.zoom>-40:
						self.view.zoom =-400
					self.timer=QTimer()
					self.timer.timeout.connect(self.fzoom_timer)
					self.timer.start(50)
				else:
					self.timer.stop()
					self.timer=None


	def event_to_3d_obj(self,event):
		x = event.x()
		y = self.height()-event.y()
		self.set_false_color(True)

		old_val=self.view.text
		self.view.text=False
		self.render()
		self.view.text=old_val

		data=glReadPixelsub(x, y, 1, 1, GL_RGBA,GL_FLOAT)

		obj=self.gl_objects_search_by_color(data[0][0][0],data[0][0][1],data[0][0][2])

		self.set_false_color(False)
		return obj

	def mouseDoubleClickEvent(self,event):
		#thumb_nail_gen()
		self.obj=self.event_to_3d_obj(event)
		if gl_obj_id_starts_with(self.obj.id,"layer")==True:
			self.selected_obj=self.obj
			self.do_draw()

	def set_cursor(self,cursor):
		if self.cursor!=cursor:
			if cursor==None:
				QApplication.restoreOverrideCursor()
			else:
				QApplication.setOverrideCursor(cursor)
			self.cursor=cursor
		
	def mouseMoveEvent(self,event):
		if 	self.timer!=None:
			self.timer.stop()
			self.timer=None

		if self.lastPos==None:
			self.lastPos=event.pos()
		dx = event.x() - self.lastPos.x();
		dy = event.y() - self.lastPos.y();
		obj=self.gl_objects_is_selected()
		if obj==False:
			if event.buttons()==Qt.LeftButton:
				
				self.view.xRot =self.view.xRot - 1 * dy
				self.view.yRot =self.view.yRot - 1 * dx

			if event.buttons()==Qt.RightButton:
				self.set_cursor(QCursor(Qt.SizeAllCursor))
				self.view.x_pos =self.view.x_pos + 0.1 * dx
				self.view.y_pos =self.view.y_pos - 0.1 * dy
		else:
			self.gl_objects_move(dx*0.05,-dy*0.05)
		
		self.lastPos=event.pos()
		self.setFocusPolicy(Qt.StrongFocus)
		self.setFocus()
		self.update()
		#self.view_dump()

	def mousePressEvent(self,event):
		self.lastPos=None
		self.mouse_click_event=mouse_event()
		self.mouse_click_event.time=time.time()
		self.mouse_click_event.x=event.x()
		self.mouse_click_event.y=event.y()
		if event.buttons()==Qt.LeftButton or event.buttons()==Qt.RightButton:
			obj=self.event_to_3d_obj(event)
			if obj!=None:
				self.gl_object_deselect_all()
				self.gl_objects_select_by_id(obj.id)
				self.set_cursor(QCursor(Qt.SizeAllCursor))

				self.text_output.emit(obj.id[0]+" "+obj.text)
				self.update()
			else:
				self.gl_object_deselect_all()
				self.update()


	def mouseReleaseEvent(self,event):
		self.set_cursor(None)

		delta=time.time() - self.mouse_click_event.time

		obj=self.event_to_3d_obj(event)

		#print(self.mouse_click_event.y,self.mouse_click_event.delta_time())
		if event.button()==Qt.RightButton:
			#print(self.obj)
			if (delta)<3:
				if obj!=None:
					epi=get_epi()
					#print(obj.id[0])
					if epi.find_shape_by_id(obj.id[0])!=None:
						#self.update()
						self.menu_layer(event)
					elif gl_obj_id_starts_with(obj.id,"mesh")==True:
						self.mesh_menu(event)
				else:
					self.menu(event)

		if event.button()==Qt.LeftButton:
			self.gl_objects_save_selected()


	def wheelEvent(self,event):
		p=event.angleDelta()
		self.view.zoom =self.view.zoom - p.y()/120
		self.update()

