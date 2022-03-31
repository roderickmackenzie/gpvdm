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

## @package gl
#  The main OpenGL display.
#

import sys
open_gl_ok=False

try:
	from OpenGL.GLU import *
	from OpenGL.GL import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from OpenGL.GLU import *
	open_gl_ok=True
except:

	print("opengl error ",sys.exc_info()[0])

from PyQt5 import QtGui
from PyQt5.QtGui import QScreen
from PyQt5.QtWidgets import QWidget, QHBoxLayout, QMenu, QColorDialog, QAction

import os

#path
from cal_path import get_sim_path


#epitaxy
from epitaxy import epitaxy_get_epi
from epitaxy import get_epi

#qt
from PyQt5.QtGui import QFont
from PyQt5.QtGui import qRgba

from PyQt5.QtGui import QPainter,QFont,QColor,QPen
from PyQt5.QtGui import QPainterPath,QPolygonF
from PyQt5.QtCore import QRectF,QPoint

import numpy as np
from str2bool import str2bool

import random

from math import fabs
from triangle import vec
from gl_fallback import gl_fallback

import copy

from thumb import thumb_nail_gen

from gl_views import gl_views
from gl_views import gl_view_options

from gl_lib import val_to_rgb

from gl_mesh import gl_mesh


from gl_object_editor import gl_object_editor

from gl_cords import gl_cords

from gl_shape_layer import shape_layer
from gl_base_widget import gl_base_widget

from gl_main_menu import gl_main_menu

from gl_list import gl_objects

#from file_watch import get_watch
from gl_input import gl_input

from gl_text import gl_text
from gl_image import gl_image

from gl_lib_ray import gl_lib_ray
from gl_contacts import gl_contacts
from gl_graph import gl_graph
from gl_draw_light_profile import gl_draw_light_profile
from gl_color import gl_color
from gl_shapes import gl_shapes
from gl_base_object import gl_base_object
from gl_render_obj import gl_render_obj
from gl_photons import gl_photons
from gl_scale import gl_scale
from PyQt5.QtCore import pyqtSignal
from gpvdm_json import gpvdm_data
from gl_toolbar import gl_toolbar
from gl_default_shapes import gl_default_shapes

class open_gl_light:
	def __init__(self):
		self.xyz=[0, 5, -10, 1.0]
		self.number=GL_LIGHT0

