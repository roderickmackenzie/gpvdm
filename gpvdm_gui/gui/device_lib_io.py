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
from inp import inp_delete_token
from inp import inp_insert_token
from inp import inp_insert_duplicate
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

def device_lib_mul_shunt_by_area():
	archives=find_device_libs()

	for archive_file in archives:
		print(archive_file)
		f=inp()
		f.load(os.path.join(archive_file,"mesh_x.inp"))
		x=float(f.get_token("#mesh_layer_length0"))

		f=inp()
		f.load(os.path.join(archive_file,"mesh_z.inp"))
		z=float(f.get_token("#mesh_layer_length0"))

		f=inp()
		f.load(os.path.join(archive_file,"parasitic.inp"))
		R=float(f.get_token("#Rshunt"))
		f.replace("#Rshunt",str(round(R*x*z,2)))
		f.save()
		print(f)
		print(R*x*z)

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
#					elif response == "no":
#						return "no"
#					elif response == "cancel":
#						return "cancel"
					#
			#break

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

def device_lib_token_rename(file_name,old_token,new_token):
	archives=find_device_libs()
	for arch in archives:
		sim_file_name=os.path.join(arch,file_name)
		f=inp().load(sim_file_name)
		if f!=False:
			print(sim_file_name)
			f.replace_token_name(old_token,new_token)	
			f.save()


def device_lib_token_delete(file_name,token):
	archives=find_device_libs()
	for i in range(0,len(archives)):
		sim_file_name=os.path.join(os.path.dirname(archives[i]),file_name)
		archive=os.path.basename(archives[i])
		print(sim_file_name,archive)
		inp_delete_token(sim_file_name, token, archive=archives[i])

def device_lib_token_delete_if_eq(file_name,token,value):
	archives=find_device_libs()
	for i in range(0,len(archives)):
		sim_file_name=os.path.join(os.path.dirname(archives[i]),file_name)
		archive=os.path.basename(archives[i])
		f=inp()
		f.load(sim_file_name, archive=archives[i])
		
		file_value=inp_get_token_value(sim_file_name, token,archive=archives[i])
		if value==f.get_token(token):
			f.delete_token(token,times=1)
			f.save()

def device_lib_token_insert(file_name,token_to_insert_after,token,value):
	archives=find_device_libs()
	for i in range(0,len(archives)):
		sim_file_name=os.path.join(os.path.dirname(archives[i]),file_name)
		archive=os.path.basename(archives[i])
		print(sim_file_name,archive)
		inp_insert_token(sim_file_name, token_to_insert_after, token, value, archive=archives[i])

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

def device_lib_epitaxy_force_shapes():
	archives=find_device_libs()
	for arch in archives:
		f=inp().load(os.path.join(arch,"epitaxy.inp"))
		for i in range(0,len(f.lines)):
			if f.lines[i].startswith("#layer_base_shape"):
				if f.lines[i+1]=="none":
					new_shape_file=f.next_sequential_file("shape")
					print(arch,"oh",new_shape_file,f.lines[i])
					f.lines[i+1]=new_shape_file[:-4]
					f.save()
					archive_copy_file(arch,new_shape_file,os.path.join(get_sim_path(),"sim.gpvdm"),"shape0.inp")

def device_lib_set_color():
	archives=find_device_libs()
	for arch in archives:
		files=zip_lsdir(arch)
		for file_name in files:
			if file_name.startswith("shape")==True:
				f=inp().load(os.path.join(arch,file_name))
				f.replace("#red_green_blue",["0.0","0.8","0.0"])
				print(f.lines)
				f.save()
def device_lib_epitaxy_token_to_shape(epitaxy_token,shape_token):
	archives=find_device_libs()
	for arch in archives:
		f=inp().load(os.path.join(arch,"epitaxy.inp"))
		for i in range(0,len(f.lines)):
			if f.lines[i].startswith(epitaxy_token):
				post_fix=f.lines[i][len(epitaxy_token):]
				shape_file_token="#layer_base_shape"+post_fix
				shape_file=f.get_token(shape_file_token)
				val=f.lines[i+1]
				sf=inp().load(os.path.join(arch,shape_file+".inp"))
				sf.replace(shape_token,val)
				sf.save()
				#print(sf)
				#print(val,shape_file)

def device_lib_token_insert_top(file_name,token,value):
	archives=find_device_libs()
	for i in range(0,len(archives)):
		sim_file_name=os.path.join(os.path.dirname(archives[i]),file_name)
		archive=os.path.basename(archives[i])
		print(sim_file_name,archive)
		inp_insert_token(sim_file_name, False, token, value, archive=archives[i])

def device_lib_token_duplicate(dest_file, dest_token, src_file, src_token):
	archives=find_device_libs()
	for i in range(0,len(archives)):
		path_to_src_file=os.path.join(os.path.dirname(archives[i]),src_file)
		path_to_dest_file=os.path.join(os.path.dirname(archives[i]),dest_file)

		archive=os.path.basename(archives[i])
		print(path_to_src_file,path_to_dest_file)
		inp_insert_duplicate(path_to_dest_file, dest_token, path_to_src_file, src_token,archive=archives[i])

def device_lib_token_repair(dest_file, dest_token_after, src_file, src_token):
	archives=find_device_libs()

	src_archives=[]
	for root, dirs, files in os.walk("/home/rod/t/gpvdm5.3/device_lib/"):
		for name in files:
			if name.endswith(".gpvdm")==True:
				src_archives.append(os.path.join(root, name))


	print(src_archives)

	for i in range(0,len(archives)):
		path_to_src_file=os.path.join(os.path.dirname(src_archives[i]),src_file)
		path_to_dest_file=os.path.join(os.path.dirname(archives[i]),dest_file)

		print(src_archives[i],archives[i])

		archive_src=os.path.basename(src_archives[i])
		archive=os.path.basename(archives[i])
		value=inp_get_token_value(src_file, src_token,archive=src_archives[i])
		if value!=None:
			inp_insert_token(dest_file, dest_token_after, src_token, value, archive=archives[i])


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


