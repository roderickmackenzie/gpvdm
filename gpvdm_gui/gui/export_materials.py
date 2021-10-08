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

## @package export_materials
#  Export the materials database to a zip file.
#

import os
import zipfile
from progress_class import progress_class
from process_events import process_events
from cal_path import remove_cwdfrompath

def export_materials(target):
	if target.endswith(".zip")==False:
		target=target+".zip"

	file_list=[]

	progress_window=progress_class()
	progress_window.show()
	progress_window.start()
	process_events()
	mat_files=["alpha.gmat","cost.xlsx","dos.inp","fit.inp","data.json","n.gmat","pl.inp"]
	for path, dirs, files in os.walk(os.path.join(os.getcwd(),"materials")):
		for file_name in files:
			if file_name in mat_files:
				file_list.append(os.path.join(path,file_name))

	zf = zipfile.ZipFile(target, 'a')

	for i in range(0,len(file_list)):
		cur_file=file_list[i]

		lines=[]
		if os.path.isfile(cur_file):
			f=open(cur_file, mode='rb')
			lines = f.read()
			f.close()


			zf.writestr(remove_cwdfrompath(cur_file), lines)
			progress_window.set_fraction(float(i)/float(len(file_list)))
			progress_window.set_text("Adding"+cur_file[len(os.getcwd()):])
			process_events()

	zf.close()
	progress_window.stop()

