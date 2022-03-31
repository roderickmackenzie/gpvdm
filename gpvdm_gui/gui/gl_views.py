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

## @package gl_views
#  The gl_views class for the OpenGL display.
#

import sys
from math import fabs

from PyQt5.QtCore import QTimer


from epitaxy import get_epi

from json_gl import json_gl_view
import os

class gl_views():

	def __init__(self):
		self.views=[]
		v=json_gl_view()
		v.window_x=0.0
		v.window_y=0.0
		v.window_w=0.5
		v.window_h=0.5
		v.name="3d"
		self.views.append(v)

		v=json_gl_view()
		v.window_x=0.5
		v.window_y=0.5
		v.window_w=0.5
		v.window_h=0.5
		v.enable_view_move=False
		v.set_xy()
		self.views.append(v)

		v=json_gl_view()
		v.window_x=0.0
		v.window_y=0.5
		v.window_w=0.5
		v.window_h=0.5
		v.enable_view_move=False
		v.set_xz()
		self.views.append(v)

		v=json_gl_view()
		v.window_x=0.5
		v.window_y=0.0
		v.window_w=0.5
		v.window_h=0.5
		v.enable_view_move=False
		v.set_yz()
		self.views.append(v)

		self.viewtarget=json_gl_view()

		self.viewtarget.xRot=0.0
		self.viewtarget.yRot=0.0
		self.viewtarget.zRot=0.0
		self.viewtarget.x_pos=-2.0
		self.viewtarget.y_pos=-1.7
		self.viewtarget.zoom=-8.0

		self.timer_save_files=False
		self.timer_save_files_number=0
		self.timer_end_callback=None
		self.focus_on_view("3d")

	def focus_on_view(self,name):
		for v in self.views:
			v.enabled=False
			if v.name==name:
				v.enabled=True
				v.window_x=0.0
				v.window_y=0.0
				v.window_w=1.0
				v.window_h=1.0

	def defocus_view(self,name):
		for v in self.views:
			v.enabled=True
			if v.name==name:
				v.enabled=True
				v.window_x=0.0
				v.window_y=0.0
				v.window_w=0.5
				v.window_h=0.5

	def view_move_to_xy(self):
		self.focus_on_view("3d")
		self.viewtarget.set_xy()
		self.timer=QTimer()
		self.timer.timeout.connect(self.ftimer_target)
		self.timer.start(25)

	def view_move_to_yz(self):
		self.focus_on_view("3d")
		self.viewtarget.set_yz()
		self.timer=QTimer()
		self.timer.timeout.connect(self.ftimer_target)
		self.timer.start(25)

	def view_move_to_xz(self):
		self.focus_on_view("3d")
		self.viewtarget.set_xz()
		self.timer=QTimer()
		self.timer.timeout.connect(self.ftimer_target)
		self.timer.start(25)

	def view_move_to_orthographic(self):
		self.defocus_view("3d")
		self.force_redraw()

	def view_push(self):
		self.stored_views=[]
		for v in self.views:
			self.stored_views.append(v)

	def view_pop(self):
		self.views=[]
		for v in self.views:
			self.view.append(v)
		
	def my_timer(self):
		#self.xRot =self.xRot + 2
		for v in self.views:
			v.yRot =v.yRot + 2
		#self.zRot =self.zRot + 5
		
		self.update()


	def ftimer_target(self):
		stopped=0
		for v in self.views:
			stop=v.shift(self.viewtarget)
			if stop==True:
				stopped=stopped+1

			self.update()
			if self.timer_save_files==True:
				if os.path.isdir("flyby")==False:
					os.mkdir("flyby")

				self.grabFrameBuffer().save(os.path.join("flyby",str(self.timer_save_files_number)+".jpg"))
				self.timer_save_files_number=self.timer_save_files_number+1

			if stopped==len(self.views):
				self.timer.stop()
				if self.timer_end_callback!=None:
					self.timer_end_callback()

	def view_count_enabled(self):
		enabled=0
		for v in self.views:
			if v.enabled==True:
				enabled=enabled+1

		return enabled

	def fzoom_timer(self):
		for v in self.views:
			v.zoom =v.zoom+4.0
			if v.zoom>16.0:
				self.timer.stop()
			self.update()

	def start_rotate(self):
		self.timer=QTimer()
		self.timer.timeout.connect(self.my_timer)
		self.timer.start(50)


	def view_dump(self):
		for v in self.views:
			print("xRot=",v.xRot)
			print("yRot=",v.yRot)
			print("zRot=",v.zRot)
			print("x_pos=",v.x_pos)
			print("y_pos=",v.y_pos)
			print("zoom=",v.zoom)

class gl_view_options():

	def __init__(self):
		self.render_grid=True
		self.render_fdtd_grid=True
		self.render_cords=True
		self.render_photons=True
		self.render_plot=True
		self.draw_device=True
		self.optical_mode=True
		self.enable_draw_light_source=True
		self.show_world_box=False
		self.text=True
		self.bg_color=[0.0,0.0,0.0]
		self.stars_distance=60
		self.transparent_objects=False
		self.draw_rays=True
