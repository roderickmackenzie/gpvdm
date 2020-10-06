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

## @package fxexperiment_tab
#  fx experiment tab widget
#

import os
from numpy import *
from inp import inp_load_file
from inp import inp_search_token_value
from fxmesh import tab_fxmesh
from inp import inp_update_token_value

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTabWidget
from PyQt5.QtGui import QPainter,QIcon
from inp import inp_update_token_value
from inp import inp_get_token_value

from tab import tab_class
from cal_path import get_sim_path

class cvexperiment_tab(QTabWidget):

	def update(self):
		self.fxmesh.update()

	def save_image(self):
		tab = self.currentWidget()
		tab.save_image()
	

	def init(self,index):
		QTabWidget.__init__(self)

		self.index=index

		lines=[]
		self.file_name=os.path.join(get_sim_path(),"cv"+str(self.index)+".inp")
		lines=inp_load_file(self.file_name)
		if lines!=False:
			self.tab_name=inp_search_token_value(lines, "#sim_menu_name")
		else:
			self.tab_name=""

		widget=tab_class(self.file_name)
		self.addTab(widget,_("CV Configure"))


		file_name=os.path.join(get_sim_path(),"cv_fxdomain_data"+str(self.index)+".inp")
		widget=tab_class(file_name)
		self.addTab(widget,_("FX domain simulation"))

		self.file_name=os.path.join(get_sim_path(),"cv"+str(self.index)+".inp")

	def set_tab_caption(self,name):
		mytext=name
		if len(mytext)<10:
			for i in range(len(mytext),10):
				mytext=mytext+" "
		self.label.set_text(mytext)

	def rename(self,tab_name):
		self.tab_name=tab_name+"@"+self.tab_name.split("@")[1]
		inp_update_token_value(self.file_name, "#sim_menu_name", self.tab_name)



