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

## @package backup
#  Backup a simulation
#
#import sys
import os
import shutil
#import glob
from shutil import copyfile
import datetime
from json_base import json_base

def backup(dest,src,notes=""):

	if os.path.isdir(dest)==False:
		os.makedirs(dest)

	data=json_base("backup")
	data.include_name=False
	data.var_list.append(["icon","backup"])
	data.var_list.append(["item_type","backup"])
	data.var_list.append(["hidden","False"])
	data.var_list.append(["date","False"])

	data.var_list_build()
	data.save_as(os.path.join(dest,"data.json"))

	for f in os.listdir(src):
		if f.endswith(".inp") or f=="sim.gpvdm" or f=="sim.json":
			src_file=os.path.join(src,f)
			dst_file=os.path.join(dest,f)
			copyfile(src_file,dst_file)

def backup_restore(dest,src):

	for f in os.listdir(dest):
		if f.endswith(".inp") or f=="sim.gpvdm" or f=="sim.json":
			os.remove(os.path.join(dest,f))

	for f in os.listdir(src):
		if f.endswith(".inp") or f=="sim.gpvdm" or f=="sim.json":
			if f!="data.json":
				src_file=os.path.join(src,f)
				dst_file=os.path.join(dest,f)
				copyfile(src_file,dst_file)


