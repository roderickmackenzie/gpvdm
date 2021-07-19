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

## @package gl_view_point
#  The gl_view_point class for the OpenGL display.
#

import sys
from math import fabs

from PyQt5.QtCore import QTimer


from epitaxy import get_epi

from gl_scale import set_m2screen
from gl_scale import scale_get_xmul
from gl_scale import scale_get_zmul
import os

class view_point():
	def __init__(self):
		self.xRot =25.0
		self.yRot =-20.0
		self.zRot =0.0
		self.x_pos=-0.5
		self.y_pos=-0.5
		self.zoom=16
		self.render_grid=True
		self.render_photons=True
		self.render_text=True
		self.render_plot=True
		self.draw_device=True
		self.optical_mode=True
		self.text=True
		self.bg_color=[0.0,0.0,0.0]
		self.stars_distance=60
		self.reset_shift_max_angles()

	def reset_shift_max_angles(self):
		self.max_angle_shift=4.0

	def shift(self,target):
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


class gl_move_view():

	def __init__(self):
		self.view=view_point()
		self.view.xRot =-25.0
		self.view.yRot =1.0
		self.view.zRot =0.0
		self.view.x_pos=0.0
		self.view.y_pos=0.
		self.view.zoom=16.0

		self.viewtarget=view_point()
		#self.viewtarget.set_value(self.view)
		self.viewtarget.xRot=0.0
		self.viewtarget.yRot=0.0
		self.viewtarget.zRot=0.0
		self.viewtarget.x_pos=-2.0
		self.viewtarget.y_pos=-1.7
		self.viewtarget.zoom=-8.0

		self.timer_save_files=False
		self.timer_save_files_number=0
		self.timer_end_callback=None

	def view_move_to_xy(self):
		self.viewtarget.xRot=-7
		self.viewtarget.yRot=0.0
		self.viewtarget.zRot=0.0
		self.viewtarget.x_pos=0.0
		self.viewtarget.y_pos=0.0
		self.viewtarget.zoom=16
		self.timer=QTimer()
		self.timer.timeout.connect(self.ftimer_target)
		self.timer.start(25)

	def view_move_to_yz(self):
		self.viewtarget.xRot=-7
		self.viewtarget.yRot=90
		self.viewtarget.zRot=0.0
		self.viewtarget.x_pos=0.0
		self.viewtarget.y_pos=0.0
		self.viewtarget.zoom=16
		self.timer=QTimer()
		self.timer.timeout.connect(self.ftimer_target)
		self.timer.start(25)

	def view_move_to_xz(self):
		self.viewtarget.xRot=-90.0
		self.viewtarget.yRot=90.0
		self.viewtarget.zRot=0.0
		self.viewtarget.x_pos=0.0
		self.viewtarget.y_pos=0.0
		self.viewtarget.zoom=16
		self.timer=QTimer()
		self.timer.timeout.connect(self.ftimer_target)
		self.timer.start(25)

	def view_push(self):
		self.stored_view=copy.copy(self.view)

	def view_pop(self):
		self.view=copy.copy(self.stored_view)
		
	def my_timer(self):
		#self.xRot =self.xRot + 2
		self.view.yRot =self.view.yRot + 2
		#self.zRot =self.zRot + 5
		
		self.update()


	def ftimer_target(self):
		stop=self.view.shift(self.viewtarget)
		
		self.update()
		if self.timer_save_files==True:
			if os.path.isdir("flyby")==False:
				os.mkdir("flyby")

			self.grabFrameBuffer().save(os.path.join("flyby",str(self.timer_save_files_number)+".jpg"))
			self.timer_save_files_number=self.timer_save_files_number+1

		if stop==True:
			self.timer.stop()
			if self.timer_end_callback!=None:
				self.timer_end_callback()
			self.timer_save_files=False
			self.timer_save_files_number=0
			

	def fzoom_timer(self):
		self.view.zoom =self.view.zoom+4.0
		if self.view.zoom>16.0:
			self.timer.stop()
		self.update()

	def start_rotate(self):
		self.timer=QTimer()
		self.timer.timeout.connect(self.my_timer)
		self.timer.start(50)



	def update_real_to_gl_mul(self):
		set_m2screen()

	def view_dump(self):
		print("xRot=",self.view.xRot)
		print("yRot=",self.view.yRot)
		print("zRot=",self.view.zRot)
		print("x_pos=",self.view.x_pos)
		print("y_pos=",self.view.y_pos)
		print("zoom=",self.view.zoom)

