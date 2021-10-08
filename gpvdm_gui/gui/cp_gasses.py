#!/usr/bin/env python3
# -*- coding: utf-8 -*-
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

## @package cp_gasses
#  Copy the gasses
#

import os
import os.path
import argparse
import shutil

def copy_gasses(dest,src):
	gas_dir_src=os.path.join(src,"gasses")
	gas_dir_dest=os.path.join(dest,"gasses")

	if os.path.isdir(gas_dir_src)==True:
		if not os.path.exists(gas_dir_dest):
			os.makedirs(gas_dir_dest)
		for f in os.listdir(gas_dir_src):
			shutil.copyfile(os.path.join(gas_dir_src,f),os.path.join(gas_dir_dest,f))
			