if open_gl_ok==True:		
	class glWidget(QGLWidget,shape_layer, gl_lib_ray,gl_objects, gl_text,gl_views,gl_mesh,gl_object_editor,gl_cords,gl_base_widget,gl_main_menu,gl_input, gl_contacts, gl_draw_light_profile, gl_graph, gl_color, gl_shapes, gl_render_obj, gl_photons, gl_toolbar):


		text_output = pyqtSignal(str)

		def __init__(self, parent):
			QGLWidget.__init__(self, parent)

			gl_base_widget.__init__(self)
			gl_objects.__init__(self)
			gl_lib_ray.__init__(self)
			gl_text.__init__(self)
			gl_color.__init__(self)
			gl_render_obj.__init__(self)
			gl_input.__init__(self)
			gl_graph.__init__(self)
			gl_toolbar.__init__(self)
			gl_views.__init__(self)
			self.lit=True
			self.setAutoBufferSwap(False)
			self.gl_image=gl_image()
			self.lights=[]
			self.view_options=gl_view_options()


			l=open_gl_light()
			l.xyz=[0, 5, -10]
			l.number=GL_LIGHT0
			self.lights.append(l)

			l=open_gl_light()
			l.xyz=[0, -5, -10]
			l.number=GL_LIGHT1
			self.lights.append(l)


			l=open_gl_light()
			l.xyz=[0, 5, 10]
			l.number=GL_LIGHT2
			self.lights.append(l)

			l=open_gl_light()
			l.xyz=[0, -5, 10]
			l.number=GL_LIGHT3
			self.lights.append(l)

			l=open_gl_light()
			l.xyz=[-10, -5, 0]
			l.number=GL_LIGHT4
			self.lights.append(l)

			l=open_gl_light()
			l.xyz=[10, -5, 0]
			l.number=GL_LIGHT5
			self.lights.append(l)



			self.failed=True
			self.graph_path=None
			self.scene_built=False
			#view pos

			self.dy_layer_offset=0.05

			self.draw_electrical_mesh=False
			self.enable_draw_ray_mesh=False
			self.plot_graph=False
			self.plot_circuit=False

			self.scale=gl_scale()
			self.font = QFont("Arial")
			self.font.setPointSize(15)
			self.called=False
			self.enable_light_profile=True
			self.build_main_menu()
			self.pre_built_scene=None
			self.open_gl_working=True
			self.default_shapes=gl_default_shapes()

		#def bix_axis(self):
		#	for xx in range(0,10):
		#		self.box(0+xx,0,0,0.5,0.5,0.5,1.0,0,0,0.5)

		#	for yy in range(0,10):
		#		self.box(0,0+yy,0,0.5,0.5,0.5,1.0,0,0,0.5)


		#	for zz in range(0,10):
		#		self.box(0,0,0+zz,0.5,0.5,0.5,0.0,0,1,0.5)

		#this may not be the best place for this
		def epitaxy_enforce_rules(self):
			y_pos=0.0
			epi=get_epi()
			for l in epi.layers:
				l.shape_dos.enabled=False
				if l.layer_type=="active":
					l.shape_dos.enabled=True
				l.x0=0.0
				l.z0=0.0
				l.y0=y_pos

				l.dx=gpvdm_data().mesh.mesh_x.get_len()
				l.dz=gpvdm_data().mesh.mesh_z.get_len()
				y_pos=y_pos+l.dy

		def draw_device2(self,x,z):
			epi=get_epi()
			contact_layers=epi.contacts.get_layers_with_contacts()
			top_contact_layer=epi.get_top_contact_layer()
			btm_contact_layer=epi.get_btm_contact_layer()

			l=0
			btm_layer=len(epitaxy_get_epi())-1

			for obj in gpvdm_data().world.world_data.segments:
				self.shape_to_screen(obj)

			self.epitaxy_enforce_rules()

			for obj in epi.layers:
				name=obj.shape_name
				display_name=name
				#alpha=obj.alpha
				#if len(obj.shapes)>0:
				

				contact_layer=False
				if l==top_contact_layer:
					contact_layer=True

				if l==btm_contact_layer:
					contact_layer=True

				#print(l,top_contact_layer,btm_contact_layer)
				#print(">>>>",l,contact_layer,contact_layers)
