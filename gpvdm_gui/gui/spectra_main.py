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

## @package spectra_main
#  An editro for optical spectra.
#

import os
from tab import tab_class
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget
from PyQt5.QtGui import QPainter,QIcon

#python modules
import webbrowser

from help import help_window

from plot_widget import plot_widget
from win_lin import desktop_open

from QWidgetSavePos import QWidgetSavePos

from ribbon_spectra import ribbon_spectra

from import_data_json import import_data_json
from json_spectra_db_item import json_spectra_db_item

from ref import ref_window
from bibtex import bibtex

class spectra_main(QWidgetSavePos):

	def changed_click(self):

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Refractive index"):
			b=bibtex()
			if b.load(os.path.join(self.path,"spectra.bib"))!=False:
				text=b.get_text()
				help_window().help_set_help(["spectra_file.png",_("<big><b>Spectra</b></big><br>"+text)])

	def update(self):
		self.alpha.update()

	def callback_ref(self):
		self.ref_window=ref_window(os.path.join(self.path,"spectra.inp"),"spectra")
		self.ref_window.show()

	def callback_import(self):
		self.im=import_data_json(self.data.spectra_import,export_path=self.path)
		self.im.run()
		self.update()

	def __init__(self,path):
		QWidgetSavePos.__init__(self,"spectra_main")
		self.path=path
		self.setMinimumSize(900, 600)
		self.setWindowIcon(icon_get("spectra_file"))

		self.setWindowTitle(_("Optical spectrum editor")+" (https://www.gpvdm.com)"+" "+os.path.basename(self.path)) 
		

		self.main_vbox = QVBoxLayout()

		self.ribbon=ribbon_spectra()
		

		self.ribbon.import_data.clicked.connect(self.callback_import)
		self.ribbon.tb_ref.triggered.connect(self.callback_ref)

		self.main_vbox.addWidget(self.ribbon)

		self.notebook = QTabWidget()

		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		mat_file=os.path.join(self.path,"data.json")
		self.data=json_spectra_db_item()
		self.data.load(mat_file)
		self.data.spectra_import.data_file="spectra.inp"


		fname=os.path.join(self.path,"spectra.inp")
		self.alpha=plot_widget(enable_toolbar=False)
		self.alpha.set_labels([_("Spectra")])
		self.alpha.load_data([fname])

		self.alpha.do_plot()
		self.notebook.addTab(self.alpha,_("Absorption"))

		tab=tab_class(self.data)
		self.notebook.addTab(tab,_("Basic"))


		self.setLayout(self.main_vbox)
		
		self.notebook.currentChanged.connect(self.changed_click)



