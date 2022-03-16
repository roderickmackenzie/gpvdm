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

## @package jv_experiment
#  JV experiment editor
#

import i18n
_ = i18n.language.gettext
from jvexperiment_tab import jvexperiment_tab
from experiment import experiment

class jv_experiment(experiment):


	def __init__(self,data=None):
		experiment.__init__(self,"jvexperiment_tab",window_save_name="jvexperiment_editor", window_title=_("JV experiment window"),json_search_path="gpvdm_data().jv")


		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()


