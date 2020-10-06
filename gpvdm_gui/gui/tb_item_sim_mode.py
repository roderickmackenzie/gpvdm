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

## @package tb_item_sim_mode
#  Toolbar item to select the simulation mode.
#


import os

#inp
from inp import inp_update_token_value
from inp import inp_get_token_value
from inp import inp_load_file
from inp import inp_lsdir
from inp import inp_search_token_value

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QLabel,QComboBox

from cal_path import get_sim_path

import i18n
_ = i18n.language.gettext

class store:
	def __init__(self,token,file):
		self.token=token
		self.file=file

class tb_item_sim_mode(QWidget):

	def update(self):
		self.sim_mode.clear()
		
		lines=[]
		self.store_list=[]
		temp=[]
		files=inp_lsdir("sim.gpvdm")
		if files!=False:
			for i in range(0,len(files)):
				if files[i].endswith(".inp") and files[i].count("/")==0:
					lines=inp_load_file(files[i])
					value=inp_search_token_value(lines, "#sim_menu_name")
					if value!=False:
						if value.count("@")==1:
							temp.append(value)

			temp.sort()

			for i in range(0,len(temp)):
				value=temp[i].rstrip()
				command,module=value.split("@")
				self.sim_mode.addItem(command)
				a=store(command,module)
				self.store_list.append(a)
			print(os.getcwd())
			token=inp_get_token_value("sim.inp", "#simmode")
			print(token)
			if token.count("@")!=0:
				command,module=token.split("@")
			else:
				command=token

			found=False

			all_items  = [self.sim_mode.itemText(i) for i in range(self.sim_mode.count())]
			for i in range(0,len(all_items)):
				if all_items[i] == command:
					self.sim_mode.setCurrentIndex(i)
					found=True

			#if there is no known mode, just set it to jv mode
			if found==False:
				for i in range(0,len(self.store_list)):
					if self.store_list[i].token=="jv":
						self.sim_mode.setCurrentIndex(i)
						inp_update_token_value(os.path.join(get_sim_path(),"sim.inp"), "#simmode", "jv@jv")
						break

	def __init__(self):
		QWidget.__init__(self)
		layout=QVBoxLayout()
		label=QLabel()
		label.setText(_("Simulation mode:"))
		layout.addWidget(label)

		self.sim_mode = QComboBox(self)
		self.sim_mode.setEditable(True)
		self.update()

		self.sim_mode.currentIndexChanged.connect(self.call_back_sim_mode_changed)

		layout.addWidget(self.sim_mode)

		self.setLayout(layout)

		return




	def call_back_sim_mode_changed(self):
		mode=self.sim_mode.currentText()
		for i in range(0,len(self.store_list)):
			if self.store_list[i].token==mode:
				inp_update_token_value(os.path.join(get_sim_path(),"sim.inp"), "#simmode", mode+"@"+self.store_list[i].file)


