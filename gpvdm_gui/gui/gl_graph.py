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

## @package gl_graph
#  The gl_graph class for the OpenGL display.
#

import os
import sys
from math import fabs
from cal_path import get_sim_path

from dat_file import dat_file
from triangle import vec

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb
	from gl_list import gl_base_object
	from PIL import Image, ImageFilter,ImageOps, ImageDraw
except:
	pass

def clamp(val):
	val=fabs(val)
	if val>1.0:
		val=1.0
	return val

class gl_graph():

	def __init__(self):
		self.frac_max=1.0
		self.frac_min=0.0
		self.graph_data=[]

	def draw_graph(self):
		for data in self.graph_data:
			if data.valid_data==True:
				if data.type=="3d":
					if self.view_options.render_plot==True:
						self.draw_graph_3d(data)
				elif data.type=="poly":
					self.draw_graph_rays(data)

	def draw_graph_rays(self,data):
		if data.plotted==True:
			return

		o=gl_base_object()
		o.id=["ray_trace"]
		o.type="solid_and_mesh"

		xyz=vec()
		xyz.x=0.0#self.scale.project_m2screen_x(0.0)
		xyz.y=0.0#self.scale.project_m2screen_y(0.0)
		xyz.z=0.0#self.scale.project_m2screen_z(0.0)

		o.xyz.append(xyz)

		for t in data.data:

			x0=self.scale.project_m2screen_x(t.xyz0.x)
			x1=self.scale.project_m2screen_x(t.xyz1.x)

			y0=self.scale.project_m2screen_y(t.xyz0.y)
			y1=self.scale.project_m2screen_y(t.xyz1.y)

			z0=self.scale.project_m2screen_z(t.xyz0.z)
			z1=self.scale.project_m2screen_z(t.xyz1.z)

			o.triangles.append([x0, y0, z0])
			o.triangles.append([x1, y1, z1])

		self.gl_objects_add(o)
		self.objects[-1].compile("lines",[data.r,data.g,data.b,1.0],line_width=1)

		data.plotted=True

	def graph_project_3d_slice_to_image(self,data):
		
		image_file=os.path.splitext(data.file_name)[0]+"_render.png"

		if len(data.y_scale)>1 and len(data.x_scale)>1 and len(data.z_scale)>1:
			print("can't project truly 3d images yet") 

		len_x=len(data.x_scale)
		len_y=len(data.y_scale)
		len_z=len(data.z_scale)

		if os.path.isfile(image_file)==False:
			r_val=1.0
			g_val=0.0
			b_val=0.0
			if data.r!=None:
				r_val=data.r

			if data.g!=None:
				g_val=data.g

			if data.b!=None:
				b_val=data.b

			my_max,my_min=data.max_min()
			my_min=0.0
			#my_max=my_max*0.7
			#my_min=my_min*0.7

			if data.cols=="xyd":
				im= Image.new("RGBA", (len(data.x_scale), len(data.y_scale)), "#000000")
			elif data.cols=="yzd":
				im= Image.new("RGBA", (len(data.z_scale), len(data.y_scale)), "#000000")
			elif data.cols=="xzd":
				im= Image.new("RGBA", (len(data.x_scale), len(data.z_scale)), "#000000")
			my_max=my_max#5e17
			#y_min=0.0
			for xi in range(0,len_x):
				for yi in range(0,len_y):
					for zi in range(0,len_z):

						r=int(255*clamp((data.data[zi][xi][yi]-my_min)/(my_max-my_min))*r_val)
						g=int(255*clamp((data.data[zi][xi][yi]-my_min)/(my_max-my_min))*g_val)
						b=int(255*clamp((data.data[zi][xi][yi]-my_min)/(my_max-my_min))*b_val)
						#print(pos,data.data[zi][xi][yi],my_min,my_max)
						#if pos>len(self.color_map):
						#	pos=len(self.color_map)-1

						#rgb=self.color_map[pos]
				
						a=255
						if r==0 and g==0 and b==0:
							a=0

						if data.cols=="xyd":
							im.putpixel((xi,yi),(r, g, b,a))
						elif data.cols=="yzd":
							im.putpixel((zi,yi),(r, g, b,a))
						elif data.cols=="xzd":
							im.putpixel((xi,zi),(r, g, b,a))
			#print(image_file)
			im.save(image_file)

		#if slice_dir=="xy":
		zi=0

		z0=self.scale.project_m2screen_z(data.z_scale[0])
		z1=self.scale.project_m2screen_z(data.z_scale[len(data.z_scale)-1])
		x0=self.scale.project_m2screen_x(data.x_scale[0])
		x1=self.scale.project_m2screen_x(data.x_scale[len(data.x_scale)-1])
		y0=self.scale.project_m2screen_y(data.y_scale[0])
		y1=self.scale.project_m2screen_y(data.y_scale[len(data.y_scale)-1])

		o=self.gl_objects_find("graph_"+data.id)
		if o==None:
			o=gl_base_object()
			xyz=vec()
			xyz.x=x0
			xyz.y=y0
			xyz.z=z0			
			o.xyz.append(xyz)

			o.dxyz.x=x1-x0
			o.dxyz.y=y1-y0
			o.dxyz.z=z1-z0
			o.id=["graph_"+data.id]
			o.type="image"
			#o.alpha=0.0

			o.image_path=image_file
			self.gl_objects_add(o)


			self.gl_image.clear_lib()

	def draw_graph_3d(self,data):
		self.graph_project_3d_slice_to_image(data)


	def draw_mode(self):
		if self.false_color==True:
			return

		glLineWidth(5)
		glColor3f(1.0, 1.0, 1.0)

		t=[]
		s=[]

		data=dat_file()
		
		path=os.path.join(get_sim_path(),"optical_output","light_1d_photons_tot_norm.dat")

		if data.load(path)==True:
			glBegin(GL_LINES)
			array_len=data.y_len
			s=data.data[0][0]
			#print(path)
			#print(data.data)
			for i in range(1,array_len):
				x=self.scale.project_m2screen_x(0)
				y=self.scale.project_m2screen_y(data.y_scale[i-1])
				z=self.scale.project_m2screen_z(0)-s[i-1]*0.5
				glVertex3f(x, y, z)

				y=self.scale.project_m2screen_y(data.y_scale[i])
				z=self.scale.project_m2screen_z(0)-s[i]*0.5
				glVertex3f(x, y, z)

			glEnd()

