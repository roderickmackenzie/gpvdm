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

## @package json_viewer
#  This is the equivlent of the inp_viewer for json files
#


import os

from token_lib import tokens
from undo import undo_list_class
from tab_base import tab_base
from str2bool import str2bool
from util import latex_to_html
from gtkswitch import gtkswitch
from leftright import leftright
from gpvdm_select import gpvdm_select
from gpvdm_select_material import gpvdm_select_material
from gpvdm_select_emission import gpvdm_select_emission
from gpvdm_select_filter import gpvdm_select_filter
from gpvdm_select_shape import gpvdm_select_shape
from gpvdm_select_filter import gpvdm_select_filter
from icon_widget import icon_widget



from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import  QApplication, QMenu
from PyQt5.QtWidgets import QTextEdit,QWidget, QScrollArea,QVBoxLayout,QLabel,QHBoxLayout,QPushButton, QSizePolicy, QTableWidget, QTableWidgetItem,QComboBox,QGridLayout,QLineEdit
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QPixmap, QIcon

from QComboBoxLang import QComboBoxLang
from QColorPicker import QColorPicker
from QColorPicker import QColorPicker_one_line
from QComboBoxNewtonSelect import QComboBoxNewtonSelect
from QComboBoxFits import QComboBoxFits

from PyQt5.QtCore import QTimer

import i18n
_ = i18n.language.gettext

import functools
from error_dlg import error_dlg
from QParasitic import QParasitic
from generic_switch import generic_switch
from mobility_widget import mobility_widget
from shape_dos_switch import shape_dos_switch
from shape_electrical_switch import shape_electrical_switch

from QChangeLog import QChangeLog
from QLabel_click import QLabel_click
from gui_util import widget_get_value
from gui_util import widget_set_value

from bibtex import bibtex
from gpvdm_json import gpvdm_data
from QComboBoxLayers import QComboBoxLayers
from icon_lib import icon_get
import json
from inp import inp
from scan_human_labels import json_get_val

class tab_line():
	def __init__(self):
		self.token=""
		self.label=None
		self.edit_box=None
		self.units=None
		self.widget=""
		self.hide_on_true_token="none"
		self.hide_on_false_token=[]
		self.hide_on_token_eq=None
		self.show_on_token_eq=None

