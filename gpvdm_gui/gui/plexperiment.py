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

## @package plexperiment
#  PL simulation set up, for PL spectra.
#

import os
from gui_util import dlg_get_text
import webbrowser
from inp import inp_update_token_value
from util_zip import zip_lsdir
from inp import inp
from inp import inp_copy_file
from inp import inp_remove_file
from util import strextract_interger
from global_objects import global_object_get
from icon_lib import icon_get
from global_objects import global_object_register
import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#window
from tab import tab_class
from QHTabBar import QHTabBar
from gui_util import yes_no_dlg
from PyQt5.QtCore import pyqtSignal
from util import wrap_text
from QWidgetSavePos import QWidgetSavePos
from cal_path import get_sim_path

from inp import inp_get_token_value

from experiment_util import experiment_new_filename

from css import css_apply

from order_widget import order_widget
from tb_lasers import tb_lasers

class plexperiment(QWidgetSavePos):

	changed = pyqtSignal()

	def update(self):
		for item in self.notebook.get_children():
			item.update()

	def callback_help(self):
		webbrowser.open('http://www.gpvdm.com/man/index.html')

	def callback_add_page(self,file_name):
		self.add_page(file_name)
		self.changed.emit()

	def callback_changed(self):
		self.changed.emit()

	def remove_invalid(self,input_name):
		return input_name.replace (" ", "_")


	def clear_pages(self):
		self.notebook.clear()

	def add_page(self,filename):
		name=inp_get_token_value(filename, "#sim_menu_name")
		tab=tab_class(filename)
		self.notebook.addTab(tab,name.split("@")[0])

	def switch_page(self):
		tab = self.notebook.currentWidget()
		self.tb_lasers.update(tab.file_name)

	def __init__(self):
		QWidgetSavePos.__init__(self,"plexperiment")

		self.main_vbox = QVBoxLayout()


		self.setMinimumSize(1200, 700)
		self.setWindowTitle(_("Steady state PL experiment window")+" (https://www.gpvdm.com)")
		self.setWindowIcon(icon_get("icon"))

		toolbar=QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(48, 48))

		self.order_widget=order_widget()
		self.order_widget.new_text=_("New experiment")
		self.order_widget.delete_text=_("Delete experiment")
		self.order_widget.clone_text=_("Clone experiment")
		self.order_widget.rename_text=_("Rename experiment")
		self.order_widget.new_dlg_text=_("New experiment name")+":"
		self.order_widget.base_file_name="pl_ss"
		self.order_widget.new_tab_name=_("experiment ")
		self.order_widget.clone_dlg_text=_("Clone the current experiment to a new experiment called:")
		self.order_widget.rename_dlg_text=_("Rename the experiment to be called")
		self.order_widget.delete_dlg_text=_("Should I remove the experiment file ")
		self.order_widget.postfix="@pl_ss"
		self.order_widget.name_token="#sim_menu_name"
		self.order_widget.init()
		self.order_widget.changed.connect(self.callback_changed)
		self.order_widget.added.connect(self.callback_add_page)

		toolbar.addWidget(self.order_widget)

		self.tb_lasers=tb_lasers()
		toolbar.addWidget(self.tb_lasers)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)


		self.help = QAction(icon_get("help"), _("Help"), self)
		self.help.setStatusTip(_("Close"))
		self.help.triggered.connect(self.callback_help)
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)


		self.notebook = QTabWidget()
		self.notebook.currentChanged.connect(self.switch_page)
		self.order_widget.notebook_pointer=self.notebook
		css_apply(self.notebook,"style_h.css")
		self.notebook.setTabBar(QHTabBar())
		self.notebook.setTabPosition(QTabWidget.West)
		self.notebook.setMovable(True)

		self.order_widget.load_tabs()

		self.main_vbox.addWidget(self.notebook)

		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)



		self.setLayout(self.main_vbox)



