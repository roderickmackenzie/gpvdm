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

## @package lasers
#  Main laser editor window.
#

import os
import webbrowser
from inp import inp_update_token_value
from inp import inp_get_token_value
from tab import tab_class
from util_zip import zip_lsdir
from inp import inp
from inp import inp_copy_file
from inp import inp_remove_file
from util import strextract_interger
from icon_lib import icon_get

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon, QPainter, QFont, QColor
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QLabel,QComboBox, QTabWidget,QStatusBar,QMenuBar, QTabBar, QStylePainter, QStyleOptionTab,QStyle

#window
from gui_util import yes_no_dlg
from gui_util import dlg_get_text
from util import wrap_text

from QWidgetSavePos import QWidgetSavePos
from cal_path import get_sim_path

from css import css_apply

from order_widget import order_widget

def laser_new_filename():
	for i in range(0,20):
		pulse_name="laser"+str(i)+".inp"
		if inp().isfile(os.path.join(get_sim_path(),pulse_name))==False:
			return i
	return -1



class lasers(QWidgetSavePos):

	def update(self):
		for item in self.notebook.get_children():
			item.update()

	def get_main_menu(self, window):
		accel_group = gtk.AccelGroup()
		item_factory = gtk.ItemFactory(gtk.MenuBar, "<main>", accel_group)

		item_factory.create_items(self.menu_items)

		window.add_accel_group(accel_group)

		self.item_factory = item_factory

		return item_factory.get_widget("<main>")

	def callback_help(self, widget, data=None):
		webbrowser.open('http://www.gpvdm.com/man/index.html')

	def remove_invalid(self,input_name):
		return input_name.replace (" ", "_")


	def callback_add_page(self,new_filename):
		self.add_page(new_filename)

	def add_page(self,filename):
		name=inp_get_token_value(filename, "#laser_name")
		tab=tab_class(filename)
		self.notebook.addTab(tab,name)

	def switch_page(self,page, page_num, user_param1):
		pageNum = self.notebook.get_current_page()
#		tab = self.notebook.get_nth_page(pageNum)
		self.status_bar.push(self.context_id, "Laser "+str(pageNum))

	def __init__(self):
		QWidgetSavePos.__init__(self,"lasers")


		self.main_vbox = QVBoxLayout()


		self.setFixedSize(900, 500)
		self.setWindowTitle(_("Laser configuration window")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("lasers"))

		toolbar=QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(48, 48))


		self.order_widget=order_widget()
		self.order_widget.new_text=_("New laser")
		self.order_widget.delete_text=_("Delete laser")
		self.order_widget.clone_text=_("Clone laser")
		self.order_widget.rename_text=_("Rename laser")
		self.order_widget.new_dlg_text=_("New laser name:")
		self.order_widget.base_file_name="laser"
		self.order_widget.new_tab_name=_("laser ")
		self.order_widget.clone_dlg_text=_("Clone the current laser to a new laser called:")
		self.order_widget.rename_dlg_text=_("Rename the laser to be called:")
		self.order_widget.delete_dlg_text=_("Should I remove the laser file ")
		self.order_widget.name_token="#laser_name"
		self.order_widget.init()
		toolbar.addWidget(self.order_widget)

		self.order_widget.added.connect(self.callback_add_page)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)


		self.help = QAction(icon_get("help"), _("Help"), self)
		self.help.setStatusTip(_("Close"))
		self.help.triggered.connect(self.callback_help)
		toolbar.addAction(self.help)

		self.main_vbox.addWidget(toolbar)


		self.notebook = QTabWidget()
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

		self.order_widget.notebook_pointer=self.notebook
		self.order_widget.load_tabs()

		self.main_vbox.addWidget(self.notebook)


		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)


		self.setLayout(self.main_vbox)


