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

## @package dim_editor
#  A window to edit the dimentions of the device.
#

import os
from str2bool import str2bool
from icon_lib import icon_get
from gpvdm_open import gpvdm_open
from cal_path import get_materials_path
from global_objects import global_object_get
from help import help_window

#windows
from gui_util import yes_no_dlg
from error_dlg import error_dlg


#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QIcon,QPalette
from PyQt5.QtWidgets import QWidget, QVBoxLayout,QProgressBar,QLineEdit,QLabel,QDesktopWidget,QToolBar,QHBoxLayout,QAction, QSizePolicy, QTableWidget, QTableWidgetItem,QComboBox,QDialog,QAbstractItemView

from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QWidget

from global_objects import global_object_run

from global_objects import global_isobject
from global_objects import global_object_get

from QComboBoxLang import QComboBoxLang

import i18n
_ = i18n.language.gettext

from gpvdm_select import gpvdm_select

from cal_path import get_sim_path
from cal_path import get_default_material_path
from QWidgetSavePos import QWidgetSavePos

from mesh import get_mesh
from gpvdm_json import gpvdm_data

class dim_editor(QWidgetSavePos):

	def __init__(self):
		QWidgetSavePos.__init__(self,"dim_editor")

		self.setWindowTitle(_("xz dimension editor")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("dimensions"))
		self.resize(400,200)

		self.cost_window=False

		self.main_vbox=QVBoxLayout()

		self.toolbar=QToolBar()
		self.toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.toolbar.setIconSize(QSize(42, 42))

		spacer = QWidget()


		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.toolbar.addWidget(spacer)

		self.help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		self.toolbar.addAction(self.help)

		self.main_vbox.addWidget(self.toolbar)

		self.widget0 = QWidget()
		self.widget0_hbox=QHBoxLayout()
		self.widget0.setLayout(self.widget0_hbox)

		self.widget0_label=QLabel("x size")
		self.widget0_hbox.addWidget(self.widget0_label)

		self.widget0_edit=QLineEdit()
		self.widget0_edit.setText(str(get_mesh().x.get_len()))
		self.widget0_edit.textChanged.connect(self.apply)
		self.widget0_hbox.addWidget(self.widget0_edit)
		self.widget0_label=QLabel("m")
		self.widget0_hbox.addWidget(self.widget0_label)

		self.main_vbox.addWidget(self.widget0)

		self.widget1 = QWidget()
		self.widget1_hbox=QHBoxLayout()
		self.widget1.setLayout(self.widget1_hbox)
		self.widget1_label=QLabel("z size")
		self.widget1_hbox.addWidget(self.widget1_label)
		self.widget1_edit=QLineEdit()
		self.widget1_edit.setText(str(get_mesh().z.get_len()))
		self.widget1_edit.textChanged.connect(self.apply)
		self.widget1_hbox.addWidget(self.widget1_edit)
		self.widget1_label=QLabel("m")
		self.widget1_hbox.addWidget(self.widget1_label)
		self.main_vbox.addWidget(self.widget1)


		#self.tab.itemSelectionChanged.connect(self.callback_tab_selection_changed)


		self.setLayout(self.main_vbox)

		#self.tab.itemSelectionChanged.connect(self.layer_selection_changed)


	def apply(self):
		data=gpvdm_data()
		try:
			val=float(self.widget0_edit.text())
			if val<=0:
				return
		except:
			return
		get_mesh().x.set_len(val)

		try:
			val=float(self.widget1_edit.text())
			if val<=0:
				return

		except:
			return

		get_mesh().z.set_len(val)

		data.save()

		global_object_run("mesh_update")
		global_object_run("gl_force_redraw")



