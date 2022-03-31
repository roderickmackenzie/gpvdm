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


## @package json_opengl_config
#  Store the cv domain json data
#


import sys
import os
import shutil
import json
from json_base import json_base
from json_gl import json_gl_view

class json_opengl_config(json_base):

	def __init__(self):
		json_base.__init__(self,"opengl")
		self.var_list=[]
		self.var_list.append(["gl_dy_layer_offset",0.1])
		self.var_list.append(["gl_device_height",1.4])
		self.var_list.append(["gl_render_text",True])
		self.var_list.append(["color_r",0.8])		#background color
		self.var_list.append(["color_g",0.8])
		self.var_list.append(["color_b",0.8])
		self.var_list.append(["color_alpha",0.8])
		self.var_list.append(["render_grid",True])
		self.var_list.append(["gl_view",json_gl_view()])
		self.var_list_build()