#				print(obj.shape_name)
				if contact_layer==False:
					#print(obj.id,name,obj.y0,obj.dy)
					self.shape_to_screen(obj,epitaxy=True)			

				if obj.layer_type=="active":
					if self.view_options.text==True:
						o=gl_base_object()
						xyz=vec()
						xyz.x=self.scale.project_m2screen_x(gpvdm_data().mesh.mesh_x.get_len())+0.1
						xyz.y=self.scale.project_m2screen_y(obj.y0)
						xyz.z=z						
						o.xyz.append(xyz)

						o.dxyz.x=0.1
						o.dxyz.y=obj.dy*self.scale.y_mul

						o.r=0.0
						o.g=0.0
						o.b=1.0

						o.type="plane"

						display_name=display_name+" ("+_("active")+")"
						self.gl_objects_add(o)

				if self.view_options.text==True:
					if self.views[0].zoom<40:
						if self.views[0].enabled==True:
							o=gl_base_object()
							o.r=1.0
							o.g=1.0
							o.b=1.0
							xyz=vec()
							xyz.x=self.scale.project_m2screen_x(gpvdm_data().mesh.mesh_x.get_len())+0.2
							xyz.y=self.scale.project_m2screen_y(obj.y0)
							xyz.z=z+(len(epi.layers)-l)*0.1
							o.xyz.append(xyz)
							o.id=["text"]
							o.type="text"
							o.text=display_name
							self.gl_objects_add(o)

				l=l+1


		def render(self):
			self.makeCurrent()
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
			glClearColor(self.view_options.bg_color[0], self.view_options.bg_color[1], self.view_options.bg_color[2], 0.5)
			glPolygonMode(GL_FRONT, GL_FILL);
			for v in self.views:
				if v.enabled==True:
					w=int(self.width()*v.window_w)
					h=int(self.height()*v.window_w)
					x=int(self.width()*v.window_x)
					y=int(self.height()*v.window_y)
					glViewport(x, y, w, h)
					v.projection = glGetDoublev(GL_PROJECTION_MATRIX)
					v.modelview = glGetDoublev(GL_MODELVIEW_MATRIX)
					v.viewport = glGetIntegerv(GL_VIEWPORT)
					self.render_view(v)

		def render_view(self,view):
			data=gpvdm_data()

			x=self.scale.project_m2screen_x(0)
			y=0.0#project_m2screen_y(0)
			z=self.scale.project_m2screen_z(0)

			self.dos_start=-1
			self.dos_stop=-1

			self.emission=False
			self.ray_model=data.ray.segments[0].config.ray_auto_run

			lines=[]
			epi=get_epi()
					

			glLoadIdentity()
			glScalef(1.0, -1.0, -1.0) 

			glTranslatef(view.x_pos, view.y_pos, view.zoom) # Move Into The Screen
			
			glRotatef(view.xRot, 1.0, 0.0, 0.0)
			glRotatef(view.yRot, 0.0, 1.0, 0.0)
			glRotatef(view.zRot, 0.0, 0.0, 1.0)

			glColor3f( 1.0, 1.5, 0.0 )


			lines=[]

			self.pos=0.0
			if self.view_options.render_cords==True:
				self.draw_cords()

			if self.enable_draw_ray_mesh==True:
				self.draw_ray_mesh()
			
			if self.view_options.optical_mode==True:
				self.draw_mode()

			if self.scene_built==False:
				self.build_scene()

			if self.plot_graph==True:
				self.draw_graph()


			if self.view_options.render_photons==True:
				self.draw_photons(x,z)
			if self.view_options.show_world_box==True:
				self.world_box()

			self.gl_objects_render()

			if view.zoom>self.view_options.stars_distance:
				self.draw_stars()


		def do_draw(self):
			#print("do_draw",self)
			self.makeCurrent()
			self.render()
			self.swapBuffers()

		def paintGL(self):
			#print("paintGL",self)
			self.makeCurrent()
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
			glLoadIdentity()
			glScalef(-1.0, 1.0, -1.0) 

			if self.failed==False:
				self.do_draw()



		def load_data(self):
			lines=[]

			data=gpvdm_data()
			self.dump_1d_slice_xpos=int(data.dump.dump_1d_slice_xpos)
			self.dump_1d_slice_zpos=int(data.dump.dump_1d_slice_zpos)

			self.dump_verbose_electrical_solver_results=str2bool(data.dump.dump_verbose_electrical_solver_results)
			try:
				self.suns=float(data.light.Psun)
			except:
				self.suns=0.0

			self.x_len=gpvdm_data().mesh.mesh_x.get_len()
			if os.path.isdir(os.path.join(os.path.join(get_sim_path(),"ray_trace")))==True:
				for v in self.views:
					v.render_photons=False


		#This will rebuild the scene from scratch
		def rebuild_scene(self):

			self.gl_objects_clear()
			self.menu_update()
			self.text_clear_lib()
			data=gpvdm_data()
			if data.triangles_loaded==False:
				data.load_triagles()

			if self.scale.world_min==None:
				self.scale.set_m2screen()

			x=self.scale.project_m2screen_x(0)
			z=self.scale.project_m2screen_z(0)

			if self.view_options.draw_rays==True:
				self.draw_rays(self.ray_file)

			if self.view_options.enable_draw_light_source==True:
				for source in data.light_sources.lights.segments:
					if source.light_illuminate_from=="xyz":
						point_x=float(source.x0)
						point_y=float(source.y0)
						point_z=float(source.z0)
						if point_x==-1.0:
							point_x=0.0
							point_y=0.0
							point_z=0.0
						else:
							point_x=self.scale.project_m2screen_x(point_x)
							point_y=self.scale.project_m2screen_y(point_y)
							point_z=self.scale.project_m2screen_z(point_z)
						#print(point_x,point_y)
						a=gl_base_object()
						a.id=[source.id]
						a.type="arrow"
						xyz=vec()
						xyz.x=point_x
						xyz.y=point_y
						xyz.z=point_z
						a.xyz.append(xyz)

						a.dxyz.x=0.5
						a.dxyz.y=0.5
						a.dxyz.z=0.5
						a.r=0.0
						a.g=1.0
						a.b=0.0
						
						a.rotate_x=source.rotate_x
						a.rotate_y=source.rotate_y

						a.moveable=True
						a.selectable=True
						self.gl_objects_add(a)

			if self.draw_electrical_mesh==True:
				self.draw_mesh()

			elif self.view_options.draw_device==True:
				self.draw_device2(x,z)
				self.draw_contacts()

			if self.enable_light_profile==True:
				self.draw_light_profile()

			if self.view_options.render_grid==True:
				self.gl_objects_add_grid(-18,20,self.scale.project_m2screen_y(self.scale.world_max.y),None,-18,20)

			for d in data.detectors.segments:
				if d.config.viewpoint_enabled==True:
					world_dx=(self.scale.world_max.x-self.scale.world_min.x)
					x0=self.scale.project_m2screen_x(self.scale.world_min.x)+world_dx*self.scale.x_mul*d.config.viewpoint_x0
					x1=x0+world_dx*self.scale.x_mul*d.config.viewpoint_dx

					y0=self.scale.project_m2screen_y(self.scale.world_min.y)+(self.scale.world_max.y-self.scale.world_min.y)*self.scale.y_mul*d.config.viewpoint_y0

					world_dz=(self.scale.world_max.z-self.scale.world_min.z)
					z0=self.scale.project_m2screen_z(self.scale.world_min.z)+world_dz*self.scale.z_mul*d.config.viewpoint_z0
					z1=z0+world_dz*self.scale.z_mul*d.config.viewpoint_dz

					dx=(x1-x0)/d.config.viewpoint_nx
					dz=(z1-z0)/d.config.viewpoint_nz

					self.gl_objects_add_grid(x0,x1,y0,None,z0,z1,color=[0.8,0.0,0.8,1.0],dx=dx,dz=dz)

					if self.view_options.text==True:
						if self.views[0].zoom<40:
							o=gl_base_object()
							o.r=1.0
							o.g=1.0
							o.b=1.0
							xyz=vec()
							xyz.x=x1+0.2
							xyz.y=y0
							xyz.z=z0
							o.xyz.append(xyz)
							o.id=["text"]
							o.type="text"
							o.text="Detector: "+d.english_name
							self.gl_objects_add(o)

			if data.sim.simmode.endswith("fdtd")==True:
				if self.view_options.render_fdtd_grid==True:
					for fdtd in data.fdtd.segments:
						if fdtd.fdtd_xzy=="zy":
							world_dy=(self.scale.world_max.y-self.scale.world_min.y)
							y0=self.scale.project_m2screen_y(self.scale.world_min.y)
							y1=y0+world_dy*self.scale.y_mul

							x0=0.0#self.scale.project_m2screen_y(self.scale.world_min.y)

							world_dz=(self.scale.world_max.z-self.scale.world_min.z)
							z0=self.scale.project_m2screen_z(self.scale.world_min.z)
							z1=z0+world_dz*self.scale.z_mul

							dy=(y1-y0)/float(fdtd.fdtd_ylen)
							dz=(z1-z0)/float(fdtd.fdtd_zlen)
							
							o=self.gl_objects_add_grid(x0,None,y0,y1,z0,z1,color=[0.8,0.0,0.8,1.0],dy=dy,dz=dz,direction="zy")
						elif fdtd.fdtd_xzy=="xy":
							world_dy=(self.scale.world_max.y-self.scale.world_min.y)
							y0=self.scale.project_m2screen_y(self.scale.world_min.y)
							y1=y0+world_dy*self.scale.y_mul

							world_dz=(self.scale.world_max.z-self.scale.world_min.z)
							z0=self.scale.project_m2screen_z(self.scale.world_min.z)+world_dz*self.scale.z_mul/2.0

							world_dx=(self.scale.world_max.x-self.scale.world_min.x)
							x0=self.scale.project_m2screen_x(self.scale.world_min.x)
							x1=x0+world_dx*self.scale.x_mul

							dy=(y1-y0)/float(fdtd.fdtd_ylen)
							dx=(x1-x0)/float(fdtd.fdtd_xlen)

							o=self.gl_objects_add_grid(x0,x1,y0,y1,z0,None,color=[0.8,0.0,0.8,1.0],dy=dy,dx=dx,direction="xy")
						elif fdtd.fdtd_xzy=="zx":
							world_dy=(self.scale.world_max.y-self.scale.world_min.y)
							y0=self.scale.project_m2screen_y(self.scale.world_min.y)+world_dy*self.scale.y_mul/2.0

							world_dz=(self.scale.world_max.z-self.scale.world_min.z)
							z0=self.scale.project_m2screen_z(self.scale.world_min.z)
							z1=z0+world_dz*self.scale.z_mul

							world_dx=(self.scale.world_max.x-self.scale.world_min.x)
							x0=self.scale.project_m2screen_x(self.scale.world_min.x)
							x1=x0+world_dx*self.scale.x_mul


							dx=(x1-x0)/float(fdtd.fdtd_xlen)
							dz=(z1-z0)/float(fdtd.fdtd_zlen)

							o=self.gl_objects_add_grid(x0,x1,y0,None,z0,z1,color=[0.8,0.0,0.8,1.0],dz=dz,dx=dx,direction="zx")
			if 1==0:
				for l in self.lights:
					a=gl_base_object()
					a.id=["rod"]
					a.type="box"
					xyz=vec()
					xyz.x=l.xyz[0]
					xyz.y=l.xyz[1]
					xyz.z=l.xyz[2]
					a.xyz.append(xyz)

					a.dxyz.x=0.4
					a.dxyz.y=0.4
					a.dxyz.z=0.4
					a.alpha=0.5
					a.r=1.0
					a.g=0.0
					a.b=0.0
					self.gl_objects_add(a)

			if self.pre_built_scene!=None:
				self.gl_objects_load(self.pre_built_scene)

			#print("rebuild")


		def build_scene(self):
			self.scene_built=True
			self.load_data()
			self.update()
			self.rebuild_scene()

		def force_redraw(self,level="rebuild"):
			if level=="reload_rebuild":
				data=gpvdm_data()
				data.load_triagles()
				self.build_scene()
				self.do_draw()
				self.menu_update()
			elif level=="rebuild":
				self.build_scene()
				self.do_draw()
				self.menu_update()
			elif level=="no_rebuild":
				self.update()
		def resizeEvent(self,event):
			if self.failed==False:
				#glClearDepth(1.0)              
				#glDepthFunc(GL_LESS)
				#glEnable(GL_DEPTH_TEST)
				#glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				#glEnable(GL_BLEND);
				#glShadeModel(GL_SMOOTH)
				glViewport(0, 0, self.width(), self.height()+100)
				glMatrixMode(GL_PROJECTION)
				glLoadIdentity()
				#glScalef(1.0, 1.0, -1.0)              
				gluPerspective(45.0,float(self.width()) / float(self.height()+100),0.1, 1000.0)
				glMatrixMode(GL_MODELVIEW)

		def initializeGL(self):
			self.load_data()
			#try:
			glClearDepth(5.0)              
			glDepthFunc(GL_LESS)
			glEnable(GL_DEPTH_TEST)
			glEnable(GL_BLEND)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	#GL_ONE
			
			glEnableClientState(GL_VERTEX_ARRAY)
			glShadeModel(GL_SMOOTH)
			glColorMaterial(GL_FRONT, GL_DIFFUSE)		#This means we can set the color of a material using glColor and not glMaterialfv
			glEnable(GL_COLOR_MATERIAL)							#This means we can set the color of a material using glColor and not glMaterialfv


				#lightZeroPosition = [0, 0, -10, 1.0]
				#lightZeroColor = [1.0, 1.0, 1.0, 1.0]
				#glLightfv(GL_LIGHT1, GL_POSITION, lightZeroPosition)
				#glLightfv(GL_LIGHT1,  GL_DIFFUSE, lightZeroColor)
				#glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1)
				#glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05)
				#glEnable(GL_LIGHT1)

				#lightZeroPosition = [10, 10, 0, 1.0]
				#lightZeroColor = [1.0, 1.0, 1.0, 1.0]
				#glLightfv(GL_LIGHT2, GL_POSITION, lightZeroPosition)
				#glLightfv(GL_LIGHT2,  GL_DIFFUSE, lightZeroColor)
				#glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.1)
				#glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05)
				#glEnable(GL_LIGHT2)

