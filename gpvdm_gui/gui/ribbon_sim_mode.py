# -*- coding: utf-8 -*-
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

## @package ribbon_sim_mode
#  The sim mode ribbon.
#


import os
import sys
from dump_io import dump_io
from tb_item_sim_mode import tb_item_sim_mode
from tb_item_sun import tb_item_sun

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QTextEdit, QAction
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QToolButton,QDialog

from info import sim_info
from win_lin import desktop_open

#windows
from scan import scan_class
from help import help_window
from gpvdm_open import gpvdm_open
from error_dlg import error_dlg
from server import server_get
from fit_window import fit_window

from global_objects import global_object_run
from util import isfiletype
from icon_lib import icon_get

from cal_path import get_sim_path
from inp import inp
from play import play
from util import wrap_text

import webbrowser
from inp import inp_lsdir
from inp import inp_load_file
from inp import inp_search_token_value
from inp import inp_get_token_value
from PyQt5.QtCore import pyqtSignal
from inp import inp_get_token_value
from inp import inp_update_token_value

from global_objects import global_object_register

from lock import get_lock

from help import help_window
from QAction_lock import QAction_lock
from str2bool import str2bool
from newton_solver import newton_solver_get_type

class gQAction(QAction_lock):
	selected=pyqtSignal(QAction_lock)

	def __init__(self,s,file_name,command,module):
		self.icon=False
		self.text=command
		self.icon_name=inp_get_token_value(file_name, "#icon")
		self.done=False

		if self.icon_name==None:
			self.icon_name="plot"

		self.english_name=inp_get_token_value(file_name, "#english_name")
		if self.english_name!=None:
			self.text=self.english_name.replace("\\n","\n")
		self.command=command
		self.module=module


		QAction_lock.__init__(self,self.icon_name, self.text, s,self.module)
		self.clicked.connect(self.callback_click)

	def callback_click(self):
		inp_update_token_value(os.path.join(get_sim_path(),"sim.inp"), "#simmode", self.command+"@"+self.module)
		self.selected.emit(self)

class ribbon_sim_mode(QToolBar):

	def callback_click(self,w,disable_help=False):
		self.blockSignals(True)
		for a in self.actions:
			if type(a)==gQAction:
				a.setChecked(False)

		w.setChecked(True)
		if disable_help==False:
			if w.icon!=False and w.text!=None:
				help_window().help_set_help([w.icon_name+".png",_("<big><b>Simulation mode changed</b></big><br>"+w.english_name.replace("\\n"," "))])

		self.blockSignals(False)

	def update(self):
		self.clear_toolbar()
		lines=[]
		newton_solver=newton_solver_get_type()
		files=inp_lsdir("sim.gpvdm")
		if files!=False:
			for i in range(0,len(files)):
				if files[i].endswith(".inp") and files[i].count("/")==0:
					lines=inp_load_file(files[i])
					value=inp_search_token_value(lines, "#sim_menu_name")
					if value!=False:
						if value.count("@")==1:
							value=value.rstrip()
							command,module=value.split("@")

							hide=False
							if newton_solver=="none":
								if module=="jv":
									hide=True

								if module=="pulse":
									hide=True

								if module=="suns_jsc":
									hide=True

								if module=="sun_voc":
									hide=True

								if module=="is":
									hide=True

							if command not in self.dont_show and hide==False:
								a = gQAction(self, files[i],command, module)
								a.setCheckable( True);
								a.selected.connect(self.callback_click)
								self.actions.append(a)

							#temp.append(value)

			#temp.sort()

			token=inp_get_token_value("sim.inp", "#simmode")
			if token==None:
				return
				#print(os.getcwd(),"!!!!!!!!!!!!!")
				#sys.exit(0)
			if token.count("@")!=0:
				command,module=token.split("@")
			else:
				command=token

			found=False
			self.build=[]
			added=0			#This is to stop || appearing in the menu
			for o in self.order:
				if o=="|":
					if added!=0:
						self.build.append("|")
					added=0
				else:
					for i in range(0,len(self.actions)):
						if self.actions[i].command.lower().startswith(o.lower())==True:
							if self.actions[i].done==False:
								self.build.append(self.actions[i])
								self.actions[i].done=True
								added=added+1


			for i in range(0,len(self.actions)):
				if self.actions[i].done==False:
					#print(self.actions[i].command)
					self.build.append(self.actions[i])

			self.actions=self.build

			self.blockSignals(True)
			for a in self.actions:
				#self.sim_mode.addItem(command)
				if type(a)==gQAction:
					self.addAction(a)
					if command.lower()==a.command.lower():
						self.callback_click(a,disable_help=True)
						found=True
				else:
					self.addSeparator()



			#if there is no known mode, just set it to jv mode
			if found==False:
				for a in self.actions:
					if a.command=="jv":
						self.callback_click(a,disable_help=True)
						inp_update_token_value(os.path.join(get_sim_path(),"sim.inp"), "#simmode", "jv@jv")
						break

			self.blockSignals(False)

	def callback_buy(self):
		webbrowser.open("https://www.gpvdm.com/buy.html")

	def clear_toolbar(self):
		#for a in self.actions:
		#	if type(a)==gQAction:
		#		self.removeAction(a)
		#	if type(a)==str:
		#		self.removeSeperator(a)
		self.clear()
		self.actions=[]

	def __init__(self):
		QToolBar.__init__(self)
		self.actions=[]
		self.order=["jv","jv_perovskite","|","sun_voc","suns_jsc","equilibrium","|","TPC","TPV","|","IS", "IMPS","|", "CELIV","Photo"]
		self.dont_show=["photon_extraction"]
		self.myserver=server_get()

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))
		global_object_register("ribbon_sim_mode_update",self.update)

	def setEnabled(self,val):
		self.undo.setEnabled(val)
		self.run.setEnabled(val)
		#self.stop.setEnabled(val)
		self.scan.setEnabled(val)
		self.plot.setEnabled(val)
		self.sun.setEnabled(val)
		self.help.setEnabled(val)


