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

## @package QHTabBar
#  A horizontal toolbar tab because QT does not have one.
#

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon, QPainter, QFont, QColor
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QLabel,QComboBox, QTabWidget,QStatusBar,QMenuBar, QTabBar, QStylePainter, QStyleOptionTab,QStyle

class QHTabBar(QTabBar):
	def __init__(self, *args, **kwargs):
		QTabBar.__init__(self)
		self.setStyleSheet("QTabBar::tab { height: 35px; width: 140px; }")

	def paintEvent(self, event):
		painter = QStylePainter(self)
		option = QStyleOptionTab()

		#painter.begin(self)
		for index in range(self.count()):
			self.initStyleOption(option, index)
			tabRect = self.tabRect(index)
			tabRect.moveLeft(10)
			painter.drawControl(QStyle.CE_TabBarTabShape, option)
			painter.drawText(tabRect, Qt.AlignVCenter | Qt.TextDontClip, self.tabText(index))
		#painter.end()


