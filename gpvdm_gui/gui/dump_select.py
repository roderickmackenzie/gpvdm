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

## @package dump_select
#  Widget to select the files which should be dumped.
#

import os
import fnmatch

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView,QTreeWidgetItemIterator, QTreeWidget, QPushButton, QHBoxLayout, QTreeWidgetItem
from PyQt5.QtGui import QPainter,QIcon
from PyQt5.QtGui import QFont

from inp import inp_save
from inp import inp_load_file
from icon_lib import icon_get
from error_dlg import error_dlg
from str2bool import str2bool


class dump_select(QWidget):

	def __init__(self):
		QWidget.__init__(self)
			
		self.main_vbox=QVBoxLayout()
		self.save_function=None
		
		self.setWindowIcon(icon_get("scan"))

		self.setWindowTitle(_("Dump File Selector")+" (https://www.gpvdm.com)") 


		self.tab = QTreeWidget()
		self.tab.header().hide()
		#self.tab.setHeaderItem("Scan items")

		self.font = QFont()
#		self.font.setFamily('DejaVu Sans')
#		self.font.setBold(True)
#		self.font.setStyleHint(QFont.Monospace)
#		self.font.setFixedPitch(True)
		self.font.setPointSize(int(20))
	
		self.tab.setFont(self.font)
		

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(32, 32))

		self.refresh = QAction(icon_get("view-refresh"), _("Refresh"), self)
		self.refresh.triggered.connect(self.callback_refresh)
		toolbar.addAction(self.refresh)

		self.main_vbox.addWidget(toolbar)

		self.main_vbox.addWidget(self.tab)


		self.setLayout(self.main_vbox)

		#okButton.clicked.connect(self.tree_apply_click) 
		#cancelButton.clicked.connect(self.close)

		#self.tab.header().close() 
		self.update()
		self.save()
		self.tab.itemChanged.connect(self.callback_tree_checked)
		self.tab.itemClicked.connect(self.tree_callback_clicked)
		return

	def callback_tree_checked(self):
		self.save()

	def wildcard_replace(self,name):
		if fnmatch.fnmatch(name, "light_1d_*_n.dat")==True:
			name="light_1d_*_n.dat"

		if fnmatch.fnmatch(name, "light_1d_*_r.dat")==True:
			name="light_1d_*_r.dat"

		if fnmatch.fnmatch(name, "light_1d_*_E.dat")==True:
			name="light_1d_*_E.dat"

		if fnmatch.fnmatch(name, "light_1d_*_alpha.dat")==True:
			name="light_1d_*_alpha.dat"

		if fnmatch.fnmatch(name, "light_1d_*_photons.dat")==True:
			name="light_1d_*_photons.dat"

		if fnmatch.fnmatch(name, "light_1d_*_photons_norm.dat")==True:
			name="light_1d_*_photons_norm.dat"

		if fnmatch.fnmatch(name, "light_1d_*_alpha.dat")==True:
			name="light_1d_*_alpha.dat"

		if fnmatch.fnmatch(name, "light_1d_*_E_tot.dat")==True:
			name="light_1d_*_E_tot.dat"

		if fnmatch.fnmatch(name, "light_1d_*_E.dat")==True:
			name="light_1d_*_E.dat"

		if fnmatch.fnmatch(name, "light_1d_*_alpha.dat")==True:
			name="light_1d_*_alpha.dat"

		if fnmatch.fnmatch(name, "light_1d_*_pointing.dat")==True:
			name="light_1d_*_pointing.dat"

		if fnmatch.fnmatch(name, "light_1d_*_Ep.dat")==True:
			name="light_1d_*_Ep.dat"

		if fnmatch.fnmatch(name, "light_1d_*_En.dat")==True:
			name="light_1d_*_En.dat"

		if fnmatch.fnmatch(name, "light_1d_*_t.dat")==True:
			name="light_1d_*_t.dat"

		if fnmatch.fnmatch(name, "light_1d_*_layer.dat")==True:
			name="light_1d_*_layer.dat"

		if fnmatch.fnmatch(name, "light_1d_*_photons_abs.dat")==True:
			name="light_1d_*_photons_abs.dat"

		if fnmatch.fnmatch(name, "stark_spectrum*.dat")==True:
			name="stark_spectrum*.dat"


		return name

	def make_entry(self,root,text,true_false):
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
				pointer.setFlags(pointer.flags() | Qt.ItemIsUserCheckable)
				#if depth==0:
				#	pointer.setIcon(0,icon_get("folder"))

				if true_false==True:
					pointer.setCheckState(0, Qt.Checked ) 
				else:
					pointer.setCheckState(0, Qt.Unchecked ) 

	
	def scan_dir(self,directory):
		outout_files=[]
		true_false_list=[]
		for root, dirs, files in os.walk(directory):
			for name in files:
				if name.endswith(".dat")==True:
					name=self.wildcard_replace(name)
						
					full_name=directory+"/"+name

					if outout_files.count(full_name)==0:
						outout_files.append(full_name)
						true_false_list.append(True)
					#print(os.path.join(root, name))	
		return outout_files,true_false_list

	def scan_root_dir(self):
		outout_files=[]
		true_false_list=[]
		for name in os.listdir():
			if name.endswith(".dat")==True:
				name=self.wildcard_replace(name)
					
				full_name="root/"+name

				if outout_files.count(full_name)==0:
					outout_files.append(full_name)
					true_false_list.append(True)
				#print(os.path.join(root, name))	

		return outout_files,true_false_list

	def set_state(self,value):
		self.tab.blockSignals(True)
		iterator = QTreeWidgetItemIterator(self.tab)

		while iterator.value():
			item = iterator.value()

			path = []

			while item is not None:
				path.append(str(item.text(0)))
				item = item.parent()

			path="/".join(reversed(path))
			path=path.split('/')
			if len(path)>1:
				path=path[1:]
				if path[0]==value:
					item = iterator.value()
					if len(path)==1:
						checked=item.checkState(0)
						#checked=bool(checked)

					if len(path)>1:
						item.setCheckState(0, checked)

			iterator+=1

		self.tab.blockSignals(False)

	def save(self):
		lines=0
		count=0
		out=[]

		iterator = QTreeWidgetItemIterator(self.tab)

		while iterator.value():
			item = iterator.value()
			lines=lines+1
			iterator+=1

		iterator = QTreeWidgetItemIterator(self.tab)

		while iterator.value():
			item = iterator.value()

			path = []
			checked=item.checkState(0)
			checked=bool(checked)
			while item is not None:
				path.append(str(item.text(0)))
				item = item.parent()
			
			path="/".join(reversed(path))
			path=path.split('/', 1)[-1]
			#print(path.encode('utf-8'))
			if path.count("/")!=0:
				if checked==False:
					out.append("#file"+str(count))
					out.append(path)
					out.append(str(checked))
					count=count+1

			iterator+=1

		out.append("#ver")
		out.append("1.0")
		out.append("#end")
		#if len(out)>10:
		inp_save("dump_file.inp",out)
		#else:
		#	print("************Warning dump_file.inp looks too short to me***********")

	def from_file(self):
		param_list=[]
		true_false_list=[]
		lines=inp_load_file("dump_file.inp")

		if lines!=False:
			pos=0
			while(1):
				token=lines[pos]
				pos=pos+1
				
				if token=="#ver":
					break

				file_name=lines[pos]
				pos=pos+1

				true_false=lines[pos]
				pos=pos+1

				true_false_list.append(str2bool(true_false))
				param_list.append(file_name)

		return param_list,true_false_list

	def callback_refresh(self):
		self.update()

	def update(self):
		self.tab.clear()
		
		param_list=[]
		tf_list=[]

		files,true_false=self.from_file()
		param_list.extend(files)
		tf_list.extend(true_false)

		files,true_false=self.scan_dir("snapshots")
		param_list.extend(files)
		tf_list.extend(true_false)

		files,true_false=self.scan_dir("optical_output")
		param_list.extend(files)
		tf_list.extend(true_false)

		files,true_false=self.scan_dir("dynamic")
		param_list.extend(files)
		tf_list.extend(true_false)

		files,true_false=self.scan_dir("solver")
		param_list.extend(files)
		tf_list.extend(true_false)

		files,true_false=self.scan_dir("equilibrium")
		param_list.extend(files)
		tf_list.extend(true_false)

		files,true_false=self.scan_root_dir()
		param_list.extend(files)
		tf_list.extend(true_false)

		root = QTreeWidgetItem(self.tab, [_("Output files")])
		root.setExpanded(True)

		i=0
		for item in range(0, len(param_list)):
			div_str=param_list[item].replace("\\", "/")
			div_str=div_str.split("/")
			piter=None
			self.make_entry(root,div_str,tf_list[item])

	 

	def on_destroy(self):
		self.hide()
		return True

	
	def tree_callback_clicked(self, item, column):
		path = []
		while item is not None:
			path.append(str(item.text(0)))
			item = item.parent()

		if len(path)==2:
			self.set_state(path[0])
			
		self.save()







