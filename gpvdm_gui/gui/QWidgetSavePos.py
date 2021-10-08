# -*- coding: utf-8 -*-
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

## @package QWidgetSavePos
#  A winndow base class which saves the window position.
#

from PyQt5.QtWidgets import QWidget

from gpvdm_local import gpvdm_local
from gpvdm_local import json_save_window
from PyQt5.QtWidgets import QWidget, QDesktopWidget

def resize_window_to_be_sane(window,x,y):
	shape=QDesktopWidget().screenGeometry()
	w=shape.width()*x
	h=shape.height()*y
	window.resize(w,h)

class QWidgetSavePos(QWidget):

	def closeEvent(self, event):
		event.accept()
		
	def moveEvent(self,event):
		data=gpvdm_local()
		x=self.x()
		y=self.y()
		for seg in data.windows.segments:
			if seg.name==self.window_name:
				seg.x=x
				seg.y=y
				data.save()
				break

		event.accept()

	def __init__(self,window_name):
		QWidget.__init__(self)
		self.window_name=window_name

		data=gpvdm_local()
		found=False

		shape=QDesktopWidget()#.screenGeometry()

		desktop_w=shape.width()
		desktop_h=shape.height()

		w=self.width()
		h=self.height()
		sain_x=desktop_w/2-w/2
		sain_y=desktop_h/2-h/2

		for seg in data.windows.segments:
			#print(seg.name,window_name)
			if seg.name==window_name:

				x=int(seg.x)
				y=int(seg.y)
				if (x+w>desktop_w) or x<0:
					x=sain_x
					#print("Reset with",x,desktop_w)
				if (y+h>desktop_h) or y<0:
					y=sain_y
					#print("Reset height",y)
				self.move(x,y)
				#print("moving to",x,y)
				found=True
				break

		if found==False:
			a=json_save_window()
			a.name=window_name
			a.x=sain_x
			a.y=sain_y
			self.move(a.x,a.y)
			data.windows.segments.append(a)
			data.save()
