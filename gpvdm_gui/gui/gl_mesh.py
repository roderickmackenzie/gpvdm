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

## @package gl_mesh
#  The gl_mesh class for the OpenGL display.
#

import sys
from math import fabs

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb
	from PyQt5.QtWidgets import QMenu

except:
	pass

from PyQt5.QtCore import QTimer
from inp import inp

from epitaxy import get_epi
from mesh import get_mesh
from gl_base_object import gl_base_object
from gpvdm_json import gpvdm_data
class gl_mesh():
	def draw_mesh(self):
		x=[]
		y=[]
		z=[]
		data=gpvdm_data()
		epi=get_epi()
		device_start=epi.get_device_start(data)
		mesh=get_mesh()

		y,temp=mesh.y.calculate_points()
		x,temp=mesh.x.calculate_points()
		z,temp=mesh.z.calculate_points()

		x=self.scale.project_m2screen_x(x)
		y=self.scale.project_m2screen_y(y)
		z=self.scale.project_m2screen_z(z)

		glLineWidth(3)
		if mesh.y.circuit_model==False:
			self.drift_diffusion_mesh()

	def drift_diffusion_mesh(self):
		x=[]
		y=[]
		z=[]
		data=gpvdm_data()
		epi=get_epi()
		device_start=epi.get_device_start(data)
		mesh=get_mesh()

		y,temp=mesh.y.calculate_points()
		x,temp=mesh.x.calculate_points()
		z,temp=mesh.z.calculate_points()

		for i in range(0,len(y)):
			y[i]=y[i]+device_start

		x=self.scale.project_m2screen_x(x)
		y=self.scale.project_m2screen_y(y)
		z=self.scale.project_m2screen_z(z)


		glLineWidth(3)

		for zi in range(0,len(z)):
			for xi in range(0,len(x)):
				for yi in range(0,len(y)):
					name="mesh:"+str(xi)+":"+str(yi)+":"+str(zi)
					if xi==self.dump_1d_slice_xpos and zi==self.dump_1d_slice_zpos:
						a=gl_base_object()
						a.id=[name]
						a.type="ball"
						a.x=x[xi]
						a.y=y[yi]
						a.z=z[zi]
						a.dx=0.05
						a.r=0.0
						a.g=0.0
						a.b=1.0
						self.gl_objects_add(a)
					else:
						a=gl_base_object()
						a.id=[name]
						a.type="ball"
						a.x=x[xi]
						a.y=y[yi]
						a.z=z[zi]
						a.dx=0.05
						if self.dump_verbose_electrical_solver_results==False:
							a.alpha=0.5
						else:
							a.alpha=0.9
						a.r=1.0
						a.g=0.0
						a.b=0.0
						self.gl_objects_add(a)

					if yi!=len(y)-1:
						a=gl_base_object()
						a.id=["electrical_mesh"]
						a.type="line"
						a.x=x[xi]
						a.y=y[yi]
						a.z=z[zi]
						a.dx=0.0
						a.dy=y[yi+1]-y[yi]
						a.dz=0.0
						a.r=1.0
						a.g=0.0
						a.b=0.0
						self.gl_objects_add(a)


					if xi!=len(x)-1:
						a=gl_base_object()
						a.id=["electrical_mesh"]
						a.type="line"
						a.x=x[xi]
						a.y=y[yi]
						a.z=z[zi]
						a.dx=x[xi+1]-x[xi]
						a.dy=0.0
						a.dz=0.0
						a.r=1.0
						a.g=0.0
						a.b=0.0
						self.gl_objects_add(a)

					if zi!=len(z)-1:
						a=gl_base_object()
						a.id=["electrical_mesh"]
						a.type="line"
						a.x=x[xi]
						a.y=y[yi]
						a.z=z[zi]
						a.dx=0.0
						a.dy=0.0
						a.dz=z[zi+1]-z[zi]
						a.r=1.0
						a.g=0.0
						a.b=0.0
						self.gl_objects_add(a)


	def mesh_menu(self,event):
		view_menu = QMenu(self)
		

		menu = QMenu(self)

		view=menu.addMenu(_("Dump"))


		if self.dump_1d_slice_xpos==-1:
			action=view.addAction(_("Dump 1D slices"))
		else:
			action=view.addAction(_("Don't dump 1D slice"))

		action.triggered.connect(self.menu_1d_slice_dump)

		if self.dump_verbose_electrical_solver_results==False:
			action=view.addAction(_("Set verbose electrical solver dumping"))
		else:
			action=view.addAction(_("Unset verbose electrical solver dumping"))

		action.triggered.connect(self.menu_dump_verbose_electrical_solver_results)



		menu.exec_(event.globalPos())


	def menu_1d_slice_dump(self):
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			s=obj.id_starts_with("mesh").split(":")

			x=int(s[1])
			y=int(s[2])
			z=int(s[3])

			if self.dump_1d_slice_xpos==x and self.dump_1d_slice_zpos==z:
				self.dump_1d_slice_xpos=-1
				self.dump_1d_slice_zpos=-1
			else:
				self.dump_1d_slice_xpos=x
				self.dump_1d_slice_zpos=z

			f=inp()
			f.load("dump.inp")
			f.replace("#dump_1d_slice_xpos",str(self.dump_1d_slice_xpos))
			f.replace("#dump_1d_slice_zpos",str(self.dump_1d_slice_zpos))
			f.save()

			self.gl_objects_remove_regex("mesh")
			self.draw_mesh()

			self.do_draw()

	def menu_dump_verbose_electrical_solver_results(self):
		self.dump_verbose_electrical_solver_results = not self.dump_verbose_electrical_solver_results
		f=inp()
		f.load("dump.inp")
		f.replace("#dump_verbose_electrical_solver_results",str(self.dump_verbose_electrical_solver_results))
		f.save()

		self.gl_objects_remove_regex("mesh")
		self.draw_mesh()

		self.do_draw()

	def project_object_through_electrical_mesh(self,o):
		mesh=get_mesh()
		mesh_with_gaps=False
		for l in mesh.x.layers:
			if l.points==0:
				mesh_with_gaps=True
				break 

		if mesh_with_gaps==False:
			self.gl_objects_add(o)
		else:
			for l in mesh.x.layers:
				if l.points!=0:
					new_obj=gl_base_object()
					new_obj.copy(o)
					#print(layer,l.start,l.end-l.start)
					new_obj.xyz.x=self.scale.project_m2screen_x(l.start)
					new_obj.dxyz.x=(l.end-l.start)*self.scale.x_mul
					#print(layer,o.xyz.x,o.dxyz.x)
					self.gl_objects_add(new_obj)

