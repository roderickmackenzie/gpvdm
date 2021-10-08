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

## @package gl_input
#  The mouse and keyboard input to the OpenGL display.
#

import sys
open_gl_ok=False

import sys
from math import fabs
from PyQt5.QtWidgets import QApplication,QWidget,QVBoxLayout,QToolBar,QAction
from PyQt5.QtGui import QCursor
from PyQt5.QtCore import QSize

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	from gl_lib import val_to_rgb
	from gl_color import color
	from gl_lim import gl_obj_id_starts_with

except:
	pass

from PyQt5.QtCore import QTimer
from PyQt5.QtCore import Qt
from PyQt5 import QtGui
import time
from icon_lib import icon_get
from gl_video_maker import gl_video_maker

class gl_toolbar():

	def __init__(self):
		#toolbar0
		self.toolbar0=QWidget()
		self.box0=QVBoxLayout()
		self.box0.setSpacing(0)
		self.box0.setContentsMargins(0, 0, 0, 0)
		self.toolbar0.setLayout(self.box0)
		self.box_tb0=QToolBar()
		self.box_tb0.setIconSize(QSize(32, 32))
		self.box0.addWidget(self.box_tb0)
		self.box_tb1=QToolBar()
		self.box_tb1.setIconSize(QSize(32, 32))
		self.box0.addWidget(self.box_tb1)

		self.xy = QAction(icon_get("xy"), _("xy"), self)
		self.box_tb0.addAction(self.xy)
		self.xy.triggered.connect(self.view_move_to_xy)

		self.yz = QAction(icon_get("yz"), _("yz"), self)
		self.box_tb0.addAction(self.yz)
		self.yz.triggered.connect(self.view_move_to_yz)

		self.xz = QAction(icon_get("xz"), _("xz"), self)
		self.box_tb1.addAction(self.xz)
		self.xz.triggered.connect(self.view_move_to_xz)

		self.tb_orthographic = QAction(icon_get("orthographic"), _("Orthographic"), self)
		self.box_tb1.addAction(self.tb_orthographic)
		self.tb_orthographic.triggered.connect(self.view_move_to_orthographic)


		#toolbar1
		self.toolbar1=QWidget()
		self.box1=QVBoxLayout()
		self.box1.setSpacing(0)
		self.box1.setContentsMargins(0, 0, 0, 0)
		self.toolbar1.setLayout(self.box1)
		self.box_tb2=QToolBar()
		self.box_tb2.setIconSize(QSize(32, 32))
		self.box1.addWidget(self.box_tb2)
		self.box_tb3=QToolBar()
		self.box_tb3.setIconSize(QSize(32, 32))
		self.box1.addWidget(self.box_tb3)


		self.tb_video = QAction(icon_get("fly"), _("Save\nvideo"), self)
		self.box_tb2.addAction(self.tb_video)
		self.tb_video.triggered.connect(self.callback_videomaker)

		self.tb_rotate = QAction(icon_get("rotate.png"), _("Rotate"), self)
		self.box_tb2.addAction(self.tb_rotate)
		self.tb_rotate.setEnabled(True)
		self.tb_rotate.triggered.connect(self.start_rotate)


	def callback_videomaker(self):
		self.video_maker_window=gl_video_maker(self)
		self.video_maker_window.show()

