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

## @package tab
#  The main tab class, used to display material properties.
#

from tab_base import tab_base

from PyQt5.QtCore import pyqtSignal

from PyQt5.QtWidgets import QTextEdit,QWidget, QScrollArea,QVBoxLayout,QLabel,QHBoxLayout,QPushButton, QSizePolicy, QDialog
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QPixmap, QIcon
from icon_lib import icon_get
from json_viewer import json_viewer

import i18n
_ = i18n.language.gettext



class json_dialog(QDialog):

	def __init__(self,title="gpvdm",icon="icon"):
		QWidget.__init__(self)
		self.editable=True
		self.setWindowTitle(title)
		self.setWindowIcon(icon_get(icon))

		self.scroll=QScrollArea()
		self.main_box_widget=QWidget()
		self.vbox=QVBoxLayout()
		self.hbox=QHBoxLayout()
		self.hbox.setAlignment(Qt.AlignTop)

		self.tab=json_viewer()
		self.tab.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.vbox.addWidget(self.tab)

		self.vbox.addWidget(self.gen_button_box_widget())

		self.setMinimumWidth(400)

		self.setLayout(self.vbox)

	def set_edit(self,editable):
		self.tab.editable=editable

	def callback_close(self):
		self.reject()

	def callback_apply(self):
		self.accept()

	def run(self,data):
		self.tab.populate(data)
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
