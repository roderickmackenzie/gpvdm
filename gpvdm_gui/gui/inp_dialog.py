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

## @package tab
#  The main tab class, used to display material properties.
#

import os

from tab_base import tab_base
from inp import inp_update_token_value
from inp import inp_get_token_value
from util import latex_to_html

from PyQt5.QtCore import pyqtSignal

from PyQt5.QtWidgets import QTextEdit,QWidget, QScrollArea,QVBoxLayout,QLabel,QHBoxLayout,QPushButton, QSizePolicy, QDialog
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QPixmap, QIcon

from icon_lib import icon_get

from PyQt5.QtCore import QTimer

import i18n
_ = i18n.language.gettext

import functools
from error_dlg import error_dlg

from file_watch import get_watch
from inp_viewer import inp_viewer
from inp import inp
from cal_path import get_image_file_path

class inp_dialog(QDialog):

	def __init__(self,title="gpvdm",icon="icon"):
		QWidget.__init__(self)
		self.editable=True
		self.setWindowTitle(title)
		self.setWindowIcon(icon_get(icon))
		#self.setWindowIcon(QIcon(os.path.join(get_image_file_path(),"image.jpg")))		

		self.scroll=QScrollArea()
		self.main_box_widget=QWidget()
		self.vbox=QVBoxLayout()
		self.hbox=QHBoxLayout()
		self.hbox.setAlignment(Qt.AlignTop)

		self.tab=inp_viewer()
		self.tab.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.vbox.addWidget(self.tab)

		#spacer = QWidget()
		#spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		#self.hbox.addWidget(spacer)

		self.vbox.addWidget(self.gen_button_box_widget())
		
		#self.main_box_widget.setLayout(self.vbox)
		
		#self.scroll.setWidget(self.main_box_widget)

		
		#self.hbox.addWidget(self.scroll)

		self.setMinimumWidth(400)

		self.setLayout(self.vbox)


	def update():
		self.f.load(self.filename)
		self.tab.update_lines(self,self.f.lines)

	def set_edit(self,editable):
		self.tab.editable=editable

	def callback_close(self):
		self.reject()

	def callback_apply(self):
		self.accept()

	def run(self,lines):
		self.tab.populate(lines)
		return self.exec_()

	def gen_button_box_widget(self):
		button_box=QHBoxLayout()

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		button_box.addWidget(spacer)

		self.close_button=QPushButton("Close", self)
		self.close_button.clicked.connect(self.callback_close)
		button_box.addWidget(self.close_button)

		self.apply=QPushButton("Apply", self)
		self.apply.clicked.connect(self.callback_apply)
		button_box.addWidget(self.apply)

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)
		return button_box_widget
