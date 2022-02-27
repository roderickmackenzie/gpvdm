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
from PyQt5.QtWidgets import QWidget, QVBoxLayout,QProgressBar,QLineEdit,QLabel,QToolBar,QHBoxLayout,QAction, QSizePolicy, QTableWidget, QTableWidgetItem,QComboBox,QDialog, QTabWidget

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

from gpvdm_json import gpvdm_data
from tab import tab_class
from help import QAction_help

class dim_editor(QWidgetSavePos):

	def __init__(self):
		#print("<<<<>>>>>>>")
		QWidgetSavePos.__init__(self,"dim_editor")

		self.setWindowTitle(_("Dimension editor")+" https://www.gpvdm.com")
		self.setWindowIcon(icon_get("dimensions"))
		self.setMinimumSize(600, 200)


		self.main_vbox=QVBoxLayout()

		self.toolbar=QToolBar()
		self.toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.toolbar.setIconSize(QSize(42, 42))

		spacer = QWidget()


		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.toolbar.addWidget(spacer)

		self.help = QAction_help()
		self.toolbar.addAction(self.help)

		#self.main_vbox.addWidget(self.toolbar)

		#notebook
		self.notebook = QTabWidget()

		#device page
		self.xz_widget=QWidget()
		self.xz_vbox=QVBoxLayout()
		self.widget0 = QWidget()
		self.widget0_hbox=QHBoxLayout()
		self.widget0.setLayout(self.widget0_hbox)

		self.widget0_label=QLabel("x size")
		self.widget0_hbox.addWidget(self.widget0_label)

		self.widget0_edit=QLineEdit()
		self.widget0_edit.setText(str(gpvdm_data().mesh.mesh_x.get_len()))
		self.widget0_edit.textChanged.connect(self.apply)
		self.widget0_hbox.addWidget(self.widget0_edit)
		self.widget0_label=QLabel("m")
		self.widget0_hbox.addWidget(self.widget0_label)

		self.xz_vbox.addWidget(self.widget0)

		self.widget1 = QWidget()
		self.widget1_hbox=QHBoxLayout()
		self.widget1.setLayout(self.widget1_hbox)
		self.widget1_label=QLabel("z size")
		self.widget1_hbox.addWidget(self.widget1_label)
		self.widget1_edit=QLineEdit()
		self.widget1_edit.setText(str(gpvdm_data().mesh.mesh_z.get_len()))
		self.widget1_edit.textChanged.connect(self.apply)
		self.widget1_hbox.addWidget(self.widget1_edit)
		self.widget1_label=QLabel("m")
		self.widget1_hbox.addWidget(self.widget1_label)
		self.xz_vbox.addWidget(self.widget1)

		self.xz_widget.setLayout(self.xz_vbox)
		self.notebook.addTab(self.xz_widget,_("Substrate xz size"))

		#World size
		self.world_widget=tab_class(gpvdm_data().world.config)
		self.notebook.addTab(self.world_widget,_("World size"))
		self.world_widget.changed.connect(self.callback_refresh_model)
		self.main_vbox.addWidget(self.notebook)
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
		data.mesh.mesh_x.set_len(val)

		try:
			val=float(self.widget1_edit.text())
			if val<=0:
				return

		except:
			return

		data.mesh.mesh_z.set_len(val)

		data.save()
		global_object_run("mesh_update")
		self.callback_refresh_model()

	def callback_refresh_model(self):
		global_object_run("gl_do_rescale")
		global_object_run("gl_force_redraw")

