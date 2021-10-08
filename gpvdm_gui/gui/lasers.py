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

## @package lasers
#  Main laser editor window.
#

import os
import webbrowser
from tab import tab_class
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

from experiment import experiment

class lasers(experiment):


	def __init__(self,data=None):
		experiment.__init__(self,window_save_name="laser_editor", window_title=_("Laser editor"),name_of_tab_class="jvexperiment_tab",json_search_path="gpvdm_data().lasers")
		self.notebook.currentChanged.connect(self.switch_page)
		#self.ribbon.tb_save.setVisible(False)
		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)


