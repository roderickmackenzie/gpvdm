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

## @package information_noweb
#  Information about gpvdm if we do not have webkit installed.
#

import os
from tab_base import tab_base
from help import my_help_class
from cal_path import get_image_file_path

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon,QPixmap,QPalette
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QScrollArea,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar, QMessageBox, QVBoxLayout, QGroupBox, QTableWidget,QAbstractItemView, QTableWidgetItem, QLabel
from cal_path import get_html_path

class information(QScrollArea,tab_base):

	def __init__(self,file_name):
		QScrollArea.__init__(self)
		self.main_widget=QWidget()

		hbox=QHBoxLayout()
		self.setStyleSheet("background-color:white;");
		self.label = QLabel()

		self.label.setAlignment(Qt.AlignTop)
		self.label.setOpenExternalLinks(True);
		self.label.setWordWrap(True)

		html_file=os.path.join(get_html_path(),file_name)
		text="File not found"
		if os.path.isfile(html_file)==True:
			f = open(os.path.join(get_html_path(),file_name), encoding='utf-8')
			data = f.readlines()
			f.close()
			text=""
			for i in range(0, len(data)):
				line=data[i]
				line=bytes(line, 'utf-8').decode('utf-8', 'ignore')
				text=text+'\n'+data[i].rstrip()

			text=text.replace("get_image_file_path()",get_image_file_path())

		self.label.setText(text)

		hbox.addWidget(self.label)


		self.main_widget.setLayout(hbox)
		self.main_widget.show()
		self.setWidget(self.main_widget)

	def update(self):
		print("")
		


