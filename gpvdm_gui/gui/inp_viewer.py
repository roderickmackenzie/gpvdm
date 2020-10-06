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

## @package inp_viewer
#  This widget turns lists representing inp files into a gui allowing editing
#


import os

from token_lib import tokens
from undo import undo_list_class
from tab_base import tab_base
from str2bool import str2bool
from inp import inp_get_token_value
from util import latex_to_html
from gtkswitch import gtkswitch
from leftright import leftright
from gpvdm_select import gpvdm_select
from gpvdm_select_material import gpvdm_select_material
from gpvdm_select_emission import gpvdm_select_emission



from PyQt5.QtCore import pyqtSignal

from PyQt5.QtWidgets import QTextEdit,QWidget, QScrollArea,QVBoxLayout,QLabel,QHBoxLayout,QPushButton, QSizePolicy, QTableWidget, QTableWidgetItem,QComboBox,QGridLayout,QLineEdit
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QPixmap, QIcon

from QComboBoxLang import QComboBoxLang
from QColorPicker import QColorPicker
from QComboBoxNewtonSelect import QComboBoxNewtonSelect
from QComboBoxShape import QComboBoxShape

from PyQt5.QtCore import QTimer

import i18n
_ = i18n.language.gettext

import functools
from inp import inp_update_token_array
from error_dlg import error_dlg
from QParasitic import QParasitic
from inp import  inp_search_token_array
from generic_switch import generic_switch
from mobility_widget import mobility_widget
from shape_dos_switch import shape_dos_switch

from QChangeLog import QChangeLog
from QLabel_click import QLabel_click
from gui_util import widget_get_value
from gui_util import widget_set_value

from inp import inp
from bibtex import bibtex
from newton_solver import newton_solver_get_type

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

