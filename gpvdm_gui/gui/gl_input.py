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
from math import cos
from math import sin

from epitaxy import get_epi
from gl_lib import gl_obj_id_starts_with
from gpvdm_json import gpvdm_data
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import Qt

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
					if self.active_view.zoom>-40:
						self.active_view.zoom =-400
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

		old_val=self.view_options.text
		self.view_options.text=False
		self.render()
		self.view_options.text=old_val

		data=glReadPixelsub(x, y, 1, 1, GL_RGBA,GL_FLOAT)

		obj=self.gl_objects_search_by_color(data[0][0][0],data[0][0][1],data[0][0][2])

		self.set_false_color(False)
		return obj

	def mouseDoubleClickEvent(self,event):
		#thumb_nail_gen()
		self.obj=self.event_to_3d_obj(event)
		if self.obj!=None:
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

	def get_3d_pos(self,event):
		self.lastPos=event.pos()
		modelview=self.active_view.modelview
		projection=self.active_view.projection
		viewport=self.active_view.viewport

		winX = event.x()
		#if winX>viewport[2]:
		#	winX=winX-viewport[2]

		y=event.y()

		#if y>viewport[3]:
		#	y=y-viewport[3]

		#print(viewport,winX,y)

		winY = viewport[3] - y

		winZ=glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT)
		x0,y0,z0=gluUnProject(winX, winY, winZ, modelview, projection, viewport)
		x1,y1,z1=gluUnProject(winX+2, winY, winZ, modelview, projection, viewport)
		dxdx=(x1-x0)/2.0
		dydx=(y1-y0)/2.0
		dzdx=(z1-z0)/2.0

		#print(x0,y0,z0,x1,y1,z1)

		x0,y0,z0=gluUnProject(winX, winY, winZ, modelview, projection, viewport)
		x1,y1,z1=gluUnProject(winX, winY+2, winZ, modelview, projection, viewport)
		dxdy=(x1-x0)/2.0
		dydy=(y1-y0)/2.0
		dzdy=(z1-z0)/2.0
		return dxdx,dydx,dzdx,dxdy,dydy,dzdy 
		
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
				if self.active_view.enable_view_move==True:
					self.active_view.xRot =self.active_view.xRot + 1 * dy
					self.active_view.yRot =self.active_view.yRot - 1 * dx

			if event.buttons()==Qt.RightButton:
				self.set_cursor(QCursor(Qt.SizeAllCursor))
				self.active_view.x_pos =self.active_view.x_pos + 0.1 * dx
				self.active_view.y_pos =self.active_view.y_pos + 0.1 * dy
		else:
			modifiers = QApplication.keyboardModifiers()
			if modifiers == Qt.ShiftModifier:
				dtheta=360*dx/self.width()
				dphi=360*dy/self.height()
				self.gl_objects_rotate(dphi,dtheta)
			else:
				if self.view_count_enabled()==1:
					dx_=self.dxdx*dx-self.dxdy*dy
					dy_=self.dydx*dy-self.dydy*dy
					dz_=self.dzdx*dx-self.dzdy*dy
					self.gl_objects_move(dx_,dy_,dz_)
				else:
					dx_=dx*cos(2.0*3.14159*self.active_view.yRot/360)+dy*sin(2.0*3.14159*self.active_view.xRot/360)
					dz_=dx*sin(2.0*3.14159*self.active_view.yRot/360)-dy*sin(2.0*3.14159*self.active_view.xRot/360)
					dy_=dy*cos(2.0*3.14159*self.active_view.xRot/360)
					self.gl_objects_move(dx_*0.2/self.active_view.zoom,dy_*0.2/self.active_view.zoom,dz_*0.2/self.active_view.zoom)

		self.lastPos=event.pos()
		self.setFocusPolicy(Qt.StrongFocus)
		self.setFocus()
		self.force_redraw(level="no_rebuild")
		#self.view_dump()

	def event_to_view(self,event):
		for v in self.views:
			if event.x()>v.window_x*self.width():
				if self.height()-event.y()>v.window_y*self.height():
					if event.x()<v.window_x*self.width()+v.window_w*self.width():
						if self.height()-event.y()<v.window_y*self.height()+v.window_h*self.height():
							return v

		return False

	def mousePressEvent(self,event):
		self.lastPos=None
		self.mouse_click_event=mouse_event()
		self.mouse_click_event.time=time.time()
		self.mouse_click_event.x=event.x()
		self.mouse_click_event.y=event.y()
		self.active_view=self.event_to_view(event)

		if event.buttons()==Qt.LeftButton or event.buttons()==Qt.RightButton:
			obj=self.event_to_3d_obj(event)

			if obj!=None:
				if len(obj.id)>0:
					self.dxdx,self.dydx,self.dzdx,self.dxdy,self.dydy,self.dzdy = self.get_3d_pos(event)
					self.gl_object_deselect_all()
					self.gl_objects_select_by_id(obj.id)
					self.set_cursor(QCursor(Qt.SizeAllCursor))
					self.text_output.emit(obj.id[0]+" "+obj.text)
					self.force_redraw(level="no_rebuild")
			else:
				self.gl_object_deselect_all()
				self.force_redraw(level="no_rebuild")


	def mouseReleaseEvent(self,event):
		self.set_cursor(None)

		delta=time.time() - self.mouse_click_event.time

		obj=self.event_to_3d_obj(event)

		#print(self.mouse_click_event.y,self.mouse_click_event.delta_time())
		if event.button()==Qt.RightButton:
			#print(self.obj)
			if (delta)<3:
				if obj!=None:
					if len(obj.id)>0:
						data_obj=gpvdm_data().find_object_by_id(obj.id[0])
						if data_obj!=None:
							if gl_obj_id_starts_with(data_obj.id,"mesh")==False:
								self.menu_obj(event,data_obj)
							else:
								self.mesh_menu(event)
				else:
					self.menu(event)

		if event.button()==Qt.LeftButton:
			self.gl_objects_save_selected()


	def wheelEvent(self,event):
		p=event.angleDelta()
		self.active_view=self.event_to_view(event)
		if self.active_view!=False:
			self.active_view.zoom =self.active_view.zoom - p.y()/120
			self.force_redraw(level="no_rebuild")

