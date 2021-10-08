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

## @package error_dlg
#  An error dialog class.
#

#qt
from gui_enable import gui_get

if gui_get()==True:
	from PyQt5.QtWidgets import QMessageBox


def error_dlg(parent,text):
	msgBox = QMessageBox(parent)
	msgBox.setIcon(QMessageBox.Critical)
	msgBox.setText("gpvdm error:")
	msgBox.setInformativeText(text)
	msgBox.setStandardButtons(QMessageBox.Ok )
	msgBox.setDefaultButton(QMessageBox.Ok)
	reply = msgBox.exec_()


