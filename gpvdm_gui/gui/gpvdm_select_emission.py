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


## @package gpvdm_select_emission
#  A widget for the tab widget which allows the user to select material files.
#


import os
from cal_path import get_emission_path
from gpvdm_select_from_db import gpvdm_select_from_db

import i18n
_ = i18n.language.gettext


class gpvdm_select_emission(gpvdm_select_from_db):

	def __init__(self,file_box=True):
		gpvdm_select_from_db.__init__(self,get_emission_path(),file_box=file_box)
		
