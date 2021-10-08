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

## @package circuit
#  Widget to draw circuit diagram
#

import os
from cal_path import get_image_file_path
from tb_pulse_load_type import tb_pulse_load_type

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar
from PyQt5.QtGui import QPainter,QIcon,QPixmap
from ersatzschaltbild import ersatzschaltbild

class my_draw(QWidget):
    def __init__(self, parent=None):
        super(QWidget, self).__init__(parent)

        self.mQImage = QPixmap("document-new.png")

    def paintEvent(self, QPaintEvent):
        painter = QPainter()
        painter.begin(self)
        painter.drawPixmap(100,10, self.mQImage)
        painter.end()


class circuit(QWidget):

	def update(self,object):
		self.darea.queue_draw()

	def __init__(self,data):
		QWidget.__init__(self)
		self.data=data
		vbox=QVBoxLayout()


		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))


		self.load_type=tb_pulse_load_type(data.config)

		toolbar.addWidget(self.load_type)
		vbox.addWidget(toolbar)

		self.darea = QWidget()


		self.diagram=ersatzschaltbild()
		self.diagram.dx=200
		self.diagram.dy=200
		self.diagram.editable=False

		vbox.addWidget(self.diagram)

		
		self.setLayout(vbox)
		self.load_type.changed.connect(self.update)
		self.update()

	def update(self):
		self.diagram.clear()
		if self.load_type.sim_mode.currentText()=="load":
			self.diagram.add_object0(1,1,1,2,"diode")

			self.diagram.add_object0(1,1,2,1,"wire")
			self.diagram.add_object0(1,2,2,2,"wire")

			self.diagram.add_object0(2,1,2,2,"capacitor")
			self.diagram.add_object0(2,1,3,1,"wire")
			self.diagram.add_object0(2,2,3,2,"wire")

			self.diagram.add_object0(3,1,3,2,"resistor")
			self.diagram.add_object0(3,1,4,1,"resistor")
			self.diagram.add_object0(3,2,4,2,"wire")

			self.diagram.add_object0(4,1,5,1,"resistor")
			self.diagram.add_object0(5,1,5,2,"vsource")

			self.diagram.add_object0(4,2,5,2,"wire")

		elif self.load_type.sim_mode.currentText()=="ideal_diode_ideal_load":

			self.diagram.add_object0(1,1,1,2,"diode")

			self.diagram.add_object0(1,1,2,1,"wire")
			self.diagram.add_object0(1,2,2,2,"wire")

			self.diagram.add_object0(2,1,3,1,"wire")
			self.diagram.add_object0(2,2,3,2,"wire")

			self.diagram.add_object0(3,1,3,2,"vsource")

		else:
			self.diagram.add_object0(1,1,1,2,"diode")

			self.diagram.add_object0(1,1,2,1,"wire")
			self.diagram.add_object0(1,2,2,2,"wire")

			self.diagram.add_object0(2,1,2,2,"capacitor")
			self.diagram.add_object0(2,1,3,1,"wire")
			self.diagram.add_object0(2,2,3,2,"wire")

			self.diagram.add_object0(3,1,3,2,"resistor")
			self.diagram.add_object0(3,1,4,1,"resistor")
			self.diagram.add_object0(3,2,4,2,"wire")

		self.diagram.repaint()

