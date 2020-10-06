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

## @package order_widget
#  This is a general toolbar widget to handle sequential files, to make, clone, and delete them.
#


import os

from gui_util import dlg_get_text
from token_lib import tokens

from icon_lib import icon_get
from str2bool import str2bool

from util import wrap_text

from gui_util import yes_no_dlg

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QTableWidgetItem, QCheckBox
from PyQt5.QtGui import QPainter,QIcon
from PyQt5.QtCore import pyqtSignal

from inp import inp_save_lines_to_file
from inp import inp_load_file
from inp import inp
from inp import inp_copy_file
from inp import inp_update_token_value
from inp import inp_get_token_value
from inp import inp_remove_file
from inp import inp_issequential_file
from inp import inp_ls_seq_files

from cal_path import get_sim_path
from QWidgetSavePos import QWidgetSavePos
from QHTabBar import QHTabBar
from util_zip import zip_lsdir

from util import strextract_interger

from radiobutton import radiobutton
from error_dlg import error_dlg

import re

def extract_number_from_file_name(file_name):
	return re.sub("\D", "", os.path.basename(file_name))

class order_widget(QToolBar):

	added = pyqtSignal(str)
	changed = pyqtSignal()

	def load_tabs(self):
		files=[]
		file_list=zip_lsdir(os.path.join(get_sim_path(),"sim.gpvdm"))
		for i in range(0,len(file_list)):
			if inp_issequential_file(file_list[i],self.base_file_name[0])==True:
				print("search>>",file_list[i],self.name_token)
				name=inp_get_token_value(file_list[i], self.name_token)
				name=name.split("@")[0]

				files.append([name,file_list[i]])

		files.sort()
		for i in range(0,len(files)):
			self.added.emit(files[i][1])

	def new_filenumber(self):
		for i in range(0,20):
			new_filename=self.base_file_name[0]+str(i)+".inp"
			if inp().isfile(os.path.join(get_sim_path(),new_filename))==False:
				return str(i)
		return False

	def is_name(self,name):
		file_list=zip_lsdir(os.path.join(get_sim_path(),"sim.gpvdm"))
		files=[]
		for i in range(0,len(file_list)):
			if inp_issequential_file(file_list[i],self.base_file_name[0])==True:
				sim_name=inp_get_token_value(file_list[i], self.name_token)
				sim_name=sim_name.split("@")[0]
				if sim_name==name:
					return True
		return False

	def new_name(self):
		n=0
		while (1):
			name=self.new_tab_name+str(n)
			if self.is_name(name)==False:
				return name
			n=n+1
		return False


	def __init__(self):
		QToolBar.__init__(self)
		self.new_text=""
		self.delete_text=""
		self.clone_text=""
		self.rename_text=""
		self.icon_size=42
		self.new_dlg_text=""
		self.base_file_name="none"
		self.new_tab_name=""
		self.notebook_pointer=None
		self.clone_dlg_text=""
		self.rename_dlg_text=""
		self.delete_dlg_text=""
		self.postfix=""
		self.name_token="#tab_name"


	def init(self):

		if isinstance(self.base_file_name, list)==False:
			self.base_file_name=[self.base_file_name]

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(self.icon_size, self.icon_size))

		self.tb_new = QAction(icon_get("document-new"), wrap_text(self.new_text,2), self)
		self.tb_new.triggered.connect(self.callback_add_page)
		self.addAction(self.tb_new)

		self.tb_delete = QAction(icon_get("edit-delete"), wrap_text(self.delete_text,3), self)
		self.tb_delete.triggered.connect(self.callback_delete_page)
		self.addAction(self.tb_delete)

		self.tb_clone = QAction(icon_get("clone"), wrap_text(self.clone_text,3), self)
		self.tb_clone.triggered.connect(self.callback_clone_page)
		self.addAction(self.tb_clone)

		self.tb_rename = QAction(icon_get("rename"), wrap_text(self.rename_text,3), self)
		self.tb_rename.triggered.connect(self.callback_rename_page)
		self.addAction(self.tb_rename)

	def show_enable(self):
		self.enabled=radiobutton()
		self.enabled.changed.connect(self.callback_enable)
		self.addWidget(self.enabled)
		self.notebook_pointer.currentChanged.connect(self.callback_notebook_tab_changed)

	def callback_enable(self):
		file_list=zip_lsdir(os.path.join(get_sim_path(),"sim.gpvdm"))
		for i in range(0,len(file_list)):
			if inp_issequential_file(file_list[i],self.base_file_name[0])==True:
				inp_update_token_value(file_list[i], "#tab_enabled", "false")

		tab = self.notebook_pointer.currentWidget()
		inp_update_token_value(os.path.join(get_sim_path(),tab.file_name), "#tab_enabled", "true")

		self.enabled.set_value(True)


	def callback_notebook_tab_changed(self):
		tab = self.notebook_pointer.currentWidget()
		name=inp_get_token_value(tab.file_name, "#tab_enabled")
		if name!=None:
			self.enabled.set_value(str2bool(name))

	def callback_add_page(self):
		new_sim_name=dlg_get_text( self.new_dlg_text+":", self.new_name(),"document-new.png")

		files=inp_ls_seq_files(os.path.join(get_sim_path(),"sim.gpvdm"),self.base_file_name[0])
		if (len(files)==0):
			error_dlg(self,_("The last tab is gone, I have nothign to copy."))
			return
		old_file_number=int(files[0][len(self.base_file_name[0]):-4])
		if new_sim_name.ret!=None:
			number=self.new_filenumber()
			for b in range(0,len(self.base_file_name)):
				new_filename=self.base_file_name[b]+number+".inp"
				orig_filename=self.base_file_name[b]+"0.inp"
				inp_copy_file(os.path.join(get_sim_path(),new_filename),os.path.join(get_sim_path(),orig_filename))
				if b==0:
					inp_update_token_value(os.path.join(get_sim_path(),new_filename), self.name_token, new_sim_name.ret+self.postfix)
					emit_file_name=new_filename
			self.added.emit(emit_file_name)


	def callback_clone_page(self):
		tab = self.notebook_pointer.currentWidget()

		orig_number=extract_number_from_file_name(tab.file_name)
		new_sim_name=dlg_get_text( self.clone_dlg_text, self.new_name(),"clone.png")
		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			number=self.new_filenumber()
			for b in range(0,len(self.base_file_name)):
				new_filename=self.base_file_name[b]+number+".inp"
				orig_filename=self.base_file_name[b]+orig_number+".inp"
				inp_copy_file(os.path.join(get_sim_path(),new_filename),os.path.join(get_sim_path(),orig_filename))
				if b==0:
					inp_update_token_value(os.path.join(get_sim_path(),new_filename), self.name_token, new_sim_name+self.postfix)
					emit_file_name=new_filename

			self.added.emit(emit_file_name)
			self.changed.emit()

	def callback_rename_page(self):
		tab = self.notebook_pointer.currentWidget()
		name=inp_get_token_value(tab.file_name, self.name_token)
		name=name.split("@")[0]

		new_sim_name=dlg_get_text( self.rename_dlg_text+":", name,"rename.png")

		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			inp_update_token_value(tab.file_name, self.name_token, new_sim_name+self.postfix)
			index=self.notebook_pointer.currentIndex()
			self.notebook_pointer.setTabText(index, new_sim_name)
			self.changed.emit()

	def callback_delete_page(self):
		files=inp_ls_seq_files(os.path.join(get_sim_path(),"sim.gpvdm"),self.base_file_name[0])
		if (len(files)<2):
			error_dlg(self,_("You should not delete the last tab."))
			return

		tab = self.notebook_pointer.currentWidget()
		name=inp_get_token_value(tab.file_name, self.name_token)
		name=name.split("@")[0]

		response=yes_no_dlg(self,self.delete_dlg_text+name)

		if response == True:
			file_number=extract_number_from_file_name(tab.file_name)
			for b in self.base_file_name:
				file_name=b+file_number+".inp"
				inp_remove_file(os.path.join(get_sim_path(),file_name))

			index=self.notebook_pointer.currentIndex()
			self.notebook_pointer.removeTab(index)
			self.changed.emit()


