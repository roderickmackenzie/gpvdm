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

## @package gpvdm_tab2
#  A table widget
#

import os

#qt
from PyQt5.QtWidgets import QTextEdit, QAction, QMenu
from PyQt5.QtCore import QSize, Qt , QPersistentModelIndex
from PyQt5.QtWidgets import QWidget,QPushButton,QToolBar, QVBoxLayout, QTableWidget,QAbstractItemView, QTableWidgetItem, QComboBox, QApplication, QLineEdit
from PyQt5.QtGui import QCursor
from PyQt5.QtCore import pyqtSignal

from QComboBoxLang import QComboBoxLang
from icon_lib import icon_get

from gpvdm_select import gpvdm_select
from energy_to_charge import energy_to_charge

from gtkswitch import gtkswitch
from leftright import leftright
from gpvdm_select_material import gpvdm_select_material
from gpvdm_applied_voltage import gpvdm_applied_voltage
from gpvdm_select_filter import gpvdm_select_filter
from tb_spectrum import tb_spectrum
from str2bool import str2bool
from gpvdm_json import gpvdm_data
from json_base import isclass
from token_lib import tokens
from json_base import json_base
from tab_button import tab_button
import copy
import json

class gpvdm_tab2(QTableWidget):

	new_row_clicked = pyqtSignal(int)
	changed = pyqtSignal()

	def __init__(self,toolbar=None):
		QTableWidget.__init__(self)
		self.json_search_path=None
		self.uid=None
		self.postfix=None
		self.json_tokens=[]
		self.base_obj=None
		self.toolbar=toolbar
		self.paste_callback=None
		self.check_enabled_callback=None
		self.disable_callbacks=False
		self.menu_disabled=False
		self.setSelectionBehavior(QAbstractItemView.SelectItems)
		self.SelectionMode (QAbstractItemView.SingleSelection)
		
		if self.toolbar!=None:
			self.toolbar.setIconSize(QSize(32, 32))
			self.tb_add = QAction(icon_get("list-add"), _("Add"), self)
			self.toolbar.addAction(self.tb_add)
			self.tb_add.triggered.connect(self.callback_add_row)

			self.tb_remove = QAction(icon_get("list-remove"), _("Delete row"), self)
			self.toolbar.addAction(self.tb_remove)
			self.tb_remove.triggered.connect(self.callback_remove_rows)

			self.tb_down= QAction(icon_get("go-down"), _("Move down"), self)
			self.toolbar.addAction(self.tb_down)
			self.tb_down.triggered.connect(self.callback_move_down)

			self.tb_up= QAction(icon_get("go-up"), _("Move up"), self)
			self.toolbar.addAction(self.tb_up)
			self.tb_up.triggered.connect(self.callback_move_up)

		self.menu = QMenu(self)
		self.menu_copy = QAction(_("Copy"), self)
		self.menu_copy.triggered.connect(self.callback_menu_copy)
		self.menu.addAction(self.menu_copy)

		self.menu_paste = QAction(_("Paste"), self)
		self.menu.addAction(self.menu_paste)
		self.menu_paste.triggered.connect(self.callback_menu_paste)

		self.menu_delete = QAction(icon_get("list-remove"),_("Delete row"), self)
		self.menu.addAction(self.menu_delete)
		self.menu_delete.triggered.connect(self.callback_remove_rows)

		self.menu_move_up = QAction(icon_get("go-up"),_("Move up"), self)
		self.menu.addAction(self.menu_move_up)
		self.menu_move_up.triggered.connect(self.callback_move_up)

		self.menu_move_down = QAction(icon_get("go-down"),_("Move down"), self)
		self.menu.addAction(self.menu_move_down)
		self.menu_move_down.triggered.connect(self.callback_move_down)

		self.my_token_lib=tokens()
		self.verticalHeader().setVisible(False)
		self.cellChanged.connect(self.callback_value_changed)

		self.callback_a=None
		self.fixup_new_row=None

	def callback_add_row(self):
		if self.base_obj==None:
			print("Update to use self.base_obj")
			self.new_row_clicked.emit(self.get_new_row_pos())
			return
		row=self.get_new_row_pos()
		obj=copy.deepcopy(self.base_obj)
		obj.update_random_ids()
		path=self.get_json_obj()
		path.insert(row,obj)
		self.insert_row(obj,row)
		self.changed.emit()

	def set_tokens(self,tokens):
		self.json_tokens=tokens
		self.blockSignals(True)
		self.clear()
		self.setColumnCount(len(self.json_tokens))

		self.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.blockSignals(False)

	def set_labels(self,labels):
		self.json_labels=labels
		self.setHorizontalHeaderLabels(self.json_labels)

	def insert_class(self,s,y):
		if isclass(s):
			x=0
			self.insertRow(y)
			for t in self.json_tokens:
				token=self.my_token_lib.find_json(t)
				added=False
				if token!=False:
					if token.widget!="QLineEdit":
						widget_name=token.widget
						item1=eval(widget_name+"()")
						self.setCellWidget(y,x, item1)
						added=True

				if added==False:
					widget_name="none"
					item1 = QTableWidgetItem()
					if self.check_enabled_callback!=None:
						if self.check_enabled_callback(s,t)==False:
							item1.setFlags(item1.flags() ^ Qt.ItemIsEnabled)
					self.setItem(y,x,item1)
				
				if widget_name=="QComboBoxLang":
					for i in range(0,len(token.defaults)):
						item1.addItemLang(token.defaults[i][0],token.defaults[i][1])
					item1.currentIndexChanged.connect(self.callback_value_changed)
				elif widget_name=="gpvdm_select_material":
					item1.changed.connect(self.callback_value_changed)
				elif widget_name=="gpvdm_select_filter":
					item1.changed.connect(self.callback_value_changed)
				elif widget_name=="gpvdm_applied_voltage":
					item1.changed.connect(self.callback_value_changed_direct)
				elif widget_name=="gpvdm_select":
					item1.edit.textChanged.connect(self.callback_value_changed)
					if self.callback_a!=None:
						item1.button.clicked.connect(self.callback_a)
				elif widget_name=="gtkswitch":
					item1.changed.connect(self.callback_value_changed)
				elif widget_name=="tb_spectrum":
					item1.currentIndexChanged.connect(self.callback_value_changed)


				x=x+1
			self.update_row(y)

			if self.fixup_new_row!=None:
				self.fixup_new_row(y)

	def update_row(self,y):
		segments=self.get_json_obj()

		x=0
		for t in self.json_tokens:
			s=segments[y]
			#print(type(s),segments[0].shape_name,segments[1].shape_name)
			for a in t.split("."):
				
				s=getattr(s, a,None)
			#if t=="shape_name":
			#	print(t,str(s))
			self.set_value(y,x,str(s),row=segments[y])
			x=x+1

	def callback_value_changed(self):
		segments=self.get_json_obj()
		if segments!=None:
			y=0
			for segment in segments:
				x=0
				for t in self.json_tokens:
					s=segment
					found=True
					for a in t.split("."):
						s_last=s
						t_last=a
						try:
							s=getattr(s,a)
						except:
							found=False
							break

					if found==True:
						orig_json_type=type(s)

						if orig_json_type==str:
							setattr(s_last,t_last,str(self.get_value(y,x)))
						if orig_json_type==bool:
							setattr(s_last,t_last,bool(self.get_value(y,x)))
						elif orig_json_type==float:
							try:
								setattr(s_last,t_last,float(self.get_value(y,x)))
							except:
								pass
					x=x+1
				y=y+1
		self.changed.emit()

	def callback_value_changed_direct(self):
		self.changed.emit()

	def get_json_obj(self):
		if self.uid==None:
			if self.json_search_path==None:
				return None
			ret=eval(self.json_search_path)
		else:
			path=eval(self.json_search_path)
			if self.postfix==None:
				ret=path.find_object_by_id(self.uid)
			else:
				ret=path.find_object_by_id(self.uid)
				for a in self.postfix.split("."):
					ret=getattr(ret, a,None)
		return ret

	def populate(self):
		self.blockSignals(True)
		path=self.get_json_obj()
		y=0
		for s in path:
			self.insert_class(s,y)
			y=y+1

		self.blockSignals(False)

	def callback_menu_copy(self):
		if self.rowCount()==0:
			return
		path=self.get_json_obj()
		b=json_base("",segment_class=True)

		rows=self.selectionModel().selectedRows()

		for a in rows:
			row_number=a.row()
			b.segments.append(path[row_number])

		cb = QApplication.clipboard()
		cb.clear(mode=cb.Clipboard )
		cb.setText("\n".join(b.gen_json())[3:], mode=cb.Clipboard)


	def callback_menu_paste(self):
		self.blockSignals(True)
		cb = QApplication.clipboard()
		text=cb.text()
		json_data=json.loads(text)
		for n in range(0,json_data['segments']):
			row=self.get_new_row_pos()
			obj=copy.deepcopy(self.base_obj)
			obj.load_from_json(json_data["segment"+str(n)])
			obj.update_random_ids()
			path=self.get_json_obj()
			path.insert(row,obj)
			self.insert_row(obj,row)

		self.blockSignals(False)

	def contextMenuEvent(self, event):
		if self.menu_disabled==False:
			self.menu.popup(QCursor.pos())


	def set_value(self,y,x,value,row=None):

		self.setUpdatesEnabled(False)
		if 1==1:
			if type(self.cellWidget(y, x))==QComboBox:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).setCurrentIndex(self.cellWidget(y, x).findText(value))
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y, x))==QComboBoxLang:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).setValue_using_english(value)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==gpvdm_select:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).setText(value)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==energy_to_charge:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).updateValue(row.id)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==gpvdm_select_material:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).setText(value)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==gpvdm_select_filter:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).setText(value)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==QLineEdit:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).setText(value)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==gpvdm_applied_voltage:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).updateValue(row.id)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==gtkswitch:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).set_value(str2bool(value))
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==tb_spectrum:
				self.cellWidget(y, x).blockSignals(True)
				self.cellWidget(y, x).set_value(value)
				self.cellWidget(y, x).blockSignals(False)
			elif type(self.cellWidget(y,x))==tab_button:
				pass
			else:
				item = self.item(y, x)
				if type(item)==QTableWidgetItem:
					item.setText(value)

		self.setUpdatesEnabled(True)


	def callback_move_down(self):
		ret=-1

		if self.rowCount()==0:
			return -1

		a=self.selectionModel().selectedRows()

		if len(a)>0:
			a=a[0].row()

			b=a+1
			if b>self.rowCount()-1:
				return

			self.blockSignals(True)
			path=self.get_json_obj()
			path[a], path[b] = path[b], path[a]
			self.update_row(a)
			self.update_row(b)
			self.selectRow(b)
			self.blockSignals(False)
			self.changed.emit()


	def get_value(self,y,x):
		if type(self.cellWidget(y, x))==QComboBox:
			return self.cellWidget(y, x).currentText()
		elif type(self.cellWidget(y, x))==QComboBoxLang:
			return self.cellWidget(y, x).currentText_english()
		elif type(self.cellWidget(y,x))==gpvdm_select:
			return self.cellWidget(y, x).text()
		elif type(self.cellWidget(y,x))==energy_to_charge:
			return self.cellWidget(y, x).text()
		elif type(self.cellWidget(y,x))==leftright:
			return self.cellWidget(y, x).get_value()
		elif type(self.cellWidget(y,x))==gtkswitch:
			return self.cellWidget(y, x).get_value()
		elif type(self.cellWidget(y,x))==tb_spectrum:
			return self.cellWidget(y, x).get_value()
		elif type(self.cellWidget(y,x))==gpvdm_select_material:
			return self.cellWidget(y, x).text()
		elif type(self.cellWidget(y,x))==gpvdm_select_filter:
			return self.cellWidget(y, x).text()
		elif type(self.cellWidget(y,x))==QLineEdit:
			return self.cellWidget(y, x).text()
		elif type(self.cellWidget(y,x))==gpvdm_applied_voltage:
			return self.cellWidget(y, x).text()
		elif type(self.cellWidget(y,x))==tab_button:
			pass
		else:
			item = self.item(y, x)
			if type(item)==QTableWidgetItem:
				return item.text()

		return None

	def get_new_row_pos(self):
		index = self.selectionModel().selectedRows()

		if len(index)>0:
			pos=index[0].row()+1
		else:
			pos = self.rowCount()

		return pos

	def insert_row(self,obj,y):
		self.blockSignals(True)
		new_row_pos=self.get_new_row_pos()

		self.insert_class(obj,y)
		
		self.blockSignals(False)

	def callback_move_up(self):
		ret=-1
		if self.rowCount()==0:
			return ret

		
		a=self.selectionModel().selectedRows()

		if len(a)==1:
			a=a[0].row()	

			b=a-1
			if b<0:
				return
				#b=tab.rowCount()-1

			self.blockSignals(True)
			path=self.get_json_obj()
			path[b], path[a] = path[a], path[b]
			self.update_row(a)
			self.update_row(b)
			self.selectRow(b)
			self.blockSignals(False)
			self.changed.emit()

	def get_selected(self):
		a=self.selectionModel().selectedRows()

		if len(a)<=0:
			return False

		ret=[]
		
		for ii in range(0,len(a)):
			y=a[ii].row()
			for i in range(0,self.columnCount()):
				ret.append(str(self.get_value(y,i)))

		return ret

	def callback_remove_rows(self):
		self.blockSignals(True)
		path=self.get_json_obj()
		rows = []
		for index in self.selectedIndexes():
			row=index.row()
			if row not in rows:
				rows.append(row) 

		for row in sorted(rows, reverse=True):
			self.removeRow(row)
			path.pop(row)

		self.blockSignals(False)
		self.changed.emit()

