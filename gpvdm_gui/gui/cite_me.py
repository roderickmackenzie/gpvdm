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


## @package cite_me
#  A widget to encourage people to cite the model
#


import os

#qt
from PyQt5.QtWidgets import QMainWindow,QLabel, QFrame,QTextEdit, QAction,QApplication,QTableWidgetItem,QComboBox, QMessageBox, QDialog, QDialogButtonBox, QFileDialog
from PyQt5.QtWidgets import QGraphicsScene,QListWidgetItem,QListView,QLineEdit,QWidget,QHBoxLayout,QPushButton,QSizePolicy
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

class cite_me(QLabel):

	changed = pyqtSignal()

	def __init__(self):
		QLabel.__init__(self)
		try:
			self.setWordWrap(True)
			self.setMinimumWidth(350)
			bib=bibtex()
			bib.load(os.path.join(gpvdm_paths.get_bib_path(),"cite.bib"))

			number=sum(ord(c) for c in get_lock().get_uid()) %10


			if number<len(bib.refs):
				text=bib.refs[number].bib_short_cite()
				self.setText("<b>Please cite,</b> "+text+" when using gpvdm in your work.  And any 2 other papers listed <a href=\"https://scholar.google.co.uk/citations?user=jgQqfLsAAAAJ&hl=en\">here.</a>")
		except:
			pass

	def mousePressEvent(self, ev):
		webbrowser.open("https://scholar.google.co.uk/citations?user=jgQqfLsAAAAJ&hl=en")
		
