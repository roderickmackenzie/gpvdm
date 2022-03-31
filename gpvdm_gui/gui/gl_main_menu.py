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

## @package gl_main_menu
#  The gl_main_menu class for the OpenGL display.
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

import io
from PyQt5.QtCore import QTimer, Qt
from cal_path import get_sim_path
from PyQt5.QtWidgets import QDialog, QFontDialog, QColorDialog
from open_save_dlg import save_as_filter
from PyQt5.QtWidgets import QApplication
from PyQt5.QtGui import QImage
from gpvdm_json import gpvdm_data
from icon_lib import icon_get
import os

class gl_main_menu():
	def build_main_menu(self):
		view_menu = QMenu(self)
		

		self.main_menu = QMenu(self)

		export=self.main_menu.addMenu(_("Export"))

		action=export.addAction(_("Save image"))
		action.triggered.connect(self.save_image_as)

		action=export.addAction(_("Clipboard"))
		action.triggered.connect(self.callback_copy)


		view=self.main_menu.addMenu(_("View"))

		self.menu_view_draw_electrical_mesh=view.addAction(_("Electrical mesh"))
		self.menu_view_draw_electrical_mesh.triggered.connect(self.menu_toggle_view)
		self.menu_view_draw_electrical_mesh.setCheckable(True)


		self.menu_view_render_photons=view.addAction(_("Show photons"))
		self.menu_view_render_photons.triggered.connect(self.menu_toggle_view)
		self.menu_view_render_photons.setCheckable(True)

		self.menu_view_draw_device=view.addAction(_("Show device"))
		self.menu_view_draw_device.triggered.connect(self.menu_toggle_view)
		self.menu_view_draw_device.setCheckable(True)

		self.menu_view_grid=view.addAction(_("Grid"))
		self.menu_view_grid.triggered.connect(self.menu_toggle_view)
		self.menu_view_grid.setCheckable(True)

		self.menu_view_fdtd_grid=view.addAction(_("FDTD grid"))
		self.menu_view_fdtd_grid.triggered.connect(self.menu_toggle_view)
		self.menu_view_fdtd_grid.setCheckable(True)

		self.menu_view_cords=view.addAction(_("Coordinates"))
		self.menu_view_cords.triggered.connect(self.menu_toggle_view)
		self.menu_view_cords.setCheckable(True)

		self.menu_view_optical_mode=view.addAction(_("Optical mode"))
		self.menu_view_optical_mode.triggered.connect(self.menu_toggle_view)
		self.menu_view_optical_mode.setCheckable(True)

		self.menu_view_text=view.addAction(_("Show text"))
		self.menu_view_text.triggered.connect(self.menu_toggle_view)
		self.menu_view_text.setCheckable(True)

		self.menu_view_plot=view.addAction(_("Show plot"))
		self.menu_view_plot.triggered.connect(self.menu_toggle_view)
		self.menu_view_plot.setCheckable(True)

		self.menu_view_transparent_objects=view.addAction(_("Transparent objects"))
		self.menu_view_transparent_objects.triggered.connect(self.menu_toggle_view)
		self.menu_view_transparent_objects.setCheckable(True)

		action=view.addAction(_("Ray tracing mesh"))
		action.triggered.connect(self.menu_toggle_view)

		self.menu_view_light_source=view.addAction(_("Light source"))
		self.menu_view_light_source.triggered.connect(self.menu_toggle_view)
		self.menu_view_light_source.setCheckable(True)

		self.menu_view_draw_rays=view.addAction(_("Rays"))
		self.menu_view_draw_rays.triggered.connect(self.menu_toggle_view)
		self.menu_view_draw_rays.setCheckable(True)

		self.menu_show_world_box=view.addAction(_("Show world box"))
		self.menu_show_world_box.triggered.connect(self.menu_toggle_view)
		self.menu_show_world_box.setCheckable(True)

		action=view.addAction(_("Stars"))
		action.triggered.connect(self.menu_stars)



		edit=self.main_menu.addMenu(_("Edit"))

		action=edit.addAction(_("Font"))
		action.triggered.connect(self.menu_toggle_view)

		action=edit.addAction(_("Backgroud color"))
		action.triggered.connect(self.menu_background_color)

		objects=self.main_menu.addMenu(_("Objects"))
		action=objects.addAction(icon_get("list-add"),_("Add"))
		action.triggered.connect(self.callback_add_object)

		action=objects.addAction(icon_get("view-refresh"),_("Rescale"))
		action.triggered.connect(self.callback_rescale)

		action=objects.addAction(icon_get("view-refresh"),_("Debug"))
		action.triggered.connect(self.callback_debug)

	def callback_debug(self):
		self.load_from_json(os.path.join(get_sim_path(),"electrical_mesh.dat"),dz=-0.012)
		from gl_base_object import gl_base_object
		from triangle import vec

		o=gl_base_object()
		xyz=vec()
		xyz.x=-6.0
		xyz.y=self.scale.project_m2screen_y(0.0)
		xyz.z=3			
		o.xyz.append(xyz)

		o.dxyz.x=5.0
		o.dxyz.y=2.0
		o.dxyz.z=2.0

		o.r=0.0
		o.g=0.0
		o.b=1.0

		o.type="image"

		o.image_path="text.png"


		####
		o2=gl_base_object()
		xyz1=vec()
		xyz1.x=-6.0		#self.scale.project_m2screen_x(-0.012)
		xyz1.y=0.0
		xyz1.z=-13.0		#self.scale.project_m2screen_z(0.5)					
		o2.xyz.append(xyz1)

		o2.dxyz.x=5.0
		o2.dxyz.y=2.0
		o2.dxyz.z=2.0

		o2.r=0.0
		o2.g=0.0
		o2.b=1.0

		o2.type="image"

		o2.image_path="r.png"
		self.force_redraw()
		self.gl_objects_add(o)
		self.gl_objects_add(o2)
		self.do_draw()

	def callback_rescale(self):
		self.scale.set_m2screen()
		self.build_scene()

	def callback_add_object(self):
		from shape import shape
		a=shape()
		a.dy=0.5
		a.dx=0.5
		a.dz=0.5

		a.shape_name="object"
		a.shapes=[]
		a.color_r=1.0
		a.color_g=0
		a.color_b=0
		a.color_alpha=0.5
		a.moveable=True
		a.load_triangles()
		gpvdm_data().world.world_data.segments.append(a)
		self.force_redraw()

	def menu(self,event):
		self.main_menu.exec_(event.globalPos())

	def callback_copy(self):
		self.render()
		QApplication.clipboard().setImage(self.grabFrameBuffer())

	def save_image_as(self):
		#self.random_device()
		#return
		ret=save_as_filter(self,"png (*.png);;3D object file (*.gobj)")
		if ret!=False:
			if ret.endswith("png"):
				self.grabFrameBuffer().save(ret)

	def menu_background_color(self):
		col = QColorDialog.getColor(Qt.white, self)
		if col.isValid():
			self.view_options.bg_color=[col.red()/255,col.green()/255,col.blue()/255]
			self.force_redraw()

	def menu_toggle_view(self):
		action = self.sender()
		text=action.text()
		self.draw_electrical_mesh=self.menu_view_draw_electrical_mesh.isChecked()
		self.view_options.render_photons=self.menu_view_render_photons.isChecked()
		self.view_options.render_grid=self.menu_view_grid.isChecked()
		self.view_options.render_fdtd_grid=self.menu_view_fdtd_grid.isChecked()
		self.view_options.render_cords=self.menu_view_cords.isChecked()
		self.view_options.draw_device=self.menu_view_draw_device.isChecked()
		self.view_options.optical_mode=self.menu_view_optical_mode.isChecked()
		self.view_options.text=self.menu_view_text.isChecked()
		self.view_options.render_plot=self.menu_view_plot.isChecked()
		self.view_options.transparent_objects=self.menu_view_transparent_objects.isChecked()
		self.view_options.enable_draw_light_source=self.menu_view_light_source.isChecked()
		self.view_options.draw_rays=self.menu_view_draw_rays.isChecked()
		self.view_options.show_world_box=self.menu_show_world_box.isChecked()

		if text==_("Ray tracing mesh"):
			self.view_options.draw_rays= not self.view_options.draw_rays
			if self.enable_draw_ray_mesh==False:
				self.gl_objects_remove_regex("ray_mesh")
		if text==_("Device view"):
			self.enable_draw_device = not self.enable_draw_device
		if text==_("Font"):
			diag=QFontDialog()
			font, ok = QFontDialog.getFont(self.font)
			if ok:
				self.font = font

		self.force_redraw()


	def menu_stars(self):
		if self.view_options.stars_distance==60:
			self.view_options.stars_distance=0.0
		else:
			self.view_options.stars_distance=60

		self.force_redraw()

	def menu_update(self):
		self.menu_view_draw_electrical_mesh.setChecked(self.draw_electrical_mesh)
		self.menu_view_render_photons.setChecked(self.view_options.render_photons)
		self.menu_view_grid.setChecked(self.view_options.render_grid)
		self.menu_view_fdtd_grid.setChecked(self.view_options.render_fdtd_grid)
		self.menu_view_cords.setChecked(self.view_options.render_cords)
		self.menu_view_draw_device.setChecked(self.view_options.draw_device)
		self.menu_view_optical_mode.setChecked(self.view_options.optical_mode)
		self.menu_view_text.setChecked(self.view_options.text)
		self.menu_view_plot.setChecked(self.view_options.render_plot)
		self.menu_view_transparent_objects.setChecked(self.view_options.transparent_objects)
		self.menu_view_light_source.setChecked(self.view_options.enable_draw_light_source)
		self.menu_view_draw_rays.setChecked(self.view_options.draw_rays)
		self.menu_show_world_box.setChecked(self.view_options.show_world_box)