#GL_DIFFUSE

			#glEnable(GL_FOG);
			#fogColor = [0.5, 0.5, 0.5, 1.0];

			#glFogi (GL_FOG_MODE, GL_EXP);
			#glFogfv (GL_FOG_COLOR, fogColor);
			#glFogf (GL_FOG_DENSITY, 0.35);
			#glHint (GL_FOG_HINT, GL_DONT_CARE);
			#glFogf (GL_FOG_START, 1.0);
			#glFogf (GL_FOG_END, 5.0);
			#self.tex = self.read_texture('/home/rod/images/image.jpg')
			glViewport(0, 0, self.width(), self.height()+100)
			glMatrixMode(GL_PROJECTION)
			glLoadIdentity()
			#glScalef(1.0, 1.0, -1.0)                  
			gluPerspective(45.0,float(self.width()) / float(self.height()+100),0.001, 1000.0) 
			glMatrixMode(GL_MODELVIEW)
			glEnable( GL_POLYGON_SMOOTH )
			#glEnable(GL_MULTISAMPLE)
			#self.resizeEvent.connect(self.resize)
		
			if self.lit==True:
				for l in self.lights:
					glEnable(GL_LIGHTING)
					lightZeroColor = [1.0, 1.0, 1.0, 1.0]
					#print(l.number,GL_LIGHT1)
					glLightfv(l.number, GL_POSITION, [l.xyz[0],l.xyz[1],l.xyz[2] ,1.0])
					glLightfv(l.number, GL_AMBIENT, [0.2,0.2,0.2,1.0 ])
					glLightfv(l.number, GL_DIFFUSE, [0.8,0.8,0.8,1.0 ])
					glLightfv(l.number, GL_SPECULAR, [1.0,1.0,1.0,1.0 ])
					#glLightfv(l .number, GL_SPOT_DIRECTION, [ 1,1,1]);
					#glLightf(l.number, GL_CONSTANT_ATTENUATION, 0.1)
					#glLightf(l.number, GL_LINEAR_ATTENUATION, 0.05)
					glEnable(l.number)

			self.failed=False

			get_epi().add_callback(self.force_redraw)

		def boom(self):
			print("oh")
else:
	class glWidget(gl_fallback, gl_toolbar,gl_views):

		def __init__(self, parent):
			QWidget.__init__(self)
			gl_fallback.__init__(self)
			gl_toolbar.__init__(self)
			self.views=[]
			self.scale=gl_scale()
			self.failed=True
			self.open_gl_working=False

		def rebuild_scene(self):
			pass

		def do_draw(self):
			pass
