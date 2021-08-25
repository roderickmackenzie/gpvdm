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

## @package detectors_editor
#  Main window for settign up detectors
#

import i18n
_ = i18n.language.gettext

from experiment import experiment

class detectors_editor(experiment):


	def __init__(self,data=None):
		experiment.__init__(self,window_save_name="detectors_editor", window_title=_("Optical detectors editor"),name_of_tab_class="detectors_tab",json_search_path="gpvdm_data().detectors")

		self.base_json_obj="from json_detectors import json_detector"
		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)


