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

## @package gl_view_point
#  The gl_view_point class for the OpenGL display.
#

import sys
from math import fabs

from PyQt5.QtCore import QTimer


from epitaxy import get_epi

from gl_scale import scale_get_xmul
from gl_scale import scale_get_zmul
import os

class view_point():
	def __init__(self):
		self.xRot =25.0
		self.yRot =1.0
		self.zRot =0.0
		self.x_pos=0.0
		self.y_pos=0.
		self.zoom=16.0

		self.window_x=0.0
		self.window_y=0.0
		self.window_w=1.0
		self.window_h=1.0
		self.enable_view_move=True
		self.reset_shift_max_angles()
		self.name="default"
		self.enabled=True

	def reset_shift_max_angles(self):
		self.max_angle_shift=4.0

	def shift(self,target):
		if self.enabled==False:
			return True

		if self.name!="3d":
			return True

		stop=False
		move=0.0
		max_xy_shift=0.2
		delta=(target.xRot-self.xRot)
		if fabs(delta)>self.max_angle_shift:
			delta=self.max_angle_shift*delta/fabs(delta)

		self.xRot=self.xRot+delta
		move=move+fabs(delta)

		delta=(target.yRot-self.yRot)
		if fabs(delta)>self.max_angle_shift:
			delta=self.max_angle_shift*delta/fabs(delta)

		self.yRot=self.yRot+delta
		move=move+fabs(delta)

		delta=(target.zRot-self.zRot)
		if fabs(delta)>self.max_angle_shift:
			delta=self.max_angle_shift*delta/fabs(delta)

		self.zRot=self.zRot+delta
		move=move+fabs(delta)
		
		delta=(target.x_pos-self.x_pos)
		if fabs(delta)>max_xy_shift:
			delta=max_xy_shift*delta/fabs(delta)

		self.x_pos=self.x_pos+delta
		move=move+fabs(delta)
		
		delta=(target.y_pos-self.y_pos)
		if fabs(delta)>max_xy_shift:
			delta=max_xy_shift*delta/fabs(delta)

		self.y_pos=self.y_pos+delta
		move=move+fabs(delta)

		delta=(target.zoom-self.zoom)
		if fabs(delta)>1.0:
			delta=1.0*delta/fabs(delta)

		self.zoom=self.zoom+delta
		move=move+fabs(delta)
		
		if move==0.0:
			stop=True

		return stop

	def set_value(self,data):
		self.xRot=data.xRot
		self.yRot=data.yRot
		#self.zRot=data.zRot
		self.x_pos=data.x_pos
		self.y_pos=data.y_pos
		self.zoom=data.zoom

	def set_xy(self):
		self.xRot=3
		self.yRot=0.0
		self.zRot=0.0
		self.x_pos=0.0
		self.y_pos=0.0
		self.zoom=16
		self.name="xy"

	def set_xz(self):
		self.xRot=90.0
		self.yRot=90.0
		self.zRot=0.0
		self.x_pos=0.0
		self.y_pos=0.0
		self.zoom=16
		self.name="xz"

	def set_yz(self):
		self.xRot=3
		self.yRot=90
		self.zRot=0.0
		self.x_pos=0.0
		self.y_pos=0.0
		self.zoom=16
		self.name="yz"


