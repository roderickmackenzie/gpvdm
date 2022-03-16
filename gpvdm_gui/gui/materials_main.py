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

## @package materials_main
#  Dialog to show information about a material.
#

import os
from tab import tab_class
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QDialog
from PyQt5.QtGui import QPainter,QIcon

from help import help_window

from win_lin import desktop_open

from ref import ref_window
from bibtex import bibtex

from gpvdm_open import gpvdm_open

from QWidgetSavePos import QWidgetSavePos
from plot_widget import plot_widget

from ribbon_materials import ribbon_materials
from import_data_json import import_data_json
from equation_editor import equation_editor
from json_material_db_item import json_material_db_item
import webbrowser
from inp import inp

class materials_main(QWidgetSavePos):

	def changed_click(self):
		self.ribbon.import_data.setEnabled(False)
		self.ribbon.equation.setEnabled(False)
		self.ribbon.tb_ref.setEnabled(False)

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Electrical parameters"):
			help_window().help_set_help(["tab.png",_("<big><b>Electrical parameters</b></big><br>Use this tab to configure the electrical parameters for the material.")])


		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Luminescence"):
			help_window().help_set_help(["tab.png",_("<big><b>Luminescence</b></big><br>Use this tab to edit the materials Luminescence.")])
			self.ribbon.import_data.setEnabled(False)

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Absorption"):
			b=bibtex()
			if b.load(os.path.join(self.path,"mat.bib"))!=False:
				text=b.get_text_of_token("alpha",html=True)
				if text!=False:
					help_window().help_set_help(["alpha.png",_("<big><b>Absorption</b></big><br>"+text)])

			self.ribbon.import_data.setEnabled(True)
			self.ribbon.equation.setEnabled(True)
			self.ribbon.tb_ref.setEnabled(True)

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Refractive index"):
			b=bibtex()
			if b.load(os.path.join(self.path,"mat.bib"))!=False:
				text=b.get_text_of_token("n",html=True)
				if text!=False:
					help_window().help_set_help(["n.png",_("<big><b>Refractive index</b></big><br>"+text)])

			self.ribbon.import_data.setEnabled(True)
			self.ribbon.equation.setEnabled(True)
			self.ribbon.tb_ref.setEnabled(True)

	def callback_cost(self):
		desktop_open(os.path.join(self.path,"cost.xlsx"))


	def __init__(self,path):
		QWidgetSavePos.__init__(self,"materials_main")
		self.path=path
		#self.setFixedSize(900, 600)
		self.setMinimumSize(900, 600)
		self.setWindowIcon(icon_get("organic_material"))

		self.setWindowTitle(_("Material editor")+" (https://www.gpvdm.com)"+" "+os.path.basename(self.path)) 
		

		self.main_vbox = QVBoxLayout()

		self.ribbon=ribbon_materials()
		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
		self.ribbon.cost.triggered.connect(self.callback_cost)
		self.ribbon.import_data.clicked.connect(self.import_data)
		self.ribbon.equation.clicked.connect(self.callback_equation_editor)

		self.ribbon.tb_ref.triggered.connect(self.callback_ref)
		self.ribbon.help.triggered.connect(self.callback_help)

		self.main_vbox.addWidget(self.ribbon)

		self.notebook = QTabWidget()
		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		fname=os.path.join(self.path,"alpha.gmat")
		self.alpha=plot_widget(enable_toolbar=False)
		self.alpha.show_title=False
		self.alpha.set_labels([_("Absorption")])
		self.alpha.load_data([fname])

		self.alpha.do_plot()
		self.notebook.addTab(self.alpha,_("Absorption"))

		fname=os.path.join(self.path,"n.gmat")
		self.n=plot_widget(enable_toolbar=False)
		self.n.show_title=False
		self.n.set_labels([_("Refractive index")])
		self.n.load_data([fname])
		self.n.do_plot()

		self.notebook.addTab(self.n,_("Refractive index"))

		mat_file=os.path.join(self.path,"data.json")
		self.data=json_material_db_item()
		self.data.load(mat_file)
		self.data.n_import.data_file="n.gmat"
		self.data.alpha_import.data_file="alpha.gmat"

		tab=tab_class(self.data,data=self.data)
		self.notebook.addTab(tab,_("Basic"))

		tab=tab_class(self.data.electrical_constants,data=self.data)
		self.notebook.addTab(tab,_("Electrical parameters"))

		tab=tab_class(self.data.thermal_constants,data=self.data)
		self.notebook.addTab(tab,_("Thermal parameters"))


		self.setLayout(self.main_vbox)
		
		self.notebook.currentChanged.connect(self.changed_click)

	def callback_equation_editor(self):
		equation_file=None
		file_name=None
		data_label=""
		data_units=""
		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Absorption"):
			file_name="alpha.gmat"
			equation_file="alpha_eq.inp"
			data_label="Absorption"
			data_units="m^{-1}"


		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Refractive index"):
			file_name="n.gmat"
			equation_file="n_eq.inp"
			data_label="n"
			data_units="au"

		if file_name!=None:
			output_file=os.path.join(self.path,file_name)
			config_file=os.path.join(self.path,file_name+"import.inp")

			self.equation_editor=equation_editor(self.path,equation_file,file_name)
			self.equation_editor.data_written.connect(self.update)

			self.equation_editor.data.y_label="Wavelength"
			self.equation_editor.data.data_label=data_label

			self.equation_editor.data.y_units="nm"
			self.equation_editor.data.data_units=data_units
			self.equation_editor.load()

			self.equation_editor.show()

	def import_data(self):
		file_name=None
		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Absorption"):
			self.im=import_data_json(self.data.alpha_import,export_path=self.path)
			self.im.run()
			self.update()
			self.data.save()

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Refractive index"):
			self.im=import_data_json(self.data.n_import,export_path=self.path)
			self.im.run()
			self.update()
			self.data.save()

	def update(self):
		self.n.update()
		self.alpha.update()

	def callback_ref(self):
		token=None
		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Absorption"):
			token="alpha"

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Refractive index"):
			token="n"

		if token!=None:
			#print("path=",os.path.join(self.path,"mat.bib"))
			self.ref_window=ref_window(os.path.join(self.path,"mat.bib"),token)
			self.ref_window.show()

	def callback_dir_open(self):
		dialog=gpvdm_open(self.path)
		dialog.show_inp_files=False
		ret=dialog.exec_()

		if ret==QDialog.Accepted:
			desktop_open(dialog.get_filename())

	def callback_help(self):
		webbrowser.open("https://www.gpvdm.com/docs.html")
