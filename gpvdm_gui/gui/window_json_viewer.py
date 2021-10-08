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

## @package window_json_viewer
#  Window to select a parameter to scan.
#

import os

import i18n
_ = i18n.language.gettext

#qt
import json
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QSizePolicy,QTabWidget,QTableWidget,QAbstractItemView, QTreeWidget, QPushButton, QHBoxLayout, QTreeWidgetItem, QLabel, QLineEdit
from PyQt5.QtGui import QPainter,QIcon
from PyQt5.QtGui import QFont

from icon_lib import icon_get
from error_dlg import error_dlg
from gpvdm_json import gpvdm_data
from token_lib import tokens
from scan_human_labels import get_json_path_from_human_path
from json_base import isclass

class window_json_viewer(QWidget):


	def __init__(self):
		QWidget.__init__(self)
		self.my_token_lib=tokens()
		self.setMinimumSize(700,700)
		self.human_path_col=0
		self.json_path_col=-1
		self.main_vbox=QVBoxLayout()
		self.save_function=None
		
		self.setWindowIcon(icon_get("json_python"))

		self.setWindowTitle(_("Explore the simulation data structure")+" (https://www.gpvdm.com)") 


		self.tab = QTreeWidget()
		#self.tab.setHeaderItem("Scan items")

		self.font = QFont()
		self.font.setPointSize(int(20))
	
		self.tab.setFont(self.font)
		self.edit_box=QLineEdit()
		self.main_vbox.addWidget(self.edit_box)

		self.main_vbox.addWidget(self.tab)

		self.hwidget=QWidget()

		self.setLayout(self.main_vbox)

		self.tab.header().close()
		self.tab.itemClicked.connect(self.update_path)
		self.update()

	def json_decode(self,class_data,path,pointer):
		class obj_info:
			def __init__(self):
				self.var_name=None
				self.pointer_to_var=None
				self.icon=None

		var_list=[]
		var_icon=icon_get("python_var")
		class_icon=icon_get("python_object")
		if "var_list" in dir(class_data):
			for v in class_data.var_list:
				a=obj_info()
				a.var_name=v[0]
				a.pointer_to_var=getattr(class_data, v[0])
				var_list.append(a)

		if "layers" in dir(class_data):
			for l in range(0,len(class_data.layers)):
				a=obj_info()
				a.var_name="layers["+str(l)+"]"
				a.pointer_to_var=class_data.layers[l]
				var_list.append(a)

		if "segments" in dir(class_data):
			for l in range(0,len(class_data.segments)):
				a=obj_info()
				a.var_name="segments["+str(l)+"]"
				a.pointer_to_var=class_data.segments[l]
				var_list.append(a)

		#find icons
		for o in var_list:
			if isclass(o.pointer_to_var)==False:
				o.icon=var_icon
			else:
				o.icon=class_icon
				if "icon" in dir(a.pointer_to_var):
					o.icon=icon_get(a.pointer_to_var.icon)

		for o in var_list:
			item=path+"/"+o.var_name
			if isclass(o.pointer_to_var)==False:
				pointer_next=QTreeWidgetItem(pointer, [o.var_name])
				pointer_next.setIcon(0,o.icon)
				pointer_next=QTreeWidgetItem(pointer_next)
				label=QLabel("value=<font color=\"green\">"+str(o.pointer_to_var)+"</font>")
				self.tab.setItemWidget(pointer_next, 0, label);

			else:
				pointer_next=QTreeWidgetItem(pointer, [o.var_name])
				pointer_next.setIcon(0,o.icon)

			if isclass(o.pointer_to_var)==True:
				self.json_decode(o.pointer_to_var,item,pointer_next)


	def update(self):
		self.tab.clear()
		root = QTreeWidgetItem(self.tab, ["data"])
		root.setExpanded(True)
		data=gpvdm_data()
		self.json_decode(data,"",root)


	def update_path(self):
		getSelected = self.tab.selectedItems()
		if getSelected:
			item = getSelected[0]

			path = []
			while item is not None:
				path.append(str(item.text(0)))
				item = item.parent()

		ret=".".join(reversed(path))
		self.edit_box.setText(ret)



