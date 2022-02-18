# -*- coding: utf-8 -*-
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

## @package ribbon_sim_mode
#  The sim mode ribbon.
#


import os
import sys
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QTextEdit, QAction
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QToolButton,QDialog

from win_lin import desktop_open

#windows
from help import help_window
from error_dlg import error_dlg
from server import server_get

from global_objects import global_object_run
from icon_lib import icon_get

from cal_path import get_sim_path
from play import play
from util import wrap_text

import webbrowser
from PyQt5.QtCore import pyqtSignal

from global_objects import global_object_register

from lock import get_lock

from help import help_window
from QAction_lock import QAction_lock
from str2bool import str2bool
from lock import get_lock
from gpvdm_json import gpvdm_data
from ribbon_page import ribbon_page

class gQAction(QAction_lock):
	selected=pyqtSignal(QAction_lock)

	def __init__(self,s,file_name,command,module):
		self.icon=False
		self.text=command
		self.command=command
		self.module=module
		self.done=False

		data=file_name
		self.icon_name=data.icon
		self.text=data.english_name.replace("\\n","\n")

		QAction_lock.__init__(self,self.icon_name, self.text, s,self.module)
		self.setCheckable( True)
		self.clicked.connect(self.callback_click)

	def callback_click(self):
		data=gpvdm_data()
		data.sim.simmode=self.command+"@"+self.module
		data.save()
		self.selected.emit(self)

	def get_simmode(self):
		a=self.command+"@"+self.module
		return a.lower()

class ribbon_sim_mode(ribbon_page):

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
		data=gpvdm_data()
		self.clear_toolbar()

		self.blockSignals(True)

		sims=[]
		sims.append([data.jv.segments, "jv","segment"])
		sims.append([data.suns_voc.segments, "suns_voc","segment"])
		sims.append([data.suns_jsc.segments, "suns_jsc","segment"])
		sims.append([data.time_domain.segments, "time_domain","segment"])

		sims.append([data.fx_domain.segments, "fx_domain","segment"])
		if get_lock().is_gpvdm_next()==True:
			sims.append([data.cv.segments, "cv","segment"])
		sims.append([data.ce.segments, "ce","segment"])
		sims.append([data.pl_ss.segments, "pl_ss","segment"])
		sims.append([data.transfer_matrix.segments, "optics","segment"])
		sims.append([data.eqe.segments, "eqe","segment"])
		sims.append([data.fdtd.segments, "fdtd","segment"])
		sims.append([data.ray.segments, "trace","segment"])
		sims.append([data.spm.segments, "spm","segment"])
		sims.append([data.exciton.segments, "exciton","segment"])
		#print(data.pl.simulations)
		for sim in sims:
			i=0
			for sub_sim in sim[0]:
				add=True
				#print(sim.english_name,sim.icon)
				if sim[1]=="fx_domain":
					if sub_sim.config.fx_modulation_type=="optical":
						add=False
					if sub_sim.english_name=="test":
						add=False
					if sub_sim.english_name=="IMVS":
						add=False
					if sub_sim.english_name=="IMPS":
						add=False

				if add==True:
					a = gQAction(self, sub_sim,sim[2]+str(i), sim[1])
					a.selected.connect(self.callback_click)
					self.actions.append(a)
				i=i+1

			self.actions.append("|")

		lines=[]


		#temp.sort()
		#print(self.actions)
		simmode=data.sim.simmode.lower()

		found=False
		last=None
		for a in self.actions:
			#self.sim_mode.addItem(command)
			if type(a)==gQAction:
				self.addAction(a)
				if a.get_simmode()==simmode:
					self.callback_click(a,disable_help=True)
					found=True
			else:
				if last != "|":
					self.addSeparator()
			last=a

		#if there is no known mode, just set it to jv mode
		if found==False:
			for a in self.actions:
				if type(a)==gQAction:
					if a.module=="jv":
						self.callback_click(a,disable_help=True)
						data=gpvdm_data()
						data.sim.simmode=a.command+"@"+a.module
						data.save()
						break

			self.blockSignals(False)

	def clear_toolbar(self):
		#for a in self.actions:
		#	if type(a)==gQAction:
		#		self.removeAction(a)
		#	if type(a)==str:
		#		self.removeSeperator(a)
		self.clear()
		self.actions=[]

	def __init__(self):
		ribbon_page.__init__(self)
		self.actions=[]
		self.dont_show=["photon_extraction"]
		self.myserver=server_get()

		global_object_register("ribbon_sim_mode_update",self.update)

	def setEnabled(self,val):
		self.undo.setEnabled(val)
		self.run.setEnabled(val)
		#self.stop.setEnabled(val)
		self.scan.setEnabled(val)
		self.plot.setEnabled(val)
		self.sun.setEnabled(val)
		self.help.setEnabled(val)


