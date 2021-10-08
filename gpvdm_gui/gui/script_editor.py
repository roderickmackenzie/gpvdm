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

## @package script_editor
#  A script editor widget
#


import os
import sys
import importlib

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication, QShortcut
from PyQt5.QtGui import QIcon, QTextFormat,QTextOption, QKeySequence
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice,QRect
from PyQt5.QtWidgets import QWidget,QSizePolicy, QPlainTextEdit,QVBoxLayout,QHBoxLayout, QPushButton,QDialog,QFileDialog,QToolBar, QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget
import sys

from PyQt5.QtGui import QPainter,QColor
from icon_lib import icon_get

from PyQt5.QtCore import QFile, QRegExp, Qt
from PyQt5.QtGui import QFont, QSyntaxHighlighter, QTextCharFormat
from code_editor import code_editor

from inp import inp_load_file
from gpvdm_api import gpvdm_api
import imp
from PyQt5.QtCore import pyqtSignal

class Highlighter(QSyntaxHighlighter):

	def __init__(self, parent=None):
		super(Highlighter, self).__init__(parent)

		self.highlightingRules = []

		keyword = QTextCharFormat()
		keyword.setForeground( Qt.darkRed )
		keyword.setFontWeight( QFont.Bold )
		keywords = [ "break", "else", "for", "if", "in"
					 "next", "repeat", "return", "switch",
					 "try", "while","self" ] 
		for word in keywords:
			pattern = QRegExp("\\b" + word + "\\b")
			self.highlightingRules.append( (pattern, keyword) )


		keywords = [ "class","def"]

		for word in keywords:
			pattern = QRegExp(word + "\\b")
			self.highlightingRules.append( (pattern, keyword) )


		keywords = [ "import", "from" ] 
		classFormat = QTextCharFormat()
		classFormat.setForeground(Qt.red)
		for k in keywords:
			self.highlightingRules.append((QRegExp(k+"\\b"),classFormat))


		classFormat = QTextCharFormat()
		classFormat.setFontWeight(QFont.Bold)
		classFormat.setForeground(Qt.darkMagenta)
		self.highlightingRules.append((QRegExp("\\bQ[A-Za-z]+\\b"),classFormat))

		singleLineCommentFormat = QTextCharFormat()
		singleLineCommentFormat.setForeground(Qt.darkBlue)
		self.highlightingRules.append((QRegExp("#[^\n]*"),singleLineCommentFormat))

		self.multiLineCommentFormat = QTextCharFormat()
		self.multiLineCommentFormat.setForeground(Qt.red)

		quotationFormat = QTextCharFormat()
		quotationFormat.setForeground(Qt.darkMagenta)
		self.highlightingRules.append((QRegExp("\".*\""), quotationFormat))

		functionFormat = QTextCharFormat()
		functionFormat.setFontItalic(True)
		functionFormat.setForeground(Qt.blue)
		self.highlightingRules.append((QRegExp("\\b[A-Za-z0-9_]+(?=\\()"),functionFormat))

		self.commentStartExpression = QRegExp("/\\*")
		self.commentEndExpression = QRegExp("\\*/")

	def highlightBlock(self, text):
		for pattern, format in self.highlightingRules:
		    expression = QRegExp(pattern)
		    index = expression.indexIn(text)
		    while index >= 0:
		        length = expression.matchedLength()
		        self.setFormat(index, length, format)
		        index = expression.indexIn(text, index + length)

		self.setCurrentBlockState(0)

		startIndex = 0
		if self.previousBlockState() != 1:
		    startIndex = self.commentStartExpression.indexIn(text)

		while startIndex >= 0:
		    endIndex = self.commentEndExpression.indexIn(text, startIndex)

		    if endIndex == -1:
		        self.setCurrentBlockState(1)
		        commentLength = len(text) - startIndex
		    else:
		        commentLength = endIndex - startIndex + self.commentEndExpression.matchedLength()

		    self.setFormat(startIndex, commentLength,
		            self.multiLineCommentFormat)
		    startIndex = self.commentStartExpression.indexIn(text,
		            startIndex + commentLength);

class script_editor(code_editor):
	status_changed = pyqtSignal()
	save_signal = pyqtSignal()
	def __init__(self,):
		code_editor.__init__(self)
		font = QFont()
		font.setFamily('Monospace')
		font.setFixedPitch(True)
		font.setPointSize(17)

		self.setFont(font)

		self.highlighter = Highlighter(self.document())
		self.textChanged.connect(self.callback_edit)
		shortcut = QShortcut(QKeySequence("Ctrl+S"), self)
		shortcut.activated.connect(self.save)

		self.not_saved=False
		self.api_callback=None

	def callback_edit(self):

		self.not_saved=True
		self.status_changed.emit()

	def load(self,file_name):
		self.blockSignals(True)
		self.file_name=file_name
		lines=inp_load_file(file_name)
		self.setPlainText("\n".join(lines))
		self.blockSignals(False)

	def setText(self,text):
		self.blockSignals(True)
		self.setPlainText(text)
		self.blockSignals(False)

	def getText(self):
		return self.toPlainText()

	def save(self):
		self.save_signal.emit()

	def run(self):
		print("Running:",self.file_name)
		mod = imp.load_source("hi",self.file_name)
		api=gpvdm_api(verbose=True)
		api.callback=self.api_callback
		api.path=os.path.dirname(self.file_name)
		a=mod.gpvdm_plugin(api)
		del mod

