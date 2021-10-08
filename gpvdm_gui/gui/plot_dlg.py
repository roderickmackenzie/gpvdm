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

## @package plot_dlg
#  Plot window
#


import os

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QLineEdit,QComboBox,QHBoxLayout,QPushButton,QLabel,QDialog,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QApplication
from PyQt5.QtGui import QPainter,QIcon,QImage

from icon_lib import icon_get

from PyQt5.QtCore import QSize, Qt

from inp import inp_load_file

import functools

class plot_dlg_class(QDialog):

	def populate_combo_box_using_input_file(self,combobox,input_file):
#		try:
		lines=[]
		lines=inp_load_file(os.path.join(self.path,input_file))
		combobox.clear()
		if lines==False:
			return

		for i in range(0, len(lines)):
			lines[i]=lines[i].rstrip()
			if len(lines[i])>0:
				if lines[i][0]=="#":
					combobox.addItem(lines[i])
#		except:
#
#			combobox.clear()

	def callback_edit0(self):
		self.populate_combo_box_using_input_file(self.token0,self.file0.text())

	def callback_edit1(self):
		self.populate_combo_box_using_input_file(self.token1,self.file1.text())

	def callback_edit2(self):
		self.populate_combo_box_using_input_file(self.token2,self.file2.text())

	def callback_click_ok(self,data):
		data.file0=self.file0.text()
		data.tag0=str(self.token0.currentText())
		data.file1=self.file1.text()
		data.tag1=str(self.token1.currentText())
		data.file2=self.file2.text()
		data.tag2=str(self.token2.currentText())
		self.ret=True
		self.close()

	def callback_click_cancel(self):
		self.ret=False
		self.close()

	def __init__(self,data):
		QWidget.__init__(self)
		self.path=os.path.dirname(data.example_file0)
		self.setWindowIcon(icon_get("jv"))
		self.setWindowTitle(_("Plot token values (www.gpvdm.com)")) 
		self.setWindowFlags(Qt.WindowStaysOnTopHint)

		l=QLabel("x-axis:")
		vbox=QVBoxLayout()
		hbox=QHBoxLayout()
		self.file0 = QLineEdit()
		self.file0.setText(data.file0)
		self.file0.textChanged.connect(self.callback_edit0)
		hbox.addWidget(self.file0)


		self.token0 = QComboBox()
		self.populate_combo_box_using_input_file(self.token0, os.path.basename(data.example_file0))
		hbox.addWidget(self.token0)

		vbox.addWidget(l)
		hbox_widget=QWidget()
		hbox_widget.setLayout(hbox)
		vbox.addWidget(hbox_widget)

		#y-axis
		l=QLabel("y-axis:")
		l.show()

		hbox=QHBoxLayout()
		self.file1 = QLineEdit()
		self.file1.setText(data.file0)
		self.file1.textChanged.connect(self.callback_edit1)
		self.file1.show()
		hbox.addWidget(self.file1)

		self.token1 = QComboBox()
		self.populate_combo_box_using_input_file(self.token1,os.path.basename(data.example_file1))
		hbox.addWidget(self.token1)

		vbox.addWidget(l)
		hbox_widget=QWidget()
		hbox_widget.setLayout(hbox)
		vbox.addWidget(hbox_widget)

		#label
		l=QLabel("label:")

		hbox=QHBoxLayout()
		self.file2 = QLineEdit( )
		self.file2.setText(data.file0)
		self.file2.textChanged.connect(self.callback_edit2)
		hbox.addWidget(self.file2)

		self.token2 = QComboBox()
		self.populate_combo_box_using_input_file(self.token2,os.path.basename(data.example_file1))
		hbox.addWidget(self.token2)

		vbox.addWidget(l)
		hbox_widget=QWidget()
		hbox_widget.setLayout(hbox)
		vbox.addWidget(hbox_widget)

		button_box=QHBoxLayout()

		button_cancel=QPushButton("Cancel", self)
		button_box.addWidget(button_cancel)
		button_cancel.clicked.connect(self.callback_click_cancel)

		button_ok=QPushButton("Ok", self)
		button_ok.clicked.connect(functools.partial(self.callback_click_ok,data))
		button_box.addWidget(button_ok)

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)
		vbox.addWidget(button_box_widget)
		self.setLayout(vbox)
		
		
	def run(self):
		self.ret=False
		self.exec_()


