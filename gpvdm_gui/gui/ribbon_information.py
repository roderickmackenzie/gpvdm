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

## @package ribbon_information
#  The information ribbon.
#


import os
from icon_lib import icon_get

from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton
from PyQt5.QtWidgets import QTabWidget

import webbrowser
from help import help_window

from inp import inp

from cal_path import get_sim_path
from util import wrap_text

from bibtex import bibtex
from ribbon_page import ribbon_page

class ribbon_information(ribbon_page):
	def __init__(self):
		ribbon_page.__init__(self)
		self.simulation_notes_window=None

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))

		self.simulation_notes = QAction(icon_get("text-x-generic"), _("Simulation\nNotes"), self)
		self.simulation_notes.triggered.connect(self.callback_simulation_notes)
		self.addAction(self.simulation_notes)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)

		self.hints = QAction(icon_get("hints.png"), _("Hints\nWindow"), self)
		self.hints.triggered.connect(self.callback_help)
		self.addAction(self.hints)

		self.hints = QAction(icon_get("internet-chat"), _("Interface\nLanguage"), self)
		self.hints.triggered.connect(self.callback_language)
		self.addAction(self.hints)



		self.paper = QAction(icon_get("pdf"), wrap_text(_("Assosiated paper"),8), self)
		self.paper.triggered.connect(self.callback_paper)
		self.addAction(self.paper)

		self.twitter = QAction(icon_get("twitter.png"), _("twitter"), self)
		self.twitter.triggered.connect(self.callback_twitter)
		self.addAction(self.twitter)


		self.youtube = QAction(icon_get("youtube.png"), _("Youtube\nchannel"), self)
		self.youtube.triggered.connect(self.callback_youtube)
		self.addAction(self.youtube)

		self.man = QAction(icon_get("internet-web-browser"), _("Documentation")+"\n", self)
		self.man.triggered.connect(self.callback_on_line_help)
		self.addAction(self.man)

	def update(self):
		if inp().isfile(os.path.join(get_sim_path(),"sim.bib"))==True:
			self.paper.setVisible(True)
		else:
			self.paper.setVisible(False)

	def setEnabled(self,val):
		self.license.setEnabled(val)
		self.ref.setEnabled(val)
		self.hints.setEnabled(val)
		self.youtube.setEnabled(val)
		self.man.setEnabled(val)

	def callback_license(self):
		webbrowser.open("https://www.gpvdm.com/license.html")

	def callback_youtube(self):
		webbrowser.open("https://www.youtube.com/channel/UCbm_0AKX1SpbMMT7jilxFfA")

	def callback_twitter(self):
		webbrowser.open("https://twitter.com/gpvdm_info")

	def callback_paper(self):
		b=bibtex()
		if b.load(os.path.join(get_sim_path(),"sim.bib"))==True:
			ref=b.get_ref("simulation")
			if ref!=False:
				if r.url!="":
					webbrowser.open(r.url)#


	def callback_on_line_help(self):
		#print("here")
		#self.a=cool_menu(self.ribbon.home.help.icon())
		#self.a.show()
		#self.a.setVisible(True)

		#self.a.setFocusPolicy(Qt.StrongFocus)
		#self.a.setFocus(True)
		#self.a.hasFocus()
		webbrowser.open("http://www.gpvdm.com/docs.html")

	def callback_help(self, widget, data=None):
		help_window().toggle_visible()
			

	def callback_language(self):
		from config_window import class_config_window
		from tab_lang import language_tab_class
		self.config_window=class_config_window([],[])
		lang_tab=language_tab_class()
		self.config_window.notebook.addTab(lang_tab,_("Language"))
		self.config_window.show()

	def callback_simulation_notes(self, widget, data=None):
		help_window().help_set_help(["si.png",_("<big><b>Record notes about the simulation here</b></big><br>Use this window to make notes about the simulation.")])


		if self.simulation_notes_window==None:
			from window_simulation_notes import window_simulation_notes
			self.simulation_notes_window=window_simulation_notes()

		if self.simulation_notes_window.isVisible()==True:
			self.simulation_notes_window.hide()
		else:
			self.simulation_notes_window.show()
