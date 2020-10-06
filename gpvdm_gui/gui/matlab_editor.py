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

## @package matlab_editor
#  An editor for running octave/matlab scripts
#

import os

from scan_select import select_param
from token_lib import tokens

from scan_io import scan_push_to_hpc
from scan_io import scan_import_from_hpc
from cal_path import get_exe_command
from icon_lib import icon_get
from str2bool import str2bool

from inp import inp_get_token_value

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QTextEdit,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon

from gpvdm_select import gpvdm_select

from scan_select import select_param

from cal_path import get_sim_path

class matlab_editor(QWidget):

	def load(self):
		if os.path.isfile(self.file_name):
			f = open(self.file_name)
			lines = f.read()
			f.close()
			self.text.setText(lines)

	def callback_edited(self):
		lines=[]
		a = open(self.file_name, "w")
		a.write(self.text.toPlainText())#.split("\n")
		a.close()

	def callback_run(self):
		octave=inp_get_token_value(os.path.join(get_sim_path(),"config.inp"), "#matlab_interpreter")
		command=octave+" "+self.file_name+" "+self.exp_file_name+" "+self.fit_target
		print(command)
		old_path=os.getcwd()
		os.chdir(get_sim_path())
		os.system(command)
		os.chdir(old_path)

	def __init__(self,index):
		QWidget.__init__(self)

		self.index=index
		self.file_name=os.path.join(get_sim_path(),"fit_math"+str(self.index)+".inp")
		self.exp_file_name=os.path.join(get_sim_path(),"fit_data"+str(self.index)+".inp")
		self.fit_target=os.path.join(get_sim_path(),"fit_target"+str(self.index)+".inp")
		
		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		self.tb_run = QAction(icon_get("media-playback-start"), _("Run"), self)
		self.tb_run.triggered.connect(self.callback_run)
		toolbar.addAction(self.tb_run)

		self.vbox.addWidget(toolbar)

		self.text = QTextEdit()
		self.text.setFontPointSize(24)
		self.load()
		self.text.textChanged.connect(self.callback_edited)

		self.vbox.addWidget(self.text)


		self.setLayout(self.vbox)
