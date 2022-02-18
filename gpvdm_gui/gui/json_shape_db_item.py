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


## @package json_shape_db_item
#  Store the shape information
#


from json_base import json_base

class json_shape_db_threshold(json_base):

	def __init__(self):
		json_base.__init__(self,"threshold")
		self.var_list=[]
		self.var_list.append(["threshold_enabled",False])
		self.var_list.append(["threshold_value",200])
		self.var_list_build()

class json_shape_db_blur(json_base):

	def __init__(self):
		json_base.__init__(self,"blur")
		self.var_list=[]
		self.var_list.append(["shape_import_blur_enabled",False])
		self.var_list.append(["shape_import_blur",10])
		self.var_list_build()

class json_shape_db_mesh(json_base):

	def __init__(self):
		json_base.__init__(self,"mesh")
		self.var_list=[]
		self.var_list.append(["mesh_show",True])
		self.var_list.append(["mesh_gen_nx",20])
		self.var_list.append(["mesh_gen_ny",20])
		self.var_list.append(["mesh_gen_opp","node_reduce"])
		self.var_list.append(["mesh_min_ang",25])
		self.var_list_build()

class json_shape_boundary(json_base):

	def __init__(self):
		json_base.__init__(self,"boundary")
		self.var_list=[]
		self.var_list.append(["boundary_enabled",False])
		self.var_list.append(["image_boundary_x0",0])
		self.var_list.append(["image_boundary_x0_color","1.0,1.0,1.0,1.0"])
		self.var_list.append(["image_boundary_x1",0])
		self.var_list.append(["image_boundary_x1_color","1.0,1.0,1.0,1.0"])
		self.var_list.append(["image_boundary_y0",0])
		self.var_list.append(["image_boundary_y0_color","1.0,1.0,1.0,1.0"])
		self.var_list.append(["image_boundary_y1",0])
		self.var_list.append(["image_boundary_y1_color","1.0,1.0,1.0,1.0"])
		self.var_list_build()

class json_shape_db_item_import(json_base):

	def __init__(self):
		json_base.__init__(self,"import_config")
		self.var_list=[]
		self.var_list.append(["shape_import_y_norm",False])
		self.var_list.append(["shape_import_z_norm",False])
		self.var_list.append(["shape_import_y_norm_percent",2])
		self.var_list.append(["shape_import_rotate",0])
		self.var_list_build()

class json_shape_db_item_honeycomb(json_base):

	def __init__(self):
		json_base.__init__(self,"honeycomb")
		self.var_list=[]
		self.var_list.append(["honeycomb_dx",30])
		self.var_list.append(["honeycomb_dy",30])
		self.var_list.append(["honeycomb_x_shift",0])
		self.var_list.append(["honeycomb_y_shift",0])
		self.var_list.append(["honeycomb_line_width",10])
		self.var_list.append(["honeycomb_rotate",0])
		self.var_list_build()

class json_shape_db_item_xtal(json_base):

	def __init__(self):
		json_base.__init__(self,"xtal")
		self.var_list=[]
		self.var_list.append(["xtal_dr",10])
		self.var_list.append(["xtal_dx",30])
		self.var_list.append(["xtal_dy",30])
		self.var_list.append(["xtal_offset",30])
		self.var_list_build()

class json_shape_db_item_lens(json_base):

	def __init__(self):
		json_base.__init__(self,"lens")
		self.var_list=[]
		self.var_list.append(["lens_type","convex"])
		self.var_list_build()

class json_shape_db_item_gaus(json_base):

	def __init__(self):
		json_base.__init__(self,"gauss")
		self.var_list=[]
		self.var_list.append(["gauss_sigma",100.0])
		self.var_list.append(["gauss_offset_x",0])
		self.var_list.append(["gauss_offset_y",0])
		self.var_list_build()

class shape_db_item(json_base):

	def __init__(self):
		json_base.__init__(self,"shape_db_item")
		self.var_list=[]
		self.var_list.append(["item_type","shape"])
		self.var_list.append(["color_r",0.8])
		self.var_list.append(["color_g",0.8])
		self.var_list.append(["color_b",0.8])
		self.var_list.append(["image_ylen",200])
		self.var_list.append(["image_xlen",200])
		self.var_list.append(["color_alpha",0.8])
		self.var_list.append(["status","public"])
		self.var_list.append(["gauss",json_shape_db_item_gaus()])
		self.var_list.append(["honeycomb",json_shape_db_item_honeycomb()])
		self.var_list.append(["xtal",json_shape_db_item_xtal()])
		self.var_list.append(["lens",json_shape_db_item_lens()])
		self.var_list.append(["import_config",json_shape_db_item_import()])
		self.var_list.append(["boundary",json_shape_boundary()])
		self.var_list.append(["mesh",json_shape_db_mesh()])
		self.var_list.append(["blur",json_shape_db_blur()])
		self.var_list.append(["threshold",json_shape_db_threshold()])

		self.var_list_build()
		self.include_name=False