class json_viewer(QWidget,tab_base):

	changed = pyqtSignal(str)

	#jason path is a string
	def __init__(self,db_json_file=None):
		QWidget.__init__(self)

		self.editable=True
		self.widget_list=[]
		self.file_name=None
		self.tab=QGridLayout()

		self.setLayout(self.tab)

		self.json_default_values=None
		self.db_json_file=db_json_file
		self.json_path=None
		#print(self.db_json_file)

		self.menu_build()

	def mousePressEvent(self, event):
		if event.button() == Qt.RightButton:
			self.main_menu.exec_(event.globalPos())

	def get_easy_value(self,m):
		val=getattr(self.template_widget, m)
		if type(val)==float:
			if val==0.0:
				return "0.0"
			if val<100 and val>0.01:
				return str(val)
			else:
				number="{:.4e}".format(val).replace("+","")
				return number
		elif type(val)==int:
			return str(val)

		return val

	def menu_build(self):
		self.main_menu = QMenu(self)

		self.copy_menu=self.main_menu.addMenu(_("Copy"))

		action=self.copy_menu.addAction(icon_get("edit-copy"),_("Copy as JSON"))
		action.triggered.connect(self.do_copy)

		action=self.copy_menu.addAction(icon_get("edit-copy"),_("Copy json/python path"))
		action.triggered.connect(self.do_copy_path)

		action=self.copy_menu.addAction(icon_get("edit-copy"),_("Copy ObjID"))
		action.triggered.connect(self.do_copy_id)

		action=self.copy_menu.addAction(icon_get("edit-copy"),_("Copy as LaTex"))
		action.triggered.connect(self.do_copy_latex)

		action=self.main_menu.addAction(icon_get("edit-paste"),_("Paste"))
		action.triggered.connect(self.do_paste)

		self.export_menu=self.main_menu.addMenu(_("Export"))
		action=self.export_menu.addAction(icon_get("export_csv"),_("Copy as csv"))
		action.triggered.connect(self.do_copy_as_csv)

		if self.db_json_file!=None:
			action=self.main_menu.addAction(icon_get("edit-undo"),_("Reimport from database"))
			action.triggered.connect(self.callback_reimport)

	def do_copy(self):
		lines=self.template_widget.gen_json()
		lines[0]="\"data\": "+lines[0].split(":")[1]
		all_data=[]
		all_data.append("{")
		all_data.append("\"data_type\": \"\",")

		all_data.extend(lines)
		all_data.append("}")

		cb = QApplication.clipboard()
		cb.clear(mode=cb.Clipboard )
		cb.setText("\n".join(all_data), mode=cb.Clipboard)

	def do_copy_path(self):
		if self.uid!=None:
			path=eval(self.json_path)
			obj,path=path.find_object_path_by_id(self.uid)
			cb = QApplication.clipboard()
			cb.clear(mode=cb.Clipboard )
			cb.setText(self.json_path+path, mode=cb.Clipboard)

	def do_copy_latex(self):
		from util_latex import latex
		latex_data=self.template_widget.dump_as_latex(token_lib=tokens())

		lx=latex()
		lx.document_start()
		lx.tab_start(["Parameter","Value","Units"])

		for latex_line in latex_data:
			lx.tab_add_row([latex_line.text,"$"+latex_line.value+"$","$"+latex_line.units+"$"])

		lx.tab_end()
		lx.latex_document_end()

		cb = QApplication.clipboard()
		cb.clear(mode=cb.Clipboard )
		cb.setText("".join(lx.lines), mode=cb.Clipboard)

	def do_copy_id(self):
		uid=self.template_widget.id

		cb = QApplication.clipboard()
		cb.clear(mode=cb.Clipboard )
		cb.setText(uid, mode=cb.Clipboard)

	def do_copy_as_csv(self):
		all_data=[]
		my_token_lib=tokens()
		for item in self.template_widget.var_list:
			token=item[0]
			result=my_token_lib.find_json(token)
			if result!=False:
				units=result.units
				text_info=result.info
				val=getattr(self.template_widget,token)
				line=text_info+","+str(val)+","+units
				all_data.append(line)
		cb = QApplication.clipboard()
		cb.clear(mode=cb.Clipboard )
		cb.setText("\n".join(all_data), mode=cb.Clipboard)

	def do_paste(self):
		lines = QApplication.clipboard().text()
		read_data=json.loads(lines)
		data=read_data['data']
		from json_base import del_keys
		data=del_keys(data, "id")
		#print(data)
		self.template_widget.load_from_json(data)

		self.update_values()
		data=gpvdm_data()
		data.save()

	def callback_reimport(self):
		if self.db_json_file!=None:
			try:
				self.f=inp()
				self.f.load_json(self.db_json_file)
				self.json_default_values=json_get_val(self.f.json,self.json_path)
			except:
				return
			for item in self.template_widget.var_list:
				print(item,self.json_default_values)
				if item[0] in self.json_default_values:
					val_from_db=self.json_default_values[item[0]]
					if type(val_from_db)==float or type(val_from_db)==int:
						setattr(self.template_widget, item[0], float(val_from_db))
			self.update_values()
			data=gpvdm_data()
			data.save()

	def refind_template_widget(self):
		if self.uid!=None:
			path=eval(self.json_path)
			self.template_widget=path.find_object_by_id(self.uid)

	def populate(self,template_widget,uid=None,json_path=None):
		self.uid=uid
		self.json_path=json_path

		self.template_widget=template_widget
		self.refind_template_widget()
		n=0
		pos=0
		my_token_lib=tokens()
		widget_number=0
		bib=bibtex()
		if self.file_name!=None:
			bib.load(os.path.splitext(self.file_name)[0]+".bib")
		#members = [attr for attr in dir(self.template_widget) if not callable(getattr(self.template_widget, attr)) and not attr.startswith("__")]

		for item in self.template_widget.var_list:
			a=tab_line()
			token=item[0]
			draw_widget=True

			show=False
			units="Units"

			result=my_token_lib.find_json(token)
			if result!=False:
				units=result.units
				text_info=result.info
				show=True
				if result.hidden==True:
					show=False
			
			if show == True :
				description=QLabel_click()
				description.setText(latex_to_html(text_info))
				if self.file_name!=None:
					if bib.get_ref(token)!=False:
						description.setStyleSheet('color: green')
					description.clicked.connect(functools.partial(self.callback_ref,token,description))

				#units widget
				if result.units_widget=="QLabel":
					unit=QLabel()
					unit.setText(latex_to_html(units))
				elif result.units_widget=="QPushButton":
					unit=QPushButton()
					unit.setText(latex_to_html(units))
					unit.setMinimumSize(50, 25)

				#edit widget
				if result.widget=="gtkswitch":
					a.edit_box=gtkswitch()
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="leftright":
					a.edit_box=leftright()
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="gpvdm_select":
					a.edit_box=gpvdm_select(file_box=True)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.edit.textChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="gpvdm_select_material":
					a.edit_box=gpvdm_select_material(file_box=False)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="gpvdm_select_filter":
					a.edit_box=gpvdm_select_filter(file_box=False)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="gpvdm_select_emission":
					a.edit_box=gpvdm_select_emission(file_box=False)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))

				elif result.widget=="gpvdm_select_shape":
					a.edit_box=gpvdm_select_shape(file_box=False)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))

				elif result.widget=="icon_widget":
					a.edit_box=icon_widget()
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))

				elif result.widget=="QLineEdit":
					a.edit_box=QLineEdit()
					a.edit_box.setMinimumSize(150, 25)
					if self.editable==False:
						a.edit_box.setReadOnly(True)

					a.edit_box.textChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
					#a.edit_box.show()
				elif result.widget=="QColorPicker":
					r=float(self.template_widget.color_r)
					g=float(self.template_widget.color_g)
					b=float(self.template_widget.color_b)
					alpha=float(self.template_widget.color_alpha)
					a.edit_box=QColorPicker(r,g,b,alpha)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="QColorPicker_one_line":
					vals=getattr(self.template_widget,token)
					a.edit_box=QColorPicker_one_line()
					a.edit_box.setText(vals)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.token=="fit_against":
					a.edit_box=QComboBoxLang()
					a.edit_box.setMinimumSize(150, 25)
					data=gpvdm_data()
					a.edit_box.addItemLang("self",_("This simulation"))
					for data_set in data.fits.fits.segments:
						a.edit_box.addItemLang(data_set.config.fit_name,data_set.config.fit_name)

					a.edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="QComboBoxLang":
					a.edit_box=QComboBoxLang()
					a.edit_box.setMinimumSize(150, 25)
					for i in range(0,len(result.defaults)):
						a.edit_box.addItemLang(result.defaults[i][0],result.defaults[i][1])
							
					a.edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="QComboBoxLayers":
					a.edit_box=QComboBoxLayers()
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="QComboBoxNewtonSelect":
					a.edit_box=QComboBoxNewtonSelect()
					a.edit_box.setMinimumSize(150, 25)
					for i in range(0,len(result.defaults)):
						a.edit_box.addItem(result.defaults[i])

					a.edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))

				elif result.widget=="QParasitic":
					a.edit_box=QParasitic()
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.textChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))

				elif result.widget=="QChangeLog":
					a.edit_box=QChangeLog(self)
					a.edit_box.setMinimumHeight(100)
					if self.editable==False:
						a.edit_box.setReadOnly(True)
					a.edit_box.textChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="generic_switch":
					a.edit_box=generic_switch(state0=result.defaults[0][0],state1=result.defaults[1][0],state0_value=result.defaults[0][1],state1_value=result.defaults[1][1],)
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="mobility_widget":
					a.edit_box=mobility_widget(electrons=result.defaults[0])
					a.edit_box.setMinimumSize(400, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="shape_dos_switch":
					a.edit_box=shape_dos_switch()
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="shape_electrical_switch":
					a.edit_box=shape_electrical_switch()
					a.edit_box.setMinimumSize(150, 25)
					a.edit_box.changed.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				elif result.widget=="QComboBox":
					a.edit_box=QComboBox()
					a.edit_box.setMinimumSize(150, 25)
					for i in range(0,len(result.defaults)):
						a.edit_box.addItem(result.defaults[i])
					
					a.edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,a.edit_box,unit,result))
				#print(token,value)
				

				if type(unit)==QPushButton:
					unit.clicked.connect(functools.partial(self.callback_unit_click,token,a.edit_box,unit))

				if draw_widget==True:
					#print(token,result.widget)
					a.token=token
					a.label=description
					a.units=unit
					a.widget=result.widget
					a.hide_on_true_token=result.hide_on_true_token
					a.hide_on_false_token=result.hide_on_false_token
					a.hide_on_token_eq=result.hide_on_token_eq
					a.show_on_token_eq=result.show_on_token_eq
					self.tab.addWidget(description,widget_number,0)
					if a.edit_box!=None:
						self.tab.addWidget(a.edit_box,widget_number,1)
					self.tab.addWidget(unit,widget_number,2)
					self.widget_list.append(a)										
					widget_number=widget_number+1

		self.update_values()
		self.hide_show_widgets()

	def update_values(self):
		#print("asad")
		#import traceback
		#traceback.print_stack()
		self.refind_template_widget()
		for item in self.widget_list:
			w=item.edit_box
			if w!=None:
				w.blockSignals(True)
				value=self.get_easy_value(item.token)

				if type(item.edit_box)==generic_switch:
					if value=="exponential":
						item.units.setEnabled(False)
					else:
						item.units.setEnabled(True)
				elif item.widget=="mobility_widget":
					if item.token=="symmetric_mobility_e":
						value = []
						value.append(self.template_widget.symmetric_mobility_e)
						value.append(self.template_widget.mue_z)
						value.append(self.template_widget.mue_x)
						value.append(self.template_widget.mue_y)
					if item.token=="symmetric_mobility_h":
						value = []
						value.append(self.template_widget.symmetric_mobility_h)
						value.append(self.template_widget.muh_z)
						value.append(self.template_widget.muh_x)
						value.append(self.template_widget.muh_y)

					if item.token=="electrical_symmetrical_resistance":
						value = []
						value.append(self.template_widget.electrical_symmetrical_resistance)
						value.append(self.template_widget.electrical_series_z)
						value.append(self.template_widget.electrical_series_x)
						value.append(self.template_widget.electrical_series_y)

				elif type(item.edit_box)==shape_dos_switch:
					value=value.enabled
					item.units.setEnabled(value)
				elif type(item.edit_box)==shape_electrical_switch:
					value=value.enabled
					item.units.setEnabled(value)
				#print(item.token)
				widget_set_value(item.edit_box,value)

				w.blockSignals(False)



	def callback_unit_click(self,token,widget,unit):
		if type(widget)==shape_dos_switch:
			electrical_file=self.template_widget.shape_dos
			from tab import tab_class
			self.window=tab_class(electrical_file)
			self.window.setWindowTitle(_("Electrical parameter editor for shape")+" "+self.template_widget.shape_name+" (https://www.gpvdm.com)") 
			self.window.show()

		if type(widget)==shape_electrical_switch:
			electrical_file=self.template_widget.shape_electrical
			from tab import tab_class
			self.window=tab_class(electrical_file)
			self.window.setWindowTitle(_("Electrical parameter editor for shape")+" "+self.template_widget.shape_name+" (https://www.gpvdm.com)") 
			self.window.show()

		if type(widget)==gpvdm_select_material:
			widget.callback_button_click()

		if type(widget)==gpvdm_select_filter:
			widget.callback_button_click()

		if type(widget)==gpvdm_select_emission:
			widget.callback_button_click()

		if type(widget)==gpvdm_select_shape:
			widget.callback_button_click()

		if type(widget)==icon_widget:
			widget.callback_button_click()


		if token=="dostype":
			from dos_editor import dos_editor
			self.dos_editor=dos_editor(self.json_path,self.uid)
			self.dos_editor.show()


	def hide_show_widgets(self):
		for w in self.widget_list:
			if w.edit_box!=None:
				w.edit_box.setVisible(True)
			w.units.setVisible(True)
			w.label.setVisible(True)

		for w in self.widget_list:
			if w.hide_on_true_token!="none":			
				for item in self.template_widget.var_list:
					json_token=item[0]
					json_val=getattr(self.template_widget,json_token)
					if json_token==w.hide_on_true_token:
						if json_val==True:
							if w.edit_box!=None:
								w.edit_box.setVisible(False)
							w.units.setVisible(False)
							w.label.setVisible(False)

			if w.hide_on_token_eq!=None:			
				for val in w.hide_on_token_eq:
					for item in self.template_widget.var_list:
						json_token=item[0]
						json_val=getattr(self.template_widget,json_token)
						if json_token==val[0]:
							if json_val==val[1]:
								if w.edit_box!=None:
									w.edit_box.setVisible(False)
								w.units.setVisible(False)
								w.label.setVisible(False)

			if w.hide_on_false_token!=[]:			
				for item in self.template_widget.var_list:
					json_token=item[0]
					json_val=getattr(self.template_widget,json_token)
					if json_token in w.hide_on_false_token:
						if json_val==False:
							if w.edit_box!=None:
								w.edit_box.setVisible(False)
							w.units.setVisible(False)
							w.label.setVisible(False)

			if w.show_on_token_eq!=None:
				do_hide=True
				found=False
				for val in w.show_on_token_eq:
					json_val=getattr(self.template_widget,val[0],None)
					if json_val!=None:
						found=True
					if json_val==val[1]:
						do_hide=False

				if do_hide==True and found==True:
					if w.edit_box!=None:		
						w.edit_box.setVisible(False)
					w.units.setVisible(False)
					w.label.setVisible(False)

	def callback_edit(self,token,widget,unit,token_class):
		val=widget_get_value(widget)
		#print("Edit",val)
		if val!=None:
			if token.startswith("symmetric_mobility_e")==True:
				self.template_widget.symmetric_mobility_e=val[0]
				self.template_widget.mue_z=float(val[1])
				self.template_widget.mue_x=float(val[2])
				self.template_widget.mue_y=float(val[3])
			elif token.startswith("symmetric_mobility_h")==True:
				self.template_widget.symmetric_mobility_h=val[0]
				self.template_widget.muh_z=float(val[1])
				self.template_widget.muh_x=float(val[2])
				self.template_widget.muh_y=float(val[3])
			elif token.startswith("electrical_symmetrical_resistance")==True:
				self.template_widget.electrical_symmetrical_resistance=val[0]
				self.template_widget.electrical_series_z=float(val[1])
				self.template_widget.electrical_series_x=float(val[2])
				self.template_widget.electrical_series_y=float(val[3])

			elif token.startswith("color_r")==True:
				setattr(self.template_widget, "color_r", widget.r)
				setattr(self.template_widget, "color_g", widget.g)
				setattr(self.template_widget, "color_b", widget.b)
				setattr(self.template_widget, "color_alpha", widget.alpha)
			elif type(widget)==shape_dos_switch:
				val=widget_get_value(widget)
				unit.setEnabled(val)
				self.template_widget.shape_dos.enabled=val
			elif type(widget)==shape_electrical_switch:
				val=widget_get_value(widget)
				unit.setEnabled(val)
				self.template_widget.shape_electrical.enabled=val
			else:
				orig_value=getattr(self.template_widget, token)
				#print(token,val,type(orig_value))
				if type(orig_value)==float:
					try:
						setattr(self.template_widget, token, float(val))
					except:
						pass
				elif type(orig_value)==int:
					try:
						setattr(self.template_widget, token, int(val))
					except:
						pass
				elif type(orig_value)==bool:
					setattr(self.template_widget, token, bool(val))
				else:
					setattr(self.template_widget, token, val)
				

			if type(widget)==QLineEdit:
				a=undo_list_class()
				if self.file_name!=None:
					a.add([self.file_name, token, val,widget])
			elif type(widget)==QChangeLog:
				a=undo_list_class()
				if self.file_name!=None:
					a.add([self.file_name, token, val,widget])
			if token=="dostype":
				if widget_get_value(widget)=="complex":
					unit.setEnabled(True)
				else:
					unit.setEnabled(False)


			self.hide_show_widgets()
			self.changed.emit(token)

	def set_edit(self,editable):
		self.editable=editable

	def callback_ref(self,token,widget):
		from ref import ref_window
		self.ref_window=ref_window(os.path.splitext(self.file_name)[0]+".bib",token)
		self.ref_window.show()


	def get_token(self,token):
		return self.f.get_token(token)


