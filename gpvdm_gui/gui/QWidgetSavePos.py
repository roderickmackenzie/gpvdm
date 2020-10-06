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

## @package QWidgetSavePos
#  A winndow base class which saves the window position.
#

from PyQt5.QtWidgets import QWidget

from window_list import wpos_update
from window_list import wpos_dump
from window_list import wpos_save
from window_list import wpos_set_window

class QWidgetSavePos(QWidget):

	def closeEvent(self, event):
		wpos_save()
		event.accept()
		
	def moveEvent(self,event):
		wpos_update(self,self.window_name)
		#wpos_dump()
		event.accept()

	def __init__(self,window_name):
		QWidget.__init__(self)
		self.window_name=window_name
		wpos_set_window(self,self.window_name)

