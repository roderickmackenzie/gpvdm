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

## @package gl_layer_editor
#  An OpenGL layer editor.
#

import sys
from math import fabs

from OpenGL.GL import *
from OpenGL.GLU import *
from PyQt5 import QtOpenGL
from PyQt5.QtOpenGL import QGLWidget
from PyQt5.QtWidgets import QMenu
from gl_lib import val_to_rgb
from object_editor import object_editor


from PyQt5.QtCore import QTimer

import os
from epitaxy import get_epi
from epitaxy import epi_layer
from gui_util import dlg_get_text
from icon_lib import icon_get
from gui_util import yes_no_dlg
from epitaxy import epi_layer
from cal_path import get_default_material_path
from mesh import get_mesh
from cal_path import get_sim_path
from shape import shape
from inp import inp_copy_file
from contacts_io import contact

class gl_layer_editor():

	def menu_layer(self,event):
		view_menu = QMenu(self)
		

		menu = QMenu(self)

		action=menu.addAction(icon_get("go-up"),_("Move up"))
		action.triggered.connect(self.layer_move_up)

		action=menu.addAction(_("Move down"))
		action.triggered.connect(self.layer_move_down)

		action=menu.addAction(icon_get("list-remove"),_("Delete"))
		action.triggered.connect(self.layer_delete)

		action=menu.addAction(icon_get("list-add"),_("Add shape"))
		action.triggered.connect(self.layer_add)

		action=menu.addAction(icon_get("rename"),_("Rename"))
		action.triggered.connect(self.layer_rename)

		action=menu.addAction(_("Edit"))
		action.triggered.connect(self.layer_object_editor)

		menu.addSeparator()

		#action=menu.addAction(_("Set layer active"))
		#action=menu.addAction(_("Thickness"))

		#action=menu.addAction(_("Optical properties"))
		#action=menu.addAction(_("Electrical properties"))


		#action.triggered.connect(self.layer_move_down)

		#menu.addSeparator()
		menu.exec_(event.globalPos())

	def layer_move_up(self):
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			name=obj.id_starts_with("layer:")
			if name!=False:
				name=name[len("layer:"):]

				epi=get_epi()
				epi.move_up(name)
				epi.save()
				#print("layer_move_up",self.selected_layer[len("layer:"):])
				self.force_redraw() 

	def layer_add(self):
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			epi=get_epi()
			s=epi.find_shape_by_id(obj.id[0])
			if type(s)==epi_layer or type(s)==shape:
				layer_index=epi.find_layer_by_id(obj.id[0])
				new_filename=epi.new_electrical_file("shape")+".inp"
				orig_filename=os.path.join(get_default_material_path(),"shape.inp")
				inp_copy_file(os.path.join(get_sim_path(),new_filename),os.path.join(get_sim_path(),orig_filename))

				mesh=get_mesh()
				my_shape=shape()
				my_shape.load(new_filename)
				my_shape.dy=epi.layers[layer_index].dy
				my_shape.dx=mesh.get_xlen()
				my_shape.dz=mesh.get_zlen()
				my_shape.shape_electrical=epi.gen_new_electrical_file("electrical")
				my_shape.shape_nx=1
				my_shape.shape_ny=1
				my_shape.shape_nz=1
				my_shape.name="New shape"
				my_shape.save()

				epi.layers[layer_index].shapes.append(my_shape)
				epi.save()
				self.force_redraw()

		
	def layer_move_down(self):
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			name=obj.id_starts_with("layer:")
			if name!=False:
				name=name[len("layer:"):]

				epi=get_epi()
				epi.move_down(name)
				epi.save()
				#print("layer_move_down",self.selected_layer[len("layer:"):])
				self.force_redraw() 

	def layer_delete(self):
		epi=get_epi()
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			s=epi.find_shape_by_id(obj.id[0])
			if s!=None:
				response=yes_no_dlg(self,"Do you really want to delete the object: "+s.name)
				if response == True:
					epi=get_epi()
					epi.remove_by_id(s.id)
					epi.save()
					self.force_redraw() 

	def layer_rename(self):
		epi=get_epi()
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			s=epi.find_shape_by_id(obj.id[0])
			if s!=None:
				old_name=s.name
				name=dlg_get_text( _("Rename the layer:"), old_name,"rename.png")
				name=name.ret

				if name!=None:
					s.name=name
					epi.save()

			self.force_redraw() 

	def layer_object_editor(self):
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			epi=get_epi()
			s=epi.find_shape_by_id(obj.id[0])
			if type(s)==shape or type(s)==contact:
				self.shape_edit=object_editor()
				self.shape_edit.load(obj.id[0])
				self.shape_edit.show()
			elif type(s)==epi_layer:
				ids=[]
				sub_shapes=epi.get_all_sub_shapes(obj.id[0])

				#ids.append(obj.id[0])
				for sub in sub_shapes:
					ids.append(sub.id)

				#print(ids)
				self.shape_edit=object_editor()
				self.shape_edit.load(ids)
				self.shape_edit.show()

				#epi.save()

				#self.do_draw()



