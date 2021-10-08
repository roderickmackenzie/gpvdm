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

## @package dlg_get_multi_text
#  A dialog to get multiple lines of text.
#

import os

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QLineEdit,QComboBox,QHBoxLayout,QPushButton,QLabel,QDialog,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QApplication
from PyQt5.QtGui import QPainter,QIcon,QImage

from PyQt5.QtCore import QSize, Qt

class dlg_get_multi_text(QDialog):

#	def callback_edit2(self):
#		self.populate_combo_box_using_input_file(self.token2,self.file2.text())

	def callback_click_ok(self):
		self.ret=[]
		for box in self.edit_boxes:
			self.ret.append(box.text())
	
		self.close()

	def callback_click_cancel(self):
		self.close()

	def __init__(self,info,title="gpvdm"):
		QWidget.__init__(self)
		self.ret=False
		self.setWindowTitle(title)
		self.setWindowFlags(Qt.WindowStaysOnTopHint)
		self.vbox=QVBoxLayout()
		self.edit_boxes=[]
		for i in range(0,len(info)):
			l=QLabel(info[i][0])
			self.vbox.addWidget(l)

			self.edit_boxes.append(QLineEdit())
			self.edit_boxes[-1].setText(info[i][1])

			self.vbox.addWidget(self.edit_boxes[-1])


		w=QWidget()
		hb=QHBoxLayout()
		w.setLayout(hb)
		ok=QPushButton(_("OK"))
		ok.pressed.connect(self.callback_click_ok)
		cancel=QPushButton(_("Cancel"))
		cancel.pressed.connect(self.callback_click_cancel)
		
		hb.addWidget(cancel)
		hb.addWidget(ok)

		self.vbox.addWidget(w)

		self.setLayout(self.vbox)

	def get_values(self):
		return self.ret

	def run(self):
		self.exec_()


