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

## @package qe
#  The quantum efficiency window
#


import os
import i18n
_ = i18n.language.gettext
from experiment import experiment
from tab_eqe import tab_eqe

class window_eqe(experiment):


	def __init__(self):
		experiment.__init__(self,"tab_eqe",window_save_name="eqe", window_title=_("Quantum efficiency editor"),json_search_path="gpvdm_data().eqe",icon="qe")

		#w=self.ribbon_simulation()
		#self.ribbon.addTab(w,_("Simulation"))

		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)



		
