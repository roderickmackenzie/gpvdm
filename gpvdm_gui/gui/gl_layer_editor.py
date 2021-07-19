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
import os
#from OpenGL.GL import *
#from OpenGL.GLU import *
#from PyQt5 import QtOpenGL
from PyQt5.QtOpenGL import QGLWidget
from PyQt5.QtWidgets import QMenu
from object_editor import object_editor
from epitaxy import get_epi
from epitaxy_class import epi_layer
from gui_util import dlg_get_text
from icon_lib import icon_get
from gui_util import yes_no_dlg
from shape import shape
from contacts_io import contact
from gpvdm_json import gpvdm_data

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

		action=menu.addAction(icon_get("list-add"),_("Add layer"))
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
		data=gpvdm_data()
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			name=obj.id_starts_with("layer:")
			if name!=False:
				name=name[len("layer:"):]

				epi=get_epi()
				epi.move_up(name)
				data.save()
				#print("layer_move_up",self.selected_layer[len("layer:"):])
				self.force_redraw() 

	def layer_add(self):
		data=gpvdm_data()
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			epi=get_epi()
			s=epi.find_object_by_id(obj.id[0])
			if type(s)==epi_layer or type(s)==shape:
				layer_index=epi.find_layer_by_id(obj.id[0])

				a=epi.add_new_layer(pos=layer_index)
				self.force_redraw()
				data.save()
		
	def layer_move_down(self):
		data=gpvdm_data()
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			name=obj.id_starts_with("layer:")
			if name!=False:
				name=name[len("layer:"):]

				epi=get_epi()
				epi.move_down(name)
				data.save()
				#print("layer_move_down",self.selected_layer[len("layer:"):])
				self.force_redraw() 

	def layer_delete(self):
		data=gpvdm_data()
		epi=get_epi()
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			s=epi.find_object_by_id(obj.id[0])
			if s!=None:
				response=yes_no_dlg(self,"Do you really want to delete the object: "+s.shape_name)
				if response == True:
					epi=get_epi()
					epi.remove_by_id(s.id)
					data.save()
					self.force_redraw() 

	def layer_rename(self):
		data=gpvdm_data()
		epi=get_epi()
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			s=epi.find_object_by_id(obj.id[0])
			if s!=None:
				old_name=s.shape_name
				name=dlg_get_text( _("Rename the layer:"), old_name,"rename.png")
				name=name.ret

				if name!=None:
					s.shape_name=name
					data.save()

			self.force_redraw() 

	def layer_object_editor(self):
		obj=self.gl_objects_get_first_selected()
		if obj!=None:
			epi=get_epi()
			s=epi.find_object_by_id(obj.id[0])
			if type(s)==shape or type(s)==contact:
				self.shape_edit=object_editor()
				self.shape_edit.load(obj.id[0])
				self.shape_edit.show()
			elif type(s)==epi_layer:
				ids=[]
				sub_shapes=epi.get_all_sub_shapes(obj.id[0])

				for sub in sub_shapes:
					ids.append(sub.id)

				self.shape_edit=object_editor()
				self.shape_edit.load(ids)
				self.shape_edit.show()





