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

## @package dlg_export
#  A window to export the simulation in some form, a .gpvdm file, a .zip file or a jpg file.
#

import os
from plot_io import get_plot_file_info
from help import my_help_class
from export_as import export_as
from export_archive import export_archive
from export_materials import export_materials

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QMessageBox

import i18n
_ = i18n.language.gettext

from util import set_file_ext
from cal_path import get_sim_path

def dlg_export(parent):

	dialog = QFileDialog(parent)
	dialog.setWindowTitle(_("Export the simulation as"))
	dialog.setAcceptMode(QFileDialog.AcceptSave)
	types=[]
	types.append(_("gpvdm archive input+output files")+" (*.gpvdm)")
	types.append(_("gpvdm archive input files")+" (*.gpvdm)")
	types.append(_("optical materials database")+" (*.zip)")
	types.append(_("pdf file")+" (*.pdf)")
	types.append(_("jpg image")+" (*.jpg)")
	types.append(_("tex file")+" (*.tex)")

	dialog.setNameFilters(types)
	dialog.setFileMode(QFileDialog.ExistingFile)
	dialog.setAcceptMode(QFileDialog.AcceptSave)

	if dialog.exec_() == QDialog.Accepted:
		file_name = dialog.selectedFiles()[0]
		#print(dialog.selectedNameFilter())
		if dialog.selectedNameFilter()==_("gpvdm archive input+output files")+" (*.gpvdm)":
			export_archive(file_name,True)
		elif dialog.selectedNameFilter()==_("gpvdm archive input files")+" (*.gpvdm)":
			export_archive(file_name,False)
		elif dialog.selectedNameFilter()==_("optical materials database")+" (*.zip)":
			export_materials(file_name)
		elif dialog.selectedNameFilter()==_("pdf file")+" (*.pdf)" or dialog.selectedNameFilter()==_("jpg image")+" (*.jpg)" or dialog.selectedNameFilter()==_("tex file")+" (*.tex)":
			export_as(file_name)

