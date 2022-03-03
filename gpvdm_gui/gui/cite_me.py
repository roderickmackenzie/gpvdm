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


## @package cite_me
#  A widget to encourage people to cite the model
#


import os

#qt
from PyQt5.QtWidgets import QMainWindow,QLabel, QFrame,QTextEdit, QAction,QApplication,QTableWidgetItem,QComboBox, QMessageBox, QDialog, QVBoxLayout , QDialogButtonBox, QFileDialog
from PyQt5.QtWidgets import QGraphicsScene,QListWidgetItem,QListView,QTextEdit,QWidget,QHBoxLayout,QPushButton,QSizePolicy
from PyQt5.QtWidgets import QFileDialog
from PyQt5.uic import loadUi
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.QtCore import QPersistentModelIndex
from QComboBoxLang import QComboBoxLang
from PyQt5.QtGui import QIcon


#cal_path
from cal_path import gpvdm_paths
from PyQt5.QtCore import pyqtSignal

from epitaxy import get_epi
from inp import inp
from dos_io import gen_fermi_from_np
from dos_io import gen_np_from_fermi
from bibtex import bibtex
from lock import get_lock
import webbrowser
import getpass
from icon_lib import icon_get
from PyQt5.QtGui import QFont

class cite_me(QLabel):

	changed = pyqtSignal()

	def gen_window(self):
		self.win=QWidget()
		self.win.setWindowIcon(icon_get("icon"))
		self.win.setWindowTitle(_("Please cite these papers:")) 
		self.win.setWindowFlags(Qt.WindowStaysOnTopHint)

		vbox=QVBoxLayout()

		l=QLabel(_("If you publish a paper, book or thesis using results from gpvdm. You must cite these three papers listed below:"))
		l.setFont(QFont('SansSerif', 14))
		l.setWordWrap(True)
		vbox.addWidget(l)

		self.papers = QTextEdit()
		vbox.addWidget(self.papers)
		self.papers.setText(self.all_text)
		
		self.win.setLayout(vbox)

		self.win.setFixedWidth(350)
		self.win.setMinimumHeight(500)
		self.win.show()

		
	def __init__(self):
		QLabel.__init__(self)
		try:
			self.setWordWrap(True)
			self.setMinimumWidth(350)
			bib=bibtex()
			bib.load(os.path.join(gpvdm_paths.get_bib_path(),"cite.bib"))

			number=sum(ord(c) for c in get_lock().get_uid()) %10

			self.all_text=""
			if number<len(bib.refs):
				text=bib.refs[number].bib_short_cite()
				count=0
				pos=number
				while count<3:
					self.all_text=self.all_text+str(count+1)+". "+bib.refs[pos].bib_short_cite()+"\n\n"
					count=count+1
					pos=pos+1
					if pos>len(bib.refs):
						pos=0
				self.all_text=self.all_text+"\n"+"Please do not cite the manual.  See the manual why I ask you to cite in this way. Thank you!"

				self.setText("<b>If you publish results generated with gvpdm in a paper, book or thesis you must cite this paper:</b> "+text+" and along with these <a href=\"https://scholar.google.co.uk/citations?user=jgQqfLsAAAAJ&hl=en\">two papers</a> in your work.")
		except:
			pass

	def mousePressEvent(self, ev):
		self.gen_window()


