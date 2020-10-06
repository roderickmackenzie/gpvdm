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

## @package dump_io
#  Toolbar icon to turn on and off dumping.
#

import os

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit

from icon_lib import icon_get
from str2bool import str2bool
from help import help_window
from inp import inp_update_token_value
from inp import inp_get_token_value

from cal_path import get_sim_path

dump_fast=0
dump_slow=1
dump_custom=2

class dump_io(QAction):
	def cal_state(self):
		val=False
		for token in self.tokens:
			v=inp_get_token_value(os.path.join(get_sim_path(),"dump.inp"), token )
			if v==None:
				return None
			val = val + int(str2bool(v))

		if val==len(self.tokens):
			return dump_slow
		
		if val==0:
			return dump_fast

		return dump_custom

	def set_state(self,val):
		for token in self.tokens:
			inp_update_token_value(os.path.join(get_sim_path(),"dump.inp"), token,str(val).lower())

		for token in self.const_tokens:
			inp_update_token_value(os.path.join(get_sim_path(),"dump.inp"), token[0],str(token[1]).lower())



	def update_ui(self,update_help):
		if self.dump==dump_slow:
			self.setIcon(icon_get("hdd_high"))
			if update_help==True:
				help_window().help_set_help(["hdd_high.png",_("<big><b>Write all data to disk (slow)</b></big><br>"+self.help_text)])

		if self.dump==dump_fast:
			self.setIcon(icon_get("hdd_low"))
			if update_help==True:
				help_window().help_set_help(["hdd_low.png",_("<big><b>Write minimal data to disk (fast)</b></big><br>"+self.help_text)])

		if self.dump==dump_custom:
			self.setIcon(icon_get("hdd_custom"))

	def refresh(self):
		self.dump=self.cal_state()
		#if self.dump==None:
		#	self.setEnabled(False)
		#	return
		#else:
		#	self.setEnabled(True)

		self.blockSignals(True)
		self.update_ui(False)
		self.blockSignals(False)

	def __init__(self,parent):
		self.dump=False
		self.tokens=["#newton_dump","#dump_dynamic","#dump_optics","#dump_optics_verbose", "#dump_1d_slices","#dump_energy_slice_switch","#dump_energy_slice_pos","#dump_pl","#dump_write_out_band_structure","#dump_equilibrium","#dump_optical_probe","#dump_optical_probe_spectrum","#dump_ray_trace_map","#dump_write_converge","#dump_first_guess","#dump_info_text"]
		self.tokens.append("#dump_workbook")
		self.const_tokens=[]
		self.const_tokens.append(["#dump_print_converge",True])
		self.const_tokens.append(["#dump_print_newtonerror",False])

		self.help_text=" Writing data to disk can be slow and gpvdm can produce a lot of data files during a simulation run.  To speed up the simulation use this icon to minimize simulation output.  Click it again to turn back on full output.  You can fine tune these parameters theough the Configure->Dump tab."
		QAction.__init__(self,icon_get("hdd_low"), _("Data dump\ncontrol"),parent)
		self.triggered.connect(self.callback_dump_io)

	def callback_dump_io(self):
		if self.dump==dump_slow:
			self.dump=dump_fast
		else:
			self.dump=dump_slow

		self.set_state(self.dump)
		self.update_ui(True)
