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

## @package gl_list
#  An object store from which the 3d scene is rendered
#

import sys
import glob

from OpenGL.GLU import *
from OpenGL.GL import *
from gl_lib import gl_obj_id_starts_with
from gl_base_object import gl_base_object
from epitaxy import get_epi
from gl_scale import gl_scale
from shape import shape
from triangle import vec

from gl_scale import scale_get_xmul
from gl_scale import scale_get_ymul
from gl_scale import scale_get_zmul
from gpvdm_json import gpvdm_data
import numpy as np
from json_light_sources import json_light_source

class gl_objects():

	def __init__(self):
		self.objects=[]

		self.gl_compiled=False

	def gl_objects_clear(self):
		self.objects=[]

		for data in self.graph_data:
			data.plotted=False

		self.ray_data.plotted=False

	def gl_object_deselect_all(self):
		for o in self.objects:
			o.selected=False

	def gl_objects_is_selected(self):
		for i in range(0,len(self.objects)):
			if self.objects[i].selected==True:
				return self.objects[i]
		return False

	def gl_objects_selected_min_max_vec(self):
		min=vec()
		min.x=1000
		min.y=1000
		min.z=1000

		max=vec()
		max.x=-1
		max.y=-1
		max.z=-1

		for o in self.objects:
			if o.selected==True:
				#min
				for xyz in o.xyz:
					if xyz.x<min.x:
						min.x=xyz.x

					if xyz.x+o.dxyz.x<min.x:
						min.x=xyz.x+o.dxyz.x

					if xyz.y<min.y:
						min.y=xyz.y

					#print("screen",o.dxyz.y)
					if xyz.y+o.dxyz.y<min.y:
						min.y=xyz.y+o.dxyz.y

					if xyz.z<min.z:
						min.z=xyz.z

					if xyz.z+o.dxyz.z<min.z:
						min.z=xyz.z+o.dxyz.z

					#max
					if xyz.x>max.x:
						max.x=xyz.x

					if xyz.x+o.dxyz.x>max.x:
						max.x=xyz.x+o.dxyz.x

					if xyz.y>max.y:
						max.y=xyz.y

					if xyz.y+o.dxyz.y>max.y:
						max.y=xyz.y+o.dxyz.y

					if xyz.z>max.z:
						max.z=xyz.z

					if xyz.z+o.dxyz.z>max.z:
						max.z=xyz.z+o.dxyz.z

		return min,max

	def gl_objects_add(self,my_object):
		if type(my_object.id)==str:
			print("id should be an array not a string")
			adsasddsa

		rgb_int=len(self.objects)+1
		my_object.r_false =  rgb_int & 255
		my_object.g_false = (rgb_int >> 8) & 255
		my_object.b_false =   (rgb_int >> 16) & 255
	
		my_object.r_false=my_object.r_false/255
		my_object.g_false=my_object.g_false/255
		my_object.b_false=my_object.b_false/255
		self.objects.append(my_object)
		self.gl_compiled=False

	def gl_objects_dump(self):
		for o in self.objects:
			print(o.type)
		print(len(self.objects))

	def gl_objects_rotate(self,rotate_x,rotate_y):
		for obj in self.objects:
			if obj.selected==True:
				if obj.moveable==True:
					s=gpvdm_data().find_object_by_id(obj.id[0])
					if type(s)==shape or type(s)==json_light_source:
						obj.rotate_x=obj.rotate_x+rotate_x
						s.rotate_x=obj.rotate_x

						obj.rotate_y=obj.rotate_y+rotate_y
						s.rotate_y=obj.rotate_y
						#print(obj.rotate_y)

	def gl_objects_move(self,dx,dy,dz):
		i=0
		epi=get_epi()

		min,max=self.gl_objects_selected_min_max_vec()

		x_min_new_screen=min.x+dx
		x_min_new_m=self.scale.project_screen_x_to_m(x_min_new_screen)

		x_max_new_screen=max.x+dx
		x_max_new_m=self.scale.project_screen_x_to_m(x_max_new_screen)

		y_min_new_screen=min.y+dy
		y_max_new_m=self.scale.project_screen_y_to_m(y_min_new_screen)

		y_max_new_screen=max.y+dy
		y_min_new_m=self.scale.project_screen_y_to_m(y_max_new_screen)

		move_x=True
		move_y=True
		move_z=True
		#for v in self.views:
		#	if v.enabled==True:
		#		print(v.xRot,v.yRot,v.zRot)
		if 1==0:
			for obj in self.objects:
				if obj.selected==True:
					s=None

					if len(obj.id)>0:
						s=gpvdm_data().find_object_by_id(obj.id[0])
						if type(s)==shape:
							nl=epi.find_layer_by_id(obj.id[0])
							if nl!=None:
								y_start=epi.get_layer_start(nl)
								y_stop=epi.get_layer_end(nl)

								x_stop=gpvdm_data().mesh.mesh_x.get_len()

								if x_min_new_m<0:
									move_x=False

								if y_min_new_m<y_start:
									move_y=False

								if y_max_new_m>=y_stop:
									move_y=False

								if x_max_new_m>=x_stop:
									move_x=False
					else:
						move_x=False
						move_y=False


		for obj in self.objects:
			if obj.selected==True:
				if obj.moveable==True:
					s=gpvdm_data().find_object_by_id(obj.id[0])

					for xyz in obj.xyz:
						if move_y==True:
							xyz.y=xyz.y+dy

						if move_x==True:
							xyz.x=xyz.x+dx
	
						if move_z==True:
							xyz.z=xyz.z+dz

					if move_y==True:
						s.y0=s.y0+dy/scale_get_ymul()

					if move_x==True:
						s.x0=s.x0+dx/scale_get_xmul()

					if move_z==True:
						s.z0=s.z0+dz/scale_get_zmul()

	def gl_objects_save_selected(self):
		epi=get_epi()
		moved=False
		for obj in self.objects:
			if obj.selected==True:
				if len(obj.id)>0:
					moved=True
		if moved==True:
			gpvdm_data().save()

	def gl_objects_count_regex(self,in_id):
		count=0
		for i in range(0,len(self.objects)):
			for id in self.objects[i].id: 	
				if id.startswith(in_id)==False:
					counnt=count+1

		return count

	def gl_objects_find(self,in_id):
		count=0
		for i in range(0,len(self.objects)):
			for id in self.objects[i].id: 
				if id==in_id:
					return self.objects[i]

		return None

	def gl_objects_remove_regex(self,in_id):
		new_objects=[]
		for i in range(0,len(self.objects)):
			if gl_obj_id_starts_with(self.objects[i].id,in_id)==False:
				new_objects.append(self.objects[i])

		self.objects=new_objects


	def gl_objects_remove(self,in_id):
		for i in range(0,len(self.objects)):
			if in_id in self.objects[i].id:
				self.objects.pop(i)
				break

	def gl_objects_select_by_id(self,in_id):
		for obj in self.objects:
			if obj.id==in_id:
				obj.selected=True

	def gl_compile(self):
		pass

	def gl_objects_render(self):
		if self.gl_compiled==False:
			self.gl_compile()

		for o in self.objects:
			if o.type=="plane":
				self.plane(o)
			if o.type=="ball":
				self.paint_ball(o)
			elif o.type=="ray":
				self.raw_ray(o)
			elif o.type=="line":
				self.paint_line(o)
			elif o.type=="resistor":
				self.paint_resistor(o)
			elif o.type=="diode":
				self.paint_diode(o)
			elif o.type=="open_triangles":
				self.paint_open_triangles_from_array(o)
			elif o.type=="solid":
				self.paint_from_array(o)
			elif o.type=="solid_and_mesh":
				if self.view_options.transparent_objects==False:
					#print(o.id)
					self.paint_from_array(o)
				else:
					self.paint_open_triangles_from_array(o,false_color=False,line_width=2)
			elif o.type=="solid_and_mesh_cut_through":
				self.paint_from_array_cut_through(o)
			elif o.type=="box":
				self.box(o)
			elif o.type=="marker":
				self.paint_marker(o)
			elif o.type=="arrow":
				self.paint_arrow(o)
			elif o.type=="marker_small":
				self.paint_marker_small(o)
			elif o.type=="box_cut_through":
				self.box(o,cut_through=True)
			elif o.type=="text":
				self.gl_render_text(o)

			if o.selected==True:
				sel=gl_base_object()
				sel.copy(o)
				sel.r=1.0
				sel.g=0.0
				sel.b=0.0
				self.gl_render_box_lines(sel)



	def gl_objects_search_by_color(self,r,g,b):
		#i=0
		#for o in self.objects:
		#	print(">>>>",i,o.type,o.id,o.r_false,o.g_false,o.b_false,r,g,b)
		#	i=i+1
		#print("")

		for o in self.objects:
			if o.match_false_color(r,g,b)==True:
				return o
		return None

	def gl_objects_get_first_selected(self):
		for o in self.objects:
			if o.selected==True:
				return o
		return None

	def gl_objects_save(self,file_name):
		f=open(file_name,"w")
		f.write("#gobj\n")
		for o in self.objects:
			ret=""
			ret=ret+o.type

			ret=ret+";;"+"{:e}".format(self.scale.project_screen_x_to_m(o.xyz.x))
			ret=ret+";;"+"{:e}".format(self.scale.project_screen_y_to_m(o.xyz.y))
			ret=ret+";;"+"{:e}".format(self.scale.project_screen_z_to_m(o.xyz.z))
			ret=ret+";;"+"{:e}".format(o.dxyz.x/scale_get_xmul())
			ret=ret+";;"+"{:e}".format(o.dxyz.y/scale_get_ymul())
			ret=ret+";;"+"{:e}".format(o.dxyz.z/scale_get_zmul())

			ret=ret+";;"+str(o.r)
			ret=ret+";;"+str(o.g)
			ret=ret+";;"+str(o.b)

			ret=ret+";;"+str(o.alpha)
			ret=ret+";;"+str(o.selected)
			ret=ret+";;"+str(o.selectable)
			ret=ret+";;"+str(o.moveable)
			ret=ret+";;"+str(o.allow_cut_view)

			ret=ret+";;"+str(o.text)

			f.write(ret+"\n")

		f.close()


	def gl_objects_load(self,objs):
		#self.objects=[]
		for o in objs:
			self.gl_objects_add(o)


