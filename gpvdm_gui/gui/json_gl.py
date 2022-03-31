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


## @package json_jv
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base
from math import fabs

class json_gl_view(json_base):

	def __init__(self):
		json_base.__init__(self,"gl_view")
		self.var_list=[]
		self.var_list.append(["xRot",25.0])
		self.var_list.append(["yRot",1.0])
		self.var_list.append(["zRot",0.0])
		self.var_list.append(["x_pos",0.0])
		self.var_list.append(["y_pos",0.0])
		self.var_list.append(["zoom",16])
		self.var_list.append(["id",self.random_id()])
		self.var_list.append(["english_name","view"])
		self.var_list.append(["window_x",0.0])
		self.var_list.append(["window_y",0.0])
		self.var_list.append(["window_w",0.0])
		self.var_list.append(["window_h",0.0])
		self.var_list.append(["enable_view_move",True])
		self.var_list_build()
		self.reset_shift_max_angles()

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


class json_gl(json_base):
	def __init__(self):
		json_base.__init__(self,"gl",segment_class=True,segment_example=json_gl_view())

	def load_from_json(self,json):
		self.segments=[]
		try:
			segs=json['segments']
		except:
			self.segments.append(json_gl_view())
			self.segments.append(json_gl_view())
			return
		for i in range(0,segs):
			a=json_gl_view()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)
