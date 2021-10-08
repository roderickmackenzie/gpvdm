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

## @package QComboBoxLang
#  A combobox which displays the native language but has an english back end.
#
import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QLabel,QComboBox
from PyQt5.QtCore import pyqtSignal

class QComboBoxLang(QComboBox):

	#changed = pyqtSignal()
	
	def __init__(self):
		QComboBox.__init__(self)
		self.english_list=[]
		self.lang_list=[]
		
		
	def addItemLang(self,english,lang):
		self.addItem(lang)
		self.english_list.append(english)
		self.lang_list.append(lang)
	
	def setValue_using_english(self,english):
		pos=None
		for i in range(0,len(self.english_list)):
			if self.english_list[i]==english:
				pos=i
				break

		if pos!=None:
			self.setCurrentIndex(pos)

	def currentText_english(self):
		pos=self.lang_list.index(self.currentText())
		return self.english_list[pos]
	



