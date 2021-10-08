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


## @package export_archive
#  Export the simulation to an archive.
#

#import sys
import os
#import glob
import zipfile
#from util_zip import archive_add_file
from progress_class import progress_class
from process_events import process_events
from cal_path import remove_simpathfrompath

from cal_path import get_sim_path

def export_archive(target,everything):
	if target.endswith(".gpvdm")==False:
		target=target+".gpvdm"

	file_list=[]

	progress_window=progress_class()
	progress_window.show()
	progress_window.start()
	process_events()

	if everything==True:
		for path, dirs, files in os.walk(get_sim_path()):
			for file_name in files:
				if file_name.endswith(".inp") or file_name.endswith(".dat") or file_name.endswith(".gmat"):
					file_list.append(os.path.join(path,file_name))
	else:
		files=os.listdir(get_sim_path())
		for file_name in files:
			if file_name.endswith(".inp"):
				file_list.append(os.path.join(get_sim_path(),file_name))

	zf = zipfile.ZipFile(target, 'a')

	for i in range(0,len(file_list)):
		cur_file=file_list[i]

		lines=[]
		if os.path.isfile(cur_file):
			try:
				f=open(cur_file, mode='rb')
				lines = f.read()
				f.close()


				zf.writestr(remove_simpathfrompath(cur_file), lines)
			except:
				print(_("Can't open file ")+cur_file)

			progress_window.set_fraction(float(i)/float(len(file_list)))
			progress_window.set_text("Adding"+cur_file[len(get_sim_path()):])
			process_events()

	src_zf = zipfile.ZipFile(os.path.join(get_sim_path(),"sim.gpvdm"), 'r')

	for file_name in src_zf.namelist():
		if file_name not in zf.namelist():
			#print "adding from archive",file_name
			lines=src_zf.read(file_name)
			zf.writestr(file_name, lines)

	zf.close()
	src_zf.close()
	progress_window.stop()

