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

from gl_scale import scale_screen_x2m
from gl_scale import scale_screen_y2m

from OpenGL.GLU import *
from OpenGL.GL import *
from gl_lib import gl_obj_id_starts_with
from gl_base_object import gl_base_object
from epitaxy import get_epi
from gl_scale import gl_scale
from shape import shape
from triangle import vec

from mesh import get_mesh
from gl_scale import scale_get_xmul
from gl_scale import scale_get_ymul
from gl_scale import scale_get_zmul
import numpy as np

class gl_objects():

	def __init__(self):
		self.objects=[]

	def gl_objects_clear(self):
		self.objects=[]
		self.gl_array_lines=[]
		self.gl_array_lines_float32=[]

		self.gl_array_colors=[]
		self.gl_array_colors_float32=[]

		for data in self.graph_data:
			data.plotted=False

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

		for i in range(0,len(self.objects)):
			if self.objects[i].selected==True:
				#min
				if self.objects[i].xyz.x<min.x:
					min.x=self.objects[i].xyz.x

				if self.objects[i].xyz.x+self.objects[i].dxyz.x<min.x:
					min.x=self.objects[i].xyz.x+self.objects[i].dxyz.x

				if self.objects[i].xyz.y<min.y:
					min.y=self.objects[i].xyz.y

				#print("screen",self.objects[i].dxyz.y)
				if self.objects[i].xyz.y+self.objects[i].dxyz.y<min.y:
					min.y=self.objects[i].xyz.y+self.objects[i].dxyz.y

				if self.objects[i].xyz.z<min.z:
					min.z=self.objects[i].xyz.z

				if self.objects[i].xyz.z+self.objects[i].dxyz.z<min.z:
					min.z=self.objects[i].xyz.z+self.objects[i].dxyz.z

				#max
				if self.objects[i].xyz.x>max.x:
					max.x=self.objects[i].xyz.x

				if self.objects[i].xyz.x+self.objects[i].dxyz.x>max.x:
					max.x=self.objects[i].xyz.x+self.objects[i].dxyz.x

				if self.objects[i].xyz.y>max.y:
					max.y=self.objects[i].xyz.y

				if self.objects[i].xyz.y+self.objects[i].dxyz.y>max.y:
					max.y=self.objects[i].xyz.y+self.objects[i].dxyz.y

				if self.objects[i].xyz.z>max.z:
					max.z=self.objects[i].xyz.z

				if self.objects[i].xyz.z+self.objects[i].dxyz.z>max.z:
					max.z=self.objects[i].xyz.z+self.objects[i].dxyz.z

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

	def gl_objects_dump(self):
		for o in self.objects:
			print(o.type)
		print(len(self.objects))

	def gl_objects_move(self,dx,dy):
		i=0
		epi=get_epi()

		min,max=self.gl_objects_selected_min_max_vec()

		x_min_new_screen=min.x+dx
		x_min_new_m=gl_scale.project_screen_x_to_m(x_min_new_screen)

		x_max_new_screen=max.x+dx
		x_max_new_m=gl_scale.project_screen_x_to_m(x_max_new_screen)

		y_min_new_screen=min.y+dy
		y_max_new_m=gl_scale.project_screen_y_to_m(y_min_new_screen)

		y_max_new_screen=max.y+dy
		y_min_new_m=gl_scale.project_screen_y_to_m(y_max_new_screen)

		move_x=True
		move_y=True

		for obj in self.objects:
			if obj.selected==True:
				s=None

				if len(obj.id)>0:
					s=epi.find_object_by_id(obj.id[0])

					if type(s)==shape:

						nl=epi.find_layer_by_id(obj.id[0])

						y_start=epi.get_layer_start(nl)
						y_stop=epi.get_layer_end(nl)

						x_stop=get_mesh().x.get_len()

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

		if move_y==True:
			for obj in self.objects:
				if obj.selected==True:
					s=epi.find_object_by_id(obj.id[0])
					if type(s)==shape:
						obj.xyz.y=obj.xyz.y+dy
						s.y0=gl_scale.project_screen_y_to_m(obj.xyz.y)

		if move_x==True:
			for obj in self.objects:
				if obj.selected==True:
					s=epi.find_object_by_id(obj.id[0])
					if type(s)==shape:
						obj.xyz.x=obj.xyz.x+dx

						if obj.origonal_object==True:
							s.x0=gl_scale.project_screen_x_to_m(obj.xyz.x)
							#print(s.x0)

	def gl_objects_save_selected(self):
		epi=get_epi()
		for obj in self.objects:
			if obj.selected==True:
				if len(obj.id)>0:
					s=epi.find_object_by_id(obj.id[0])
					if type(s)==shape:
						s.save()

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

	def gl_objects_render(self):
		#print("----")
		#self.gl_objects_dump()
		#print(len(self.objects))
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
				#print("solid_and_mesh>>",o.z)
				if self.view_options.transparent_objects==False:
					self.paint_from_array(o)
					self.paint_open_triangles_from_array(o)
				else:
					self.paint_open_triangles_from_array(o,false_color=False,line_width=2)
			elif o.type=="solid_and_mesh_cut_through":
				self.paint_from_array_cut_through(o)
			elif o.type=="solid_and_mesh_color":
				self.paint_from_array(o)
				self.paint_open_triangles_from_array(o,colored=True)
			elif o.type=="box":
				#print("box>>",o.z)
				self.box(o)
			elif o.type=="marker":
				self.paint_marker(o)
			elif o.type=="marker_small":
				self.paint_marker_small(o)
			elif o.type=="box_cut_through":
				self.box(o,cut_through=True)
			elif o.type=="grid":
				self.gl_render_grid(o)
			elif o.type=="text":
				self.gl_render_text(o)
			else:
				self.paint_from_array(o)
				self.paint_open_triangles_from_array(o)

			#print(o.selected,o.selectable,o.id)

			if o.selected==True:
				sel=gl_base_object()
				sel.copy(o)
				sel.r=1.0
				sel.g=0.0
				sel.b=0.0

				#sel.dy=sel.dy*1.1
				#sel.dz=sel.dz*1.1
				#print(sel.dxyz.z,sel.dxyz.x,sel.dxyz.y)
				#print(sel.text)
				self.gl_render_box_lines(sel)
				#print("!!!!!!!!!!")
			#if o.text!="":
				#print(o.type,o.xyz.y)
				#self.gl_render_text(o)


		if len(self.gl_array_lines_float32)==0:
			self.gl_array_lines_float32=np.array(self.gl_array_lines, dtype='float32')
			#print(self.gl_array_colors)
			self.gl_array_colors_float32=np.array(self.gl_array_colors, dtype='float32')
			#print(self.gl_array_colors)

		if len(self.gl_array_lines_float32)>0:
			glLineWidth(2)
			glEnableClientState(GL_VERTEX_ARRAY)
			glVertexPointer(3, GL_FLOAT, 0, self.gl_array_lines_float32)

			glEnableClientState(GL_COLOR_ARRAY)
			glColorPointer(4, GL_FLOAT, 0, self.gl_array_colors_float32)
			glDrawArrays(GL_LINES, 0, len(self.gl_array_lines))

		#self.gl_objects_save("out.dat")
		#self.gl_objects_load("out.dat")
	def gl_objects_search_by_color(self,r,g,b):
		for o in self.objects:
			#print(o.r_false,r,o.g_false,g,o.b_false,b)
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

			ret=ret+";;"+"{:e}".format(gl_scale.project_screen_x_to_m(o.xyz.x))
			ret=ret+";;"+"{:e}".format(gl_scale.project_screen_y_to_m(o.xyz.y))
			ret=ret+";;"+"{:e}".format(gl_scale.project_screen_z_to_m(o.xyz.z))
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

			ret=ret+";;"+str(o.origonal_object)
			f.write(ret+"\n")

		f.close()

	def gl_objects_load(self,objs):
		#self.objects=[]
		for o in objs:
			self.gl_objects_add(o)


