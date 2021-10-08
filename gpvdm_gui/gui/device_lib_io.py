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

## @package device_lib_io
#  Make changes to the device lib enmass.
#

import os
import glob
from cal_path import get_device_lib_path
from cal_path import get_sim_path
from util_zip import archive_add_file
from util_zip import zip_remove_file
from inp import inp_update_token_value
from inp import inp_get_token_value
from inp import inp_load_file
from inp import inp_save
from inp import inp

from util_zip import zip_lsdir
from util_zip import archive_copy_file
from yes_no_cancel_dlg import yes_no_cancel_dlg
from cal_path import get_backup_path

class col:
    HEADER = '\033[95m'
    blue = '\033[94m'
    green = '\033[92m'
    WARNING = '\033[93m'
    red = '\033[91m'
    black = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def device_lib_check(file_to_check=""):
	from import_archive import get_file_info
	src_archive=os.path.join(get_sim_path(),"sim.gpvdm")
	files=zip_lsdir(src_archive)
	archives=find_device_libs()

	for file_name in files:			#scan local sim.gpvdm
		if file_name.endswith(".inp")==True and (file_name==file_to_check or file_to_check==""):
			print(file_name)
			for archive_file in archives:
				f_orig=inp()
				f_orig.load(file_name)
				print("Report for",archive_file,file_name)
				sim_file_name=os.path.join(os.path.dirname(archive_file),file_name)
				archive=os.path.basename(archive_file)
				f_archive=inp()
				f_archive.load(sim_file_name, archive=archive_file)
				if f_archive.lines!=False:
					if f_archive.lines[0]!="#sections" and file_name!="dump_file.inp":
						delta=f_orig.delta_tokens(f_archive)
						if len(delta)==0:
							print(col.green+"OK"+col.black)
						else:
							print(col.red+"Delta found"+col.black)
							print(col.red+"local:"+col.black,f_orig)
							print(col.blue+"archive:"+col.black,f_archive)


							f_orig.import_tokens(f_archive)
							print("change to:",f_orig)
							print(delta)
							response = yes_no_cancel_dlg(None,"Should I repair the file"+f_archive.get_file_name()+"\n")
							if response == "yes":
								f_archive.lines=f_orig.lines
								f_archive.save()
				else:
					print(os.path.basename(sim_file_name))
					info=get_file_info(os.path.basename(sim_file_name))
					if info.needed==True:
						print(col.red+"Not found"+col.black)
						response = yes_no_cancel_dlg(None,"Should I copy the origonal file in?")

						print(response)
						if response == "yes":
							archive_copy_file(archive_file,file_name,src_archive,file_name)


def find_device_libs():
	out=[]
	for root, dirs, files in os.walk(get_device_lib_path()):
		for name in files:
			if name.endswith(".gpvdm")==True:
				out.append(os.path.join(root, name))

	for root, dirs, files in os.walk(get_backup_path()):
		for name in files:
			if name.endswith(".gpvdm")==True:
				out.append(os.path.join(root, name))

	return out

def device_lib_replace(file_name,dir_name=""):
	if dir_name=="":
		dir_name=get_device_lib_path()
	archives=glob.glob(os.path.join(dir_name,"*.gpvdm"))
	for i in range(0,len(archives)):
		print("replace ",archives[i],file_name)
		archive_add_file(archives[i],file_name,"")

def device_lib_delete(file_name):
	archives=find_device_libs()
	for archive in archives:
		zip_remove_file(archive,file_name)

def device_lib_token_change(file_name,token,value):
	archives=find_device_libs()
	for arch in archives:
		#print(arch)
		sim_file_name=os.path.join(arch,file_name)
		f=inp().load(sim_file_name)
		if f!=False:
			#print(">>",f)
			f.replace(token,value)
			f.save()

def device_lib_token_token_set_if_eq(file_name,token,test_value,new_value):
	archives=find_device_libs()
	for arch in archives:
		f=inp().load(os.path.join(arch,file_name))
		old_val=f.get_token(token)
		#print(old_val,type(old_val))
		if old_val!=False:
			if old_val.lower()==test_value.lower():
				#print(old_val,test_value)
				f.replace(token,new_value)
				f.save()


def device_lib_token_repair():
	archives=find_device_libs()

	src_archives=[]
	for root, dirs, files in os.walk("./device_lib/"):
		for name in files:
			if name.endswith(".gpvdm")==True:
				src_archives.append(os.path.join(root, name))


	for i in range(0,len(archives)):
		path_to_epitaxy=os.path.join(archives[i],"epitaxy.inp")
		print(path_to_epitaxy)
		f=inp()
		f.load(path_to_epitaxy)
		f.to_sections(start="#layer_type")

		for s in f.sections:
			shape=inp()
			shape.load(os.path.join(archives[i],s.layer_base_shape+".inp"))
			dos_file=s.layer_dos_file
			if dos_file.startswith("dos")==False:
				dos_file="none"
			shape.replace("#shape_dos",dos_file)
			shape.replace("#shape_homo_file",s.layer_homo)
			shape.replace("#shape_lumo_file",s.layer_lumo)
			shape.replace("#shape_pl_file",s.layer_pl_file)
			shape.save()


def device_lib_fix_ver(file_name, ver ):
	archives=find_device_libs()

	for i in range(0,len(archives)):
		src_file=os.path.join(os.path.dirname(archives[i]),file_name)
		archive=os.path.basename(archives[i])
		value=inp_get_token_value(src_file, "#ver",archive=archives[i])
		if value!=None and value != ver:
			inp_update_token_value(src_file, "#ver", ver,archive=archives[i])
		print(value)
		if value==None:
			lines=inp_load_file(src_file,archive=archives[i])

			for ii in range(0,len(lines)):
				if lines[ii]=="#end":
					lines[ii]="#ver"
					lines[ii+1]=ver

					lines.append("#end")
			print(lines)
			inp_save(src_file,lines,archive=archives[i])

