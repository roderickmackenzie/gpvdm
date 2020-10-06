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


## @package tb_lasers
#  Toolbar item to select the type of laser a user wants.
#


#inp
from inp import inp_update_token_value
from inp import inp_get_token_value
from inp import inp_load_file
from inp import inp_lsdir
from inp import inp_search_token_value

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtWidgets import  QTextEdit
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QLabel,QComboBox


class tb_lasers(QWidget):

	def update(self,config_file):
		self.config_file=config_file
		self.sim_mode.blockSignals(True)
		self.sim_mode.clear()
		lines=[]

		files=inp_lsdir("sim.gpvdm")
		if files!=False:
			for i in range(0,len(files)):
				if files[i].endswith(".inp"):
					lines=inp_load_file(files[i])
					value=inp_search_token_value(lines, "#laser_name")
					if value!=False:
						value=value.rstrip()
						self.sim_mode.addItem(value)

			token=inp_get_token_value(self.config_file, "#pump_laser")

			all_items  = [self.sim_mode.itemText(i) for i in range(self.sim_mode.count())]

			for i in range(0,len(all_items)):
				if all_items[i] == token:
					self.sim_mode.setCurrentIndex(i)
					found=True

		self.sim_mode.blockSignals(False)

	def __init__(self):

		QWidget.__init__(self)
		layout=QVBoxLayout()
		label=QLabel()
		label.setText(_("Laser:"))
		layout.addWidget(label)

		self.sim_mode = QComboBox(self)
		self.sim_mode.setEditable(True)

		self.sim_mode.currentIndexChanged.connect(self.call_back_sim_mode_changed)

		layout.addWidget(self.sim_mode)

		self.setLayout(layout)

		return

	def call_back_sim_mode_changed(self):
		mode=self.sim_mode.currentText()
		inp_update_token_value(self.config_file, "#pump_laser", mode)


