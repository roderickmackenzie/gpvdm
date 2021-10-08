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

## @package splash
#  The splash screen.
#

import os
import glob
from const_ver import const_ver
from notice import notice
import random
from random import randint
import time

from PyQt5.QtGui import QIcon,QTransform
from PyQt5.QtCore import QSize, Qt, QTimer, QBasicTimer
from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QApplication,QGraphicsScene,QWidget,QGraphicsView,QLabel,QVBoxLayout,QProgressBar
from PyQt5.QtGui import QPixmap,QFont

#cal_path
from cal_path import get_image_file_path
from cal_path import get_ui_path
import time

class splash_window(QWidget):

	def center(self):
		frameGm = self.frameGeometry()
		screen = QApplication.desktop().screenNumber(QApplication.desktop().cursor().pos())
		centerPoint = QApplication.desktop().screenGeometry(screen).center()
		frameGm.moveCenter(centerPoint)
		self.move(frameGm.topLeft())

	def callback_destroy(self):
		self.close()

	def __init__(self):
		QWidget.__init__(self)
		mul=1.1
		self.counts=0
		width=459*mul
		self.setFixedSize(width, 260)
		self.center()

		self.view=QGraphicsView(self)

		self.view.setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }")


		self.view.setFixedSize(width+2+2,261)
		self.view.move(-2,0)
		self.setWindowFlags(Qt.FramelessWindowHint|Qt.WindowStaysOnTopHint)
		self.li=QLabel("ver",self)
		self.li.move(30,170)

		self.font = QFont()
		self.font.setFamily('DejaVu Sans')
		self.font.setBold(True)
		self.font.setStyleHint(QFont.Monospace)
		self.font.setFixedPitch(True)
		self.font.setPointSize(int(72))

		self.terminals=[]
		self.process=[]
	
		self.gpvdm=QLabel("gpvdm",self)
		self.gpvdm.setFont(self.font)
		self.gpvdm.move(80,40)


		self.li.setText(notice()+"\n"+const_ver())
		self.view.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
		self.view.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
		window_h=self.height()
		window_w=self.width()

		QTimer.singleShot(2000, self.callback_destroy)

		files=glob.glob(os.path.join(get_image_file_path(),"splash","*.jpg"))
		files.extend(glob.glob(os.path.join(get_image_file_path(),"splash","*.png")))

		number=time.localtime().tm_yday

		number=number % len(files)

		if number>=len(files[number]):
			number=0

		image_file=files[number]

		image_path=os.path.join(get_image_file_path(),"splash",image_file)
		if os.path.isfile(image_path):
			image=QPixmap(image_path)
		
			image.scaledToHeight(window_h)

			w=image.width()
			h=image.height()
			x_max=w-window_h-window_w/2

			hour=float(time.strftime("%H"))*60
			m=float(time.strftime("%m"))
			tot=hour+m
			my_max=float(24*60)

			value=tot/my_max

			xpos=int(x_max*value)+window_w/2

			scene=QGraphicsScene();
			scene.setSceneRect(xpos, 0, 0, h)
			self.view.setScene(scene)


			scene.addPixmap(image);
		else:
			print("Image not found",image_path)

		self.pbar = QProgressBar(self)
		self.pbar.setGeometry(0, 261-20, width-5, 15)
		#self.pbar.move(50,150)
		


		self.show()

	def inc_value(self):
		#print(self.counts)
		self.counts=self.counts+1
		value=100.0*self.counts/29.0
		self.pbar.setValue(value)
		if value>=100.0:
			self.pbar.hide()
		QApplication.processEvents()

	def set_value(self,value):
		self.pbar.setValue(value)
		QApplication.processEvents()
		

		    




