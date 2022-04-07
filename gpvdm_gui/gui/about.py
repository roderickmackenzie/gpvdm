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


import os
from const_ver import const_ver
from cal_path import get_image_file_path
from cal_path import get_materials_path
from cal_path import get_html_path
import sys

from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QPushButton,QListWidget,QTextBrowser,QTabWidget,QWidget,QHBoxLayout,QLabel,QDialog,QVBoxLayout,QListWidgetItem,QListView
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import QSize
from PyQt5.QtGui import QIcon

from icon_lib import icon_get

from cal_path import get_device_lib_path, get_bin_path, get_plugins_path, get_exe_path, get_exe_command, get_exe_name
from lock import get_lock

from cal_path import multiplatform_exe_command
from cal_path import get_exe_command

class about_dlg(QDialog):
	def __init__(self):
		QDialog.__init__(self)
		self.main_hbox=QHBoxLayout()
		self.left_vbox=QVBoxLayout()
		self.main_vbox=QVBoxLayout()
		self.setFixedSize(750,480) 
		self.setWindowTitle(_("About")+" (https://www.gpvdm.com)")
		self.setWindowIcon(icon_get("icon"))
		self.gpvdm=QLabel("<font size=40><b>gpvdm_gui</b></font>")
		self.image=QLabel()
		self.image.mousePressEvent=self.callback
		self.written_by=QLabel(_("Written by Roderick MacKenzie 2012-2019, published under the GPL V2.0 license."))
		self.written_by.setWordWrap(True)
		self.ver=QLabel(_("Version ")+const_ver())
		pixmap = QPixmap(os.path.join(get_image_file_path(),"image.jpg"))
		self.image.setPixmap(pixmap)
		self.left_vbox.addWidget(self.gpvdm)
		self.left_vbox.addWidget(self.image)
		self.left_vbox.addWidget(self.written_by)
		self.left_vbox.addWidget(self.ver)
		self.left=QWidget()
		self.left.setLayout(self.left_vbox)
		self.right=QTabWidget()
		self.right.setMinimumWidth(500)
		self.about=QTextBrowser()
		text=""
		text=text+_("gpvdm is a general-purpose tool for simulation of light harvesting devices. It was originally written to simulate organic solar cells and OLEDs, but it has recently been extended to simulate other devices including silicon based devices. Currently the model can sumulate:")
		text=text+"<ul>"
		text=text+"<li>"+_("Organic solar cells")+"</li>"
		text=text+"<li>"+_("Organic LEDs")+"</li>"
		text=text+"<li>"+_("Crystalline silicon solar cells")+"</li>"
		text=text+"<li>"+_("a-Si solar cells")+"</li>"
		text=text+"<li>"+_("CIGS solar cells")+"</li>"
		text=text+"</ul> "
		text=text+_("The model solves both electron and hole drift-diffusion, and carrier continuity equations in position space to describe the movement of charge within the device. The model also solves Poisson's equation to calculate the internal electrostatic potential. Recombination and carrier trapping are described within the model using a Shockley-Read-Hall (SRH) formalism, the distribution of trap sates can be arbitrarily defined. All equations can be solved either in steady state or in time domain. A fuller description of the model can be found in the here, in the associated publications and in the manual.")
		text=text+"<br>"
		text=text+"<br>"
		text=text+"<center><a href=\"https://www.gpvdm.com\">https://www.gpvdm.com</a></center>"
		self.about.setText(text)
		self.right.addTab(self.about,_("About"))
		
		self.license=QTextBrowser()
		text=""
		license_file=os.path.join(get_html_path(),"LICENSE.html")
		if (os.path.isfile(license_file)==True):
				f = open(license_file, mode='rb')
				lines = f.read()
				f.close()
				lines=lines.decode('utf-8')
				lines=lines.split("\n")
				for l in lines:
					text=text+l+"<br>"

		self.license.setText(text)
		self.right.addTab(self.license,_("Legal"))
		
		self.translations=QTextBrowser()
		text=""

		text=text+"Translations of gpvdm:"
		text=text+"<br>"
		text=text+"<br>"
		text=text+"<b>Greek</b>: Dimitris Tsikritzis"
		text=text+"<br>"
		text=text+"<b>Chinese</b>: Liu Di (刘迪) and Zhao Chenyao (赵辰尧)"
		text=text+"<br>"
		text=text+"<b>Portuguese</b>: Luciano Azevedo Neves"
		text=text+"<br>"
		text=text+"<br>"
		text=text+"Help translate gpvdm into your language.."
		text=text+"If so then please consider joining the gpvdm translation effort.  This is somthing you can put on your CV and it\'s a way to make sure that speakers of your language have access to high quality scientific tools for simulating solar cells."

		self.translations.setText(text)
		self.right.addTab(self.translations,_("Translations"))




		self.paths=QTextBrowser()
		text=""

		text=text+"<b>"+_("Materials library path")+":</b>"+get_materials_path()+"<br>"
		text=text+"<b>"+_("Device library path")+":</b>"+get_device_lib_path()+"<br>"
		text=text+"<b>"+_("Binary path")+":</b>"+get_bin_path()+"<br>"
		text=text+"<b>"+_("Plugins path")+":</b>"+get_plugins_path()+"<br>"
		text=text+"<b>"+_("Binary name")+":</b>"+get_exe_name()+"<br>"
		text=text+"<b>"+_("Install ID")+":</b>"+get_lock().data.uid+"<br>"

		self.paths.setText(text)
		self.right.addTab(self.paths,_("Paths"))


		#self.materials=QListWidget()
		#self.right.addTab(self.materials,_("Materials"))

		self.main_hbox.addWidget(self.left)
		self.main_hbox.addWidget(self.right)
		self.widget_main_hbox=QWidget()
		self.widget_main_hbox.setLayout(self.main_hbox)
		self.main_vbox.addWidget(self.widget_main_hbox)
		
		self.hwidget=QWidget()

		self.closeButton = QPushButton(_("Close"))
		self.closeButton.clicked.connect(self.callback_close)
		hbox = QHBoxLayout()
		hbox.addStretch(1)
		hbox.addWidget(self.closeButton)

		self.hwidget.setLayout(hbox)
		
		self.main_vbox.addWidget(self.hwidget)

		self.setLayout(self.main_vbox)
		self.show()

		self.mat_icon = icon_get("organic_material")

	def callback_close(self):
		self.close()

	def callback(self,event):
		command=multiplatform_exe_command(get_exe_command()+" --license")
		os.system(command)
		print("ok")