class inp_viewer(QWidget,tab_base):

	changed = pyqtSignal(str)

	def __init__(self):
		QWidget.__init__(self)
		self.editable=True
		self.widget_list=[]
		self.file_name=None
		self.tab=QGridLayout()

		self.setLayout(self.tab)
		self.f=inp()

	def populate(self,lines):

		self.f.lines=lines
		n=0
		pos=0
		my_token_lib=tokens()
		widget_number=0
		bib=bibtex()
		if self.file_name!=None:
			bib.load(os.path.splitext(self.file_name)[0]+".bib")
		while (1):
			draw_widget=True
			token,values=self.f.get_next_token_array()
			#print(token,values)
			if token=="#ver":
				break

			if token=="#end":
				break

			if token.startswith("#"):
				show=False
				units="Units"

				value=values[0]

				result=my_token_lib.find(token)
				if result!=False:
					#print(token,result.info)
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
						unit.setFixedSize(50, 25)

					#edit widget
					if result.widget=="gtkswitch":
						edit_box=gtkswitch()
						edit_box.setFixedSize(300, 25)
						edit_box.changed.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="leftright":
						edit_box=leftright()
						edit_box.setFixedSize(300, 25)
						edit_box.changed.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="gpvdm_select":
						edit_box=gpvdm_select(file_box=True)
						edit_box.setFixedSize(300, 25)
						edit_box.edit.textChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="gpvdm_select_material":
						edit_box=gpvdm_select_material(file_box=False)
						edit_box.setFixedSize(300, 25)
						edit_box.edit.textChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))

					elif result.widget=="gpvdm_select_emission":
						edit_box=gpvdm_select_emission(file_box=False)
						edit_box.setFixedSize(300, 25)
						edit_box.edit.textChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))

					elif result.widget=="QLineEdit":
						edit_box=QLineEdit()
						edit_box.setFixedSize(300, 25)
						if self.editable==False:
							edit_box.setReadOnly(True)

						edit_box.textChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
						#edit_box.show()
					elif result.widget=="QColorPicker":
						r=float(values[0])
						g=float(values[1])
						b=float(values[2])
						edit_box=QColorPicker(r,g,b)
						edit_box.setFixedSize(300, 25)
						edit_box.changed.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="QComboBoxLang":
						edit_box=QComboBoxLang()
						edit_box.setFixedSize(300, 25)
						for i in range(0,len(result.defaults)):
							edit_box.addItemLang(result.defaults[i][0],result.defaults[i][1])
								
						edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="QComboBoxNewtonSelect":
						edit_box=QComboBoxNewtonSelect()
						edit_box.setFixedSize(300, 25)
						for i in range(0,len(result.defaults)):
							edit_box.addItem(result.defaults[i])

						edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="QComboBoxShape":
						edit_box=QComboBoxShape()
						edit_box.setFixedSize(300, 25)
						edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))

					elif result.widget=="QParasitic":
						edit_box=QParasitic()
						edit_box.setFixedSize(300, 25)
						edit_box.textChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))

					elif result.widget=="QChangeLog":
						edit_box=QChangeLog(self)
						edit_box.setMinimumHeight(100)
						if self.editable==False:
							edit_box.setReadOnly(True)
						edit_box.textChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="generic_switch":
						edit_box=generic_switch(state0=result.defaults[0][0],state1=result.defaults[1][0],state0_value=result.defaults[0][1],state1_value=result.defaults[1][1],)
						edit_box.setFixedSize(300, 25)
						if value=="exponential":
							unit.setEnabled(False)
						edit_box.changed.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="mobility_widget":
						edit_box=mobility_widget(electrons=result.defaults[0])
						edit_box.setFixedSize(400, 25)
						if token=="#symmetric_mobility_e":
							value = [self.f.get_token(token),self.f.get_token("#mue_z"),self.f.get_token("#mue_x"),self.f.get_token("#mue_y")]
						if token=="#symmetric_mobility_h":
							value = [self.f.get_token(token),self.f.get_token("#muh_z"),self.f.get_token("#muh_x"),self.f.get_token("#muh_y")]
						if token=="#electrical_symmetrical_resistance":
							value = [self.f.get_token(token),self.f.get_token("#electrical_series_z"),self.f.get_token("#electrical_series_x"),self.f.get_token("#electrical_series_y")]

						edit_box.changed.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
					elif result.widget=="shape_dos_switch":
						if token=="#shape_dos":
							if newton_solver_get_type()!="newton_simple":
								edit_box=shape_dos_switch()
								edit_box.shape_file=self.file_name
								edit_box.setFixedSize(300, 25)
								edit_box.changed.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
								if value=="none":
									unit.setEnabled(False)
							else:
								draw_widget=False

						if token=="#shape_electrical":
							if newton_solver_get_type()=="newton_simple":
								edit_box=shape_dos_switch()
								edit_box.shape_file=self.file_name
								edit_box.setFixedSize(300, 25)
								edit_box.changed.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))
								if value=="none":
									unit.setEnabled(False)
							else:
								draw_widget=False

					else:
						edit_box=QComboBox()
						edit_box.setFixedSize(300, 25)
						for i in range(0,len(result.defaults)):
							edit_box.addItem(result.defaults[i])
						
						edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,token,edit_box,unit,result))

					widget_set_value(edit_box,value)

					if type(unit)==QPushButton:
						unit.clicked.connect(functools.partial(self.callback_unit_click,token,edit_box,unit))

					if draw_widget==True:
						a=tab_line()
						a.token=token
						a.label=description
						a.edit_box=edit_box
						a.units=unit
						a.widget=result.widget
						a.hide_on_true_token=result.hide_on_true_token
						a.hide_on_false_token=result.hide_on_false_token
						a.hide_on_token_eq=result.hide_on_token_eq

						self.widget_list.append(a)
						self.tab.addWidget(description,widget_number,0)
						self.tab.addWidget(edit_box,widget_number,1)
						self.tab.addWidget(unit,widget_number,2)
											
						widget_number=widget_number+1

		self.hide_show_widgets()

	def callback_unit_click(self,token,widget,unit):
		if type(widget)==shape_dos_switch:
			f=inp()
			f.load(self.file_name)
			if newton_solver_get_type()=="newton_simple":
				electrical_file=f.get_token("#shape_electrical")+".inp"
			else:
				electrical_file=f.get_token("#shape_dos")+".inp"

			from tab import tab_class
			self.window=tab_class(electrical_file)
			self.window.setWindowTitle(_("Electrical parameter editor for shape")+" "+f.get_token("#shape_name")+" (https://www.gpvdm.com)") 
			self.window.show()

		if type(widget)==gpvdm_select_material:
			widget.callback_button_click()

		if type(widget)==gpvdm_select_emission:
			widget.callback_button_click()

		if token=="#dostype":
			from dos_editor import dos_editor
			self.dos_editor=dos_editor(self.file_name)
			self.dos_editor.show()

	def hide_show_widgets(self):
		for w in self.widget_list:
			w.edit_box.setVisible(True)
			w.units.setVisible(True)
			w.label.setVisible(True)

		for w in self.widget_list:
			if w.hide_on_true_token!="none":			
				for ww in self.widget_list:
					if ww.token==w.hide_on_true_token:
						if str2bool(ww.edit_box.get_value())==True:
							w.edit_box.setVisible(False)
							w.units.setVisible(False)
							w.label.setVisible(False)

			if w.hide_on_token_eq!=None:			
				for ww in self.widget_list:
					for val in w.hide_on_token_eq:
						#print(ww.token,val[0])
						if ww.token==val[0]:
							if widget_get_value(ww.edit_box)==val[1]:
								w.edit_box.setVisible(False)
								w.units.setVisible(False)
								w.label.setVisible(False)

			if w.hide_on_token_eq!=None:			
				for ww in self.widget_list:
					for val in w.hide_on_token_eq:
						if w.token==val[0]:
							if widget_get_value(ww.edit_box)==val[1]:
								w.edit_box.setVisible(False)
								w.units.setVisible(False)
								w.label.setVisible(False)

			if w.hide_on_false_token!=[]:			
				for ww in self.widget_list:
					if ww.token in w.hide_on_false_token:
						if str2bool(ww.edit_box.get_value())==False:
							w.edit_box.setVisible(False)
							w.units.setVisible(False)
							w.label.setVisible(False)

	def callback_edit(self,token,widget,unit,token_class):
		#print("oh")
		val=widget_get_value(widget)
		if val!=None:
			if token_class.data_type=="float":
				try:
					float(widget_get_value(widget))
				except:
					return
			#print(token)
			if token.startswith("#symmetric_mobility_e")==True:
				self.f.replace("#symmetric_mobility_e", val[0])
				self.f.replace("#mue_z", val[1] )
				self.f.replace("#mue_x", val[2] )
				self.f.replace("#mue_y", val[3] )
			elif token.startswith("#symmetric_mobility_h")==True:
				self.f.replace("#symmetric_mobility_h", val[0])
				self.f.replace("#muh_z", val[1] )
				self.f.replace("#muh_x", val[2] )
				self.f.replace("#muh_y", val[3] )
			elif token.startswith("#electrical_symmetrical_resistance")==True:
				self.f.replace("#electrical_symmetrical_resistance", val[0])
				self.f.replace("#electrical_series_z", val[1] )
				self.f.replace("#electrical_series_x", val[2] )
				self.f.replace("#electrical_series_y", val[3] )
			else:
				self.f.replace(token, val)

			if type(widget)==QLineEdit:
				a=undo_list_class()
				if self.file_name!=None:
					a.add([self.file_name, token, val,widget])
			elif type(widget)==QChangeLog:
				a=undo_list_class()
				if self.file_name!=None:
					a.add([self.file_name, token, val,widget])
			if token=="#dostype":
				if widget_get_value(widget)=="complex":
					unit.setEnabled(True)
				else:
					unit.setEnabled(False)
			elif type(widget)==shape_dos_switch:
				if widget_get_value(widget)=="none":
					unit.setEnabled(False)
				else:
					unit.setEnabled(True)


			self.hide_show_widgets()
			self.changed.emit(token)

	def set_edit(self,editable):
		self.editable=editable

	def callback_ref(self,token,widget):
		from ref import ref_window
		self.ref_window=ref_window(os.path.splitext(self.file_name)[0]+".bib",token)
		self.ref_window.show()

	def update_lines(self,lines):
		self.f.lines=lines

		for w in self.widget_list:
			if w.edit_box.hasFocus()==False:
				values=self.f.get_token(w.token)

				widget_set_value(values[0])


	def get_token(self,token):
		return self.f.get_token(token)


