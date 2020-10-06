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

from import_data import import_data

from ref import ref_window
from bibtex import bibtex

articles = []
mesh_articles = []

class spectra_main(QWidgetSavePos):

	def changed_click(self):

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Refractive index"):
			b=bibtex()
			if b.load(os.path.join(self.path,"spectra.bib"))!=False:
				text=b.get_text()
				help_window().help_set_help(["spectra_file.png",_("<big><b>Spectra</b></big><br>"+text)])

	def callback_help(self):
		webbrowser.open("https://www.gpvdm.com/docs.html")

	def update(self):
		self.alpha.update()

	def callback_ref(self):
		self.ref_window=ref_window(os.path.join(self.path,"spectra.inp"),"spectra")
		self.ref_window.show()

	def callback_import(self):
		output_file=os.path.join(self.path,"spectra.inp")
		config_file=os.path.join(self.path,"spectra_import.inp")
		self.im=import_data(output_file,config_file,multi_files=True)
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

		self.ribbon.help.triggered.connect(self.callback_help)


		self.main_vbox.addWidget(self.ribbon)

		self.notebook = QTabWidget()

		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)


		files=["mat.inp"]
		description=[_("Parameters")]


		fname=os.path.join(self.path,"spectra.inp")
		self.alpha=plot_widget(enable_toolbar=False)
		self.alpha.set_labels([_("Spectra")])
		self.alpha.load_data([fname])

		self.alpha.do_plot()
		self.notebook.addTab(self.alpha,_("Absorption"))

		for i in range(0,len(files)):
			tab=tab_class(os.path.join(self.path,files[i]))
			self.notebook.addTab(tab,description[i])


		self.setLayout(self.main_vbox)
		
		self.notebook.currentChanged.connect(self.changed_click)



