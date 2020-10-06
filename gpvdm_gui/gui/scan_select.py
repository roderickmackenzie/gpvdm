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

## @package scan_select
#  Window to select a parameter to scan.
#

import os

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QTreeWidget, QPushButton, QHBoxLayout, QTreeWidgetItem
from PyQt5.QtGui import QPainter,QIcon
from PyQt5.QtGui import QFont

from icon_lib import icon_get
from error_dlg import error_dlg
from scan_human_labels import get_scan_human_labels

class select_param(QWidget):

	def set_save_function(self,save_function):
		self.save_function=save_function

	def __init__(self,treeview):
		QWidget.__init__(self)
		self.dest_treeview=treeview
		self.setFixedSize(500,700)
		self.file_name_tab_pos=0
		self.token_tab_pos=1
		self.path_tab_pos=2
			
		self.main_vbox=QVBoxLayout()
		self.save_function=None
		
		self.setWindowIcon(icon_get("scan"))

		self.setWindowTitle(_("Select simulation parameter")+" (https://www.gpvdm.com)") 


		self.tab = QTreeWidget()
		self.scan_human_labels=get_scan_human_labels()
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

		#self.tab.itemSelectionChanged.connect(self.tree_apply_click)
		self.tab.header().close() 
		self.update()



	def make_entry(self,root,text):
		depth=0
		pointer=root
		for depth in range(0,len(text)):
			found=False
			for i in range(0,pointer.childCount()):
				if pointer.child(i).text(0)==text[depth]:
					found=True
					pointer=pointer.child(i)
					break
			if found==False:
				pointer=QTreeWidgetItem(pointer, [text[depth]])
				


	def update(self):
		self.tab.clear()
		self.scan_human_labels.populate_from_files()
		root = QTreeWidgetItem(self.tab, [_("Simulation parameters")])
		root.setExpanded(True)
		param_list=self.scan_human_labels.list
		i=0
		for item in range(0, len(param_list)):
			div_str=param_list[item].human_label.replace("\\", "/")
			div_str=div_str.split("/")
			piter=None
			self.make_entry(root,div_str)

	 

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
		ret=ret.replace("/", os.path.sep)
		return ret
	
	def tree_apply_click(self):
		index = self.dest_treeview.selectionModel().selectedRows()
		if len(index)>0:
			#print("row=",index[0].row(),len(index))
			pos=index[0].row()
			path=self.cal_path()
			file_name=self.scan_human_labels.get_file_from_human_label(path)
			token=self.scan_human_labels.get_token_from_human_label(path)

			self.dest_treeview.set_value(pos,self.file_name_tab_pos,file_name)
			self.dest_treeview.set_value(pos,self.token_tab_pos,token)
			self.dest_treeview.set_value(pos,self.path_tab_pos,path)

			if self.save_function!=None:
				self.save_function()

			self.close()
		else:
			error_dlg(self,_("No row selected in the scan window, can't insert the selection"))





