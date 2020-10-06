# -*- coding: utf-8 -*-
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

## @package fit_ribbon
#  The ribbon for the fit window.
#

import os


from code_ctrl import enable_betafeatures
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import  QAction
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QToolBar, QLineEdit, QToolButton

from order_widget import order_widget


from icon_lib import icon_get


from util import wrap_text

from ribbon_base import ribbon_base

from play import play
from tick_cross import tick_cross
from QAction_lock import QAction_lock

class fit_ribbon(ribbon_base):
		
	def file(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		self.order_widget=order_widget()
		self.order_widget.new_text=_("New experiment")
		self.order_widget.delete_text=_("Delete experiment")
		self.order_widget.clone_text=_("Clone experiment")
		self.order_widget.rename_text=_("Rename experiment")
		self.order_widget.new_dlg_text=_("New experiment name:")
		self.order_widget.base_file_name=["fit","fit_data","fit_patch","fit_math"]
		self.order_widget.new_tab_name=_("fit ")
		self.order_widget.clone_dlg_text=_("Clone the current experiment to a new experiment called:")
		self.order_widget.rename_dlg_text=_("Rename the experiment to be called:")
		self.order_widget.delete_dlg_text=_("Should I remove the experiment file ")
		self.order_widget.name_token="#fit_name"
		self.order_widget.init()

		toolbar.addWidget(self.order_widget)

		toolbar.addSeparator()

		self.import_data= QAction(icon_get("import"), wrap_text(_("Import experimental data"),8), self)
		toolbar.addAction(self.import_data)

		toolbar.addSeparator()

		self.enable=tick_cross(enable_text=_("Fit this\ndata set"),disable_text=_("Dont' fit this\ndata set"))
		toolbar.addAction(self.enable)

		return toolbar

	def export(self):
		self.export_toolbar = QToolBar()
		self.export_toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.export_toolbar.setIconSize(QSize(42, 42))

		self.tb_export_as_jpg = QAction_lock("export_image", _("Export\nimage"), self,"fit_export_image")
		self.export_toolbar.addAction(self.tb_export_as_jpg)

		self.tb_export_as_csv = QAction_lock("export_csv", _("Export\ncsv"), self,"fit_export_csv")
		self.export_toolbar.addAction(self.tb_export_as_csv)

		self.tb_export_as_xls = QAction_lock("export_xls", _("Export\nxls"), self,"fit_export_xls")
		#self.export_toolbar.addAction(self.tb_export_as_xls)

		self.tb_export_as_gnuplot = QAction_lock("export_gnuplot", _("Export\ngnuplot"), self,"fit_export_gnuplot")
		self.export_toolbar.addAction(self.tb_export_as_gnuplot)

		return self.export_toolbar

	def run(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))

		toolbar.addSeparator()

		self.tb_configure= QAction(icon_get("preferences-system"), wrap_text(_("Configure"),4), self)
		toolbar.addAction(self.tb_configure)


		toolbar.addSeparator()

		self.play_one= play(self,"fit_ribbon_one",run_text=wrap_text(_("One itteration"),2))

		toolbar.addAction(self.play_one)
		
		self.play= play(self,"fit_ribbon_run",play_icon="forward",run_text=wrap_text(_("Run fit"),2))

		toolbar.addAction(self.play)


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)


		self.help = QAction(icon_get("help"), _("Help"), self)

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
		
		w=self.run()
		self.addTab(w,_("Run"))

		w=self.export()
		self.addTab(w,_("Export"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

