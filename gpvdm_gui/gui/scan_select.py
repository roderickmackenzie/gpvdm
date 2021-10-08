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

## @package scan_select
#  Window to select a parameter to scan.
#

import os

import i18n
_ = i18n.language.gettext

#qt
import json
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QTreeWidget, QPushButton, QHBoxLayout, QTreeWidgetItem
from PyQt5.QtGui import QPainter,QIcon
from PyQt5.QtGui import QFont

from icon_lib import icon_get
from error_dlg import error_dlg
from gpvdm_json import gpvdm_data
from token_lib import tokens
from scan_human_labels import get_json_path_from_human_path

class select_param(QWidget):

	def set_save_function(self,save_function):
		self.save_function=save_function

	def __init__(self,treeview):
		QWidget.__init__(self)
		self.my_token_lib=tokens()
		self.dest_treeview=treeview
		self.setMinimumSize(700,700)
		self.human_path_col=0
		self.json_path_col=-1
		self.main_vbox=QVBoxLayout()
		self.save_function=None
		
		self.setWindowIcon(icon_get("scan"))

		self.setWindowTitle(_("Select simulation parameter")+" (https://www.gpvdm.com)") 


		self.tab = QTreeWidget()
		#self.tab.setHeaderItem("Scan items")

		self.font = QFont()
#		self.font.setFamily('DejaVu Sans')
#		self.font.setBold(True)
#		self.font.setStyleHint(QFont.Monospace)
#		self.font.setFixedPitch(True)
		self.font.setPointSize(int(20))
	
		self.tab.setFont(self.font)
		
		self.main_vbox.addWidget(self.tab)

		self.hwidget=QWidget()

		okButton = QPushButton(_("OK"))
		cancelButton = QPushButton(_("Cancel"))

		hbox = QHBoxLayout()
		hbox.addStretch(1)
		hbox.addWidget(okButton)
		hbox.addWidget(cancelButton)

		self.hwidget.setLayout(hbox)

		self.main_vbox.addWidget(self.hwidget)

		self.setLayout(self.main_vbox)

		okButton.clicked.connect(self.tree_apply_click) 
		cancelButton.clicked.connect(self.close)

		self.tab.header().close() 
		self.update()

	def json_decode(self,dic,path,pointer):
		for key in dic:
			item=path+"/"+key

			name=str(key)
			#try to decode it
			token=self.my_token_lib.find_json(str(key))
			if token!=False:
				name=token.info

			#see if it is a shape with a name
			try:
				name=dic[key]['shape_name']
			except:
				pass

			pointer_next=QTreeWidgetItem(pointer, [name])

			if type(dic[key])==dict:
				self.json_decode(dic[key],item,pointer_next)


	def update(self):
		self.tab.clear()
		root = QTreeWidgetItem(self.tab, [_("Simulation parameters")])
		root.setExpanded(True)
		data=gpvdm_data()
		lines=data.gen_json()
		lines="\n".join(lines)
		decode=json.loads(lines)
		self.json_decode(decode,"",root)

	def on_destroy(self):
		self.hide()
		return True

	def cal_path(self):
		getSelected = self.tab.selectedItems()
		if getSelected:
			item = getSelected[0]
#			getChildNode = baseNode.text(0)


			path = []
			while item is not None:
				path.append(str(item.text(0)))
				item = item.parent()

		ret="/".join(reversed(path))
		ret=ret.split('/', 1)[-1]
		return ret
	
	def tree_apply_click(self):
		if self.dest_treeview==None:
			return
		
		index = self.dest_treeview.selectionModel().selectedRows()
		if len(index)>0:
			pos=index[0].row()
			path=self.cal_path()
			self.dest_treeview.set_value(pos,self.human_path_col,path)

			if self.json_path_col!=-1:
				data=gpvdm_data()
				self.dest_treeview.set_value(pos,self.json_path_col,get_json_path_from_human_path(data,path))

			if self.save_function!=None:
				self.save_function()

			self.close()
		else:
			error_dlg(self,_("No row selected in the scan window, can't insert the selection"))





