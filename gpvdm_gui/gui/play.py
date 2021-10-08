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

## @package play
#  A play button
#

import os

#qt
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit

from icon_lib import icon_get
from str2bool import str2bool

from util import wrap_text

from server import server_get

from QAction_lock import QAction_lock

class play(QAction_lock):

	start_sim = pyqtSignal()
	stop_sim = pyqtSignal()

	def start(self):
		self.setIcon(icon_get("media-playback-pause"))
		self.setText(_("Stop\nsimulation"))
		self.running=True

	def stop(self):
		self.setIcon(icon_get(self.play_icon))
		self.setText(self.run_text)
		self.running=False

	def do_emit(self):
		value=False


		if self.running==False:
			self.start_sim.emit()
		else:
			server_get().killall()


	def __init__(self,parent,id,play_icon="media-playback-start",run_text=_("Run simulation"),connect_to_server=True):
		self.play_icon=play_icon
		self.run_text=run_text
		self.running=False
		QAction_lock.__init__(self,self.play_icon,self.run_text,parent,id)
		self.clicked.connect(self.do_emit)
		if connect_to_server==True:
			server_get().sim_started.connect(self.start)
			server_get().sim_finished.connect(self.stop)


