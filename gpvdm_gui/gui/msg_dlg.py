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

## @package msg_box
#  A message box class.
#

#qt
from PyQt5.QtWidgets import QDialog,QVBoxLayout,QLabel,QHBoxLayout,QWidget,QSizePolicy,QPushButton
from gui_enable import gui_get
from icon_lib import icon_get
from PyQt5.QtGui import QFont,QPixmap


class msg_dlg(QDialog):

	def __init__(self,title="gpvdm.com",image_file=""):
		QDialog.__init__(self)
		self.setWindowIcon(icon_get("icon"))
		self.setWindowTitle(title)

		vbox=QVBoxLayout()

		main_hbox=QHBoxLayout()

		if image_file!="":
			self.image=QLabel()
			#self.text.setFont(QFont('SansSerif', 16))
			self.image.setPixmap(QPixmap(image_file))
			main_hbox.addWidget(self.image)

		self.text=QLabel()
		self.text.setFont(QFont('SansSerif', 16))
		main_hbox.addWidget(self.text)

		main_hbox_widget=QWidget()
		main_hbox_widget.setLayout(main_hbox)
		vbox.addWidget(main_hbox_widget)

		button_box=QHBoxLayout()

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		button_box.addWidget(spacer)

		self.ok=QPushButton("Ok", self)
		self.ok.clicked.connect(self.callback_ok)
		button_box.addWidget(self.ok)

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)
		vbox.addWidget(button_box_widget)
		self.text.setWordWrap(True)
		self.setLayout(vbox)
		self.setMinimumWidth(500)

	def setText(self,data):
		self.text.setText(data)


	def callback_ok(self):

		self.accept()

