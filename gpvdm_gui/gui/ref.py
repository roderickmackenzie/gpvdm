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

## @package ref
#  Reference manager window.
#


from icon_lib import icon_get
import os

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QHBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView,QPushButton
from PyQt5.QtGui import QPainter,QIcon

#windows

from tab import tab_class

from bibtex import bibtex

from QWidgetSavePos import QWidgetSavePos
from QWidgetSavePos import resize_window_to_be_sane
from help import QAction_help

class ref_window(QWidgetSavePos):
	def __init__(self,bib_file,token,show_toolbar=True):
		"""Pass this the file name of the file you want referenced."""
		QWidgetSavePos.__init__(self,"ref_window")
		resize_window_to_be_sane(self,0.5,0.5)
		self.bib_file=bib_file
		self.token=token
		self.setWindowIcon(icon_get("ref"))
		self.setWindowTitle(_("Reference manager")+" (https://www.gpvdm.com)")

		self.vbox=QVBoxLayout()


		self.toolbar=QToolBar()
		self.toolbar.setIconSize(QSize(48, 48))

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.toolbar.addWidget(spacer)

		self.tb_help = QAction_help()
		self.toolbar.addAction(self.tb_help)

		if show_toolbar==True:
			self.vbox.addWidget(self.toolbar)


		self.button_widget=QWidget()
		self.button_hbox=QHBoxLayout()
		self.button_widget.setLayout(self.button_hbox)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
		self.button_hbox.addWidget(spacer)


		self.b=bibtex()
		self.b.load(self.bib_file)
		self.item=self.b.get_ref(self.token)
		if self.item==False:
			self.item=self.b.new()
			self.item.token=token

		self.tab=tab_class(self.item,data=self.item)
		#self.item.bib_dump()
		self.tab.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.vbox.addWidget(self.tab)

		self.vbox.addWidget(self.button_widget)
		self.setLayout(self.vbox)


