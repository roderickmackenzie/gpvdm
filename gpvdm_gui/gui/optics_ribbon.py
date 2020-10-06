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

## @package optics_ribbon
#  The ribbon for the optics window
#


import os

from dump_io import dump_io

from code_ctrl import enable_betafeatures
from cal_path import get_css_path

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt,QFile,QIODevice
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox, QLineEdit, QToolButton, QMenu
from PyQt5.QtWidgets import QTabWidget

from icon_lib import icon_get

from about import about_dlg

from mode_selector import mode_selector
from tb_spectrum import tb_spectrum

from util import wrap_text
from ribbon_base import ribbon_base
from play import play
from QAction_lock import QAction_lock
from inp import inp
from cal_path import get_sim_path
from gui_util import dlg_get_text
from generation_rate_editor import generation_rate_editor
from tab import tab_class
from str2bool import str2bool

class mode_button(QAction_lock):
	def __init__(self,image,text,s,name):
		QAction_lock.__init__(self,image,text,s,name)
		self.setCheckable(True)
		self.mode="full"

class ribbon_optical_models(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)
		self.actions=[]
		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))
		
		a=mode_button("transfer_matrix", _("Transfer\nmatrix"), self,"optics_ribbon_export_image")
		a.mode="full"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("exp_light", _("Exponential\nprofile"), self,"optics_ribbon_mode_exp")
		a.mode="exp"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("flat_light", _("Flat\nprofile"), self,"optics_ribbon_mode_flat")
		a.mode="flat"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("file_light", _("From\nfile"), self,"optics_ribbon_mode_from_file")
		a.mode="fromfile"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("qe_light", _("From QE\nspectra"), self,"optics_ribbon_mode_qe")
		a.mode="qe"
		a.clicked.connect(self.callback_click)
		self.actions.append(a)

		a=mode_button("constant_light", _("Constant\nvalue"), self,"optics_ribbon_mode_constant")
		a.mode="constant"
		a.clicked.connect(self.callback_click)

		self.menu_set_constant_value = QMenu(self)
		#self.populate_used_file_menu()
		a.setMenu(self.menu_set_constant_value)

		f=QAction(_("Edit constant"), self)
		f.triggered.connect(self.callback_edit_constant)
		self.menu_set_constant_value.addAction(f)


		self.actions.append(a)

		for a in self.actions:
			self.addAction(a)

		self.set_mode()

	def callback_edit_constant(self):
		self.generation_rate_editor=generation_rate_editor()
		self.generation_rate_editor.show()


	def set_mode(self):
		self.blockSignals(True)
		f=inp()
		f.load(os.path.join(get_sim_path(),"light.inp"))
		used_model=f.get_token("#light_model")
		for a in self.actions:
			a.setChecked(False)
			if a.mode==used_model:
				a.setChecked(True)
				break

		self.blockSignals(False)

	def callback_click(self,w):
		self.blockSignals(True)
		for a in self.actions:
			a.setChecked(False)

		w.setChecked(True)
		f=inp()
		f.load(os.path.join(get_sim_path(),"light.inp"))
		used_model=f.set_token("#light_model",w.mode)
		f.save()
		#inp_update_token_value(os.path.join(get_sim_path(),"light.inp"), "#light_model",w.mode)
		self.blockSignals(False)


class ribbon_optics_files(QToolBar):
	def __init__(self):
		QToolBar.__init__(self)

		self.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.setIconSize(QSize(42, 42))
		
		self.run = play(self,"optics_ribbon_run",run_text=wrap_text(_("Run optical simulation"),5))
		self.addAction(self.run)

		self.fx_box=mode_selector()
		self.fx_box.show_all=True
		self.fx_box.update()
		self.addWidget(self.fx_box)
		
		self.spectrum=tb_spectrum()
		self.addWidget(self.spectrum)

		self.configwindow = QAction(icon_get("preferences-system"), _("Configure"), self)
		self.addAction(self.configwindow)

		self.optical_filter = QAction(icon_get("optical_filter"), _("Optical\nFilter"), self)
		self.optical_filter.setCheckable(True)
		self.optical_filter.triggered.connect(self.callback_filter_clicked)

		self.menu_optical_filter = QMenu(self)
		self.optical_filter.setMenu(self.menu_optical_filter)

		self.filter_edit=QAction(_("Edit"), self)
		self.filter_edit.triggered.connect(self.callback_filter_window)
		self.menu_optical_filter.addAction(self.filter_edit)
		self.addAction(self.optical_filter)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.addWidget(spacer)


		self.help = QAction(icon_get("help"), _("Help"), self)
		self.addAction(self.help)

	def callback_filter_clicked(self):
		f=inp()
		f.load(os.path.join(get_sim_path(),"filter.inp"))
		enabled=f.set_token("#filter_enabled",str(self.optical_filter.isChecked()))
		f.save()

	def callback_filter_window(self):
		widget=tab_class(os.path.join(get_sim_path(),"filter.inp"))
		widget.setWindowIcon(icon_get("filter_wheel"))

		widget.setWindowTitle(_("Filter editor")+" (https://www.gpvdm.com)")    

		widget.show()



class optics_ribbon(ribbon_base):

	def export_data(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))
		
		self.tb_save = QAction_lock("export_image", _("Save graph"), self,"optics_ribbon_export_image")
		toolbar.addAction(self.tb_save)

		return toolbar

	def update(self):
		self.blockSignals(True)

		self.optics.fx_box.update()

		f=inp()
		f.load(os.path.join(get_sim_path(),"filter.inp"))
		enabled=f.get_token("#filter_enabled")
		self.optics.optical_filter.setChecked(str2bool(enabled))

		self.blockSignals(False)


	def callback_about_dialog(self):
		dlg=about_dlg()
		dlg.exec_()

	def __init__(self):
		QTabWidget.__init__(self)
		#self.setStyleSheet("QWidget {	background-color:cyan; }")

		self.about = QToolButton(self)
		self.about.setText(_("About"))
		self.about.pressed.connect(self.callback_about_dialog)

		self.setCornerWidget(self.about)

		self.optics=ribbon_optics_files()
		self.addTab(self.optics,_("Optics"))

		w=ribbon_optical_models()
		self.addTab(w,_("Optical models"))

		w=self.export_data()
		self.addTab(w,_("Export data"))

		sheet=self.readStyleSheet(os.path.join(get_css_path(),"style.css"))
		if sheet!=None:
			sheet=str(sheet,'utf-8')
			self.setStyleSheet(sheet)

		self.update()
