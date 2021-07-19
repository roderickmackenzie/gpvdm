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

## @package tab
#  The main tab class, used to display material properties.
#

import os

from PyQt5.QtWidgets import QTextEdit

from QComboBoxLang import QComboBoxLang
from QColorPicker import QColorPicker
from QComboBoxNewtonSelect import QComboBoxNewtonSelect

from PyQt5.QtCore import QTimer

import i18n
_ = i18n.language.gettext


class QChangeLog(QTextEdit):
	def __init__(self, parent):
		QTextEdit.__init__(self, parent)

