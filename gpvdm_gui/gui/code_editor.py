# -*- coding: utf-8 -*-
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

## @package optics_ribbon
#  The ribbon for the optics window
#


import os

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon, QTextFormat,QTextOption
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice,QRect
from PyQt5.QtWidgets import QWidget,QSizePolicy, QPlainTextEdit,QVBoxLayout,QHBoxLayout, QPushButton,QDialog,QFileDialog,QToolBar, QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

from PyQt5.QtGui import QPainter,QColor
from icon_lib import icon_get

from util import wrap_text
from ribbon_base import ribbon_base
from play import play
from QAction_lock import QAction_lock
from inp import inp_get_token_value
from inp import inp_update_token_value
from cal_path import get_sim_path

from PyQt5.QtCore import QFile, QRegExp, Qt
from PyQt5.QtGui import QFont, QSyntaxHighlighter, QTextCharFormat

class QLineNumberArea(QWidget):
    def __init__(self, editor):
        super().__init__(editor)
        self.codeEditor = editor

    def sizeHint(self):
        return QSize(self.editor.lineNumberAreaWidth(), 0)

    def paintEvent(self, event):
        self.codeEditor.lineNumberAreaPaintEvent(event)


class code_editor(QPlainTextEdit):
	def __init__(self, parent=None):
		super().__init__(parent)
		self.setWordWrapMode(QTextOption.NoWrap)
		self.lineNumberArea = QLineNumberArea(self)
		self.blockCountChanged.connect(self.updateLineNumberAreaWidth)
		self.updateRequest.connect(self.updateLineNumberArea)
		self.cursorPositionChanged.connect(self.highlightCurrentLine)
		self.updateLineNumberAreaWidth(0)

	def lineNumberAreaWidth(self):
		digits = 1
		max_value = max(1, self.blockCount())
		while max_value >= 10:
		    max_value /= 10
		    digits += 1
		space = 3 + self.fontMetrics().width('9') * digits
		return space

	def updateLineNumberAreaWidth(self, _):
		self.setViewportMargins(self.lineNumberAreaWidth(), 0, 0, 0)

	def updateLineNumberArea(self, rect, dy):
		if dy:
		    self.lineNumberArea.scroll(0, dy)
		else:
		    self.lineNumberArea.update(0, rect.y(), self.lineNumberArea.width(), rect.height())
		if rect.contains(self.viewport().rect()):
		    self.updateLineNumberAreaWidth(0)

	def resizeEvent(self, event):
		super().resizeEvent(event)
		cr = self.contentsRect()
		self.lineNumberArea.setGeometry(QRect(cr.left(), cr.top(), self.lineNumberAreaWidth(), cr.height()))

	def highlightCurrentLine(self):
		extraSelections = []
		if not self.isReadOnly():
		    selection = QTextEdit.ExtraSelection()
		    lineColor = QColor(Qt.yellow).lighter(160)
		    selection.format.setBackground(lineColor)
		    selection.format.setProperty(QTextFormat.FullWidthSelection, True)
		    selection.cursor = self.textCursor()
		    selection.cursor.clearSelection()
		    extraSelections.append(selection)
		self.setExtraSelections(extraSelections)

	def lineNumberAreaPaintEvent(self, event):
		painter = QPainter(self.lineNumberArea)

		painter.fillRect(event.rect(), Qt.lightGray)

		block = self.firstVisibleBlock()
		blockNumber = block.blockNumber()
		top = self.blockBoundingGeometry(block).translated(self.contentOffset()).top()
		bottom = top + self.blockBoundingRect(block).height()

		# Just to make sure I use the right font
		height = self.fontMetrics().height()
		while block.isValid() and (top <= event.rect().bottom()):
		    if block.isVisible() and (bottom >= event.rect().top()):
		        number = str(blockNumber + 1)
		        painter.setPen(Qt.black)
		        painter.drawText(0, top, self.lineNumberArea.width(), height, Qt.AlignRight, number)

		    block = block.next()
		    top = bottom
		    bottom = top + self.blockBoundingRect(block).height()
		    blockNumber += 1

