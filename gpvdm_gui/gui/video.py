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
import os

from PyQt5.QtCore import QDir, Qt, QUrl
from PyQt5.QtMultimedia import QMediaContent, QMediaPlayer
from PyQt5.QtMultimediaWidgets import QVideoWidget
from PyQt5.QtWidgets import QHBoxLayout, QLabel, QPushButton, QSizePolicy, QSlider, QStyle, QVBoxLayout, QWidget,QDesktopWidget
from PyQt5.QtWidgets import QWidget, QPushButton, QAction
from PyQt5.QtGui import QIcon
import sys
from cal_path import get_video_path

class video(QWidget):

	def __init__(self):
		QWidget.__init__(self)
		self.setMinimumSize(800,520)
		centerPoint = QDesktopWidget().availableGeometry().center()
		qtRectangle = self.frameGeometry()
		qtRectangle.moveCenter(centerPoint)
		self.move(qtRectangle.topLeft())
		self.setWindowFlags(Qt.FramelessWindowHint|Qt.WindowStaysOnTopHint)
		self.setWindowTitle("Welcome") 


		self.mediaPlayer = QMediaPlayer(None, QMediaPlayer.VideoSurface)
		videoWidget = QVideoWidget()

		self.playButton = QPushButton()
		self.playButton.setEnabled(False)
		self.playButton.setIcon(self.style().standardIcon(QStyle.SP_MediaPlay))
		self.playButton.clicked.connect(self.play)

		self.positionSlider = QSlider(Qt.Horizontal)
		self.positionSlider.setRange(0, 0)
		self.positionSlider.sliderMoved.connect(self.setPosition)


		# Create a widget for window contents
		wid = QWidget(self)

		# Create layouts to place inside widget
		controlLayout = QHBoxLayout()
		controlLayout.setContentsMargins(0, 0, 0, 0)
		controlLayout.addWidget(self.playButton)
		controlLayout.addWidget(self.positionSlider)

		layout = QVBoxLayout()
		layout.addWidget(videoWidget)
		layout.addLayout(controlLayout)

		# Set widget to contain window contents
		wid.setLayout(layout)

		self.mediaPlayer.setVideoOutput(videoWidget)
		self.mediaPlayer.stateChanged.connect(self.mediaStateChanged)
		self.mediaPlayer.positionChanged.connect(self.positionChanged)
		self.mediaPlayer.durationChanged.connect(self.durationChanged)
		self.mediaPlayer.error.connect(self.handleError)

		self.openFile()
		self.mediaPlayer.play()

		self.setLayout(layout)


	def openFile(self):
		fileName=os.path.join(get_video_path(),"welcome.wmv")
		print(QUrl.fromLocalFile(fileName))
		self.mediaPlayer.setMedia(QMediaContent(QUrl.fromLocalFile(fileName)))
		self.playButton.setEnabled(True)


	def play(self):
		if self.mediaPlayer.state() == QMediaPlayer.PlayingState:
		    self.mediaPlayer.pause()
		else:
		    self.mediaPlayer.play()

	def mediaStateChanged(self, state):
		if self.mediaPlayer.state()==0:
			self.close()
		if self.mediaPlayer.state() == QMediaPlayer.PlayingState:
		    self.playButton.setIcon(
		            self.style().standardIcon(QStyle.SP_MediaPause))
		else:
		    self.playButton.setIcon(
		            self.style().standardIcon(QStyle.SP_MediaPlay))

	def positionChanged(self, position):
		self.positionSlider.setValue(position)

	def durationChanged(self, duration):
		self.positionSlider.setRange(0, duration)

	def setPosition(self, position):
		self.mediaPlayer.setPosition(position)

	def handleError(self):
		self.playButton.setEnabled(False)
		print("Error: " + self.mediaPlayer.errorString())

