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

## @package plot_widget_menu
#  The main plot widget menu
#

from __future__ import unicode_literals

import os
import io
from PyQt5.QtWidgets import QMenu
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QApplication
from PyQt5.QtGui import QPainter,QIcon,QImage

from icon_lib import icon_get

class plot_widget_menu(QWidget):

	def menu_build(self):
		self.main_menu = QMenu(self)

		clipboard=self.main_menu.addMenu(icon_get("edit-copy"),_("Copy"))

		action=clipboard.addAction(icon_get("image-x-generic"),_("Image"))
		action.triggered.connect(self.callback_do_clip)

		save_as=self.main_menu.addMenu(_("Save as"))

		action=save_as.addAction(icon_get("export_gnuplot"),_("Gnuplot"))
		action.triggered.connect(self.callback_save_gnuplot)

		action=save_as.addAction(icon_get("edit-copy"),_("Image"))
		action.triggered.connect(self.callback_save_image)

		action=save_as.addAction(icon_get("export_csv"),_("Csv"))
		action.triggered.connect(self.callback_save_csv)

		action=save_as.addAction(icon_get("text-x-generic"),_("Text file (txt)"))
		action.triggered.connect(self.callback_save_txt)

		action=save_as.addAction(icon_get("wps-office-xls"),_("MS Excel (xlsx)"))
		action.triggered.connect(self.callback_save_xlsx)

		graph=self.main_menu.addMenu(_("Graph"))
		self.menu_show_title=graph.addAction(_("Show title"))
		self.menu_show_title.setCheckable(True)
		self.menu_show_title.setChecked(self.show_title)
		self.menu_show_title.triggered.connect(self.callback_hide_show_title)

		self.menu_show_key=graph.addAction(_("Show key"))
		self.menu_show_key.setCheckable(True)
		self.menu_show_key.setChecked(True)
		self.menu_show_key.triggered.connect(self.callback_hide_show_key)



	def callback_hide_show_title(self):
		self.show_title=self.menu_show_title.isChecked()
		self.do_plot()

	def callback_hide_show_key(self):
		self.show_key=self.menu_show_key.isChecked()
		self.do_plot()
