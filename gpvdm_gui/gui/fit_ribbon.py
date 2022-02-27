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

## @package fit_ribbon
#  The ribbon for the fit window.
#

import os


from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import  QAction
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QLineEdit, QToolButton

from icon_lib import icon_get

from util import wrap_text

from ribbon_base import ribbon_base

from play import play
from tick_cross import tick_cross
from QAction_lock import QAction_lock
from help import QAction_help

class fit_ribbon(ribbon_base):
		
	def file(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.tb_new = QAction(icon_get("document-new"), wrap_text(_("New experiment"),2), self)
		toolbar.addAction(self.tb_new)

		######## Box widget
		self.box_widget=QWidget()
		self.box=QVBoxLayout()
		self.box.setSpacing(0)
		self.box.setContentsMargins(0, 0, 0, 0)
		self.box_widget.setLayout(self.box)
		self.box_tb0=QToolBar()
		self.box_tb0.setIconSize(QSize(32, 32))
		self.box.addWidget(self.box_tb0)
		self.box_tb1=QToolBar()
		self.box_tb1.setIconSize(QSize(32, 32))
		self.box.addWidget(self.box_tb1)
		

		self.tb_delete = QAction(icon_get("edit-delete"), wrap_text(_("Delete experiment"),3), self)
		self.box_tb0.addAction(self.tb_delete)

		self.tb_refresh = QAction(icon_get("view-refresh"), wrap_text(_("Refresh"),3), self)
		self.box_tb0.addAction(self.tb_refresh)

		self.tb_clone = QAction(icon_get("clone"), wrap_text(_("Clone experiment"),3), self)
		self.box_tb1.addAction(self.tb_clone)


		self.tb_rename = QAction(icon_get("rename"), wrap_text(_("Rename experiment"),3), self)
		self.box_tb1.addAction(self.tb_rename )

		#self.tb_notes = QAction(icon_get("text-x-generic"), wrap_text(_("Notes"),3), self)
		#toolbar.addAction(self.tb_notes)

		toolbar.addWidget(self.box_widget)


		toolbar.addSeparator()

		self.export_zip = QAction_lock("zip", _("Export\nData"), self,"main_zip")
		toolbar.addAction(self.export_zip)

		self.import_data= QAction(icon_get("import"), wrap_text(_("Import data"),4), self)
		toolbar.addAction(self.import_data)

		toolbar.addSeparator()

		self.tb_configure= QAction(icon_get("preferences-system"), wrap_text(_("Configure"),4), self)
		toolbar.addAction(self.tb_configure)


		toolbar.addSeparator()

		self.play_one= play(self,"fit_ribbon_one",run_text=wrap_text(_("One itteration"),2))

		toolbar.addAction(self.play_one)
		
		self.play= play(self,"fit_ribbon_run",play_icon="forward",run_text=wrap_text(_("Run fit"),2))
		toolbar.addAction(self.play)

		toolbar.addSeparator()

		self.enable=tick_cross(enable_text=_("Fit this\ndata set"),disable_text=_("Dont' fit this\ndata set"))
		toolbar.addAction(self.enable)



		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)


		self.help = QAction_help()
		toolbar.addAction(self.help)

		return toolbar



	def update(self):
		print("update")
		#self.device.update()
		#self.simulations.update()
		#self.configure.update()
		#self.home.update()


	def __init__(self):
		ribbon_base.__init__(self)
		#self.setStyleSheet("QWidget {	background-color:cyan; }")


		w=self.file()
		self.addTab(w,_("Experimental data"))
		
		#w=self.run()
		#self.addTab(w,_("Run"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

