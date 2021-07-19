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

## @package backup
#  Backup a simulation
#
#import sys
import os
import shutil
#import glob
from util_zip import read_lines_from_archive
from util_zip import write_lines_to_archive
from shutil import copyfile
from inp import inp_search_token_value

import datetime

def backup(dest,src,notes=""):

	if os.path.isdir(dest)==False:
		os.makedirs(dest)

	lines=[]
	lines.append("#gpvdm_file_type")
	lines.append("backup")
	lines.append("#date")
	lines.append(str(datetime.datetime.now()))
	lines.append("#notes")
	lines.append(notes)
	lines.append("#end")

	write_lines_to_archive(os.path.join(dest,"sim.gpvdm"),"mat.inp",lines,mode="l",dest="file")

	for f in os.listdir(src):
		if f.endswith(".inp") or f=="sim.gpvdm":
			src_file=os.path.join(src,f)
			dst_file=os.path.join(dest,f)
			copyfile(src_file,dst_file)

def backup_restore(dest,src):

	for f in os.listdir(dest):
		if f.endswith(".inp") or f=="sim.gpvdm":
			os.remove(os.path.join(dest,f))

	for f in os.listdir(src):
		if f.endswith(".inp") or f=="sim.gpvdm":
			if f!="mat.inp":
				src_file=os.path.join(src,f)
				dst_file=os.path.join(dest,f)
				copyfile(src_file,dst_file)


