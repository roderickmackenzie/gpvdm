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

## @package util_zip
#  Functions to manipulate zip files/.gpvdm files
#

import os
import shutil
from tempfile import mkstemp
import zipfile

from cal_path import subtract_paths
import time
import glob
import hashlib
from update_file_info import update_file_info
from libcrypt import libcrypt_decrypt
from safe_delete import gpvdm_delete_file

## Copy a file from one archive to another.
def archive_copy_file(dest_archive,dest_file_name,src_archive,file_name,dest="archive"):
	lines=read_lines_from_archive(src_archive,file_name)
	if lines==False:
		return False

	write_lines_to_archive(dest_archive,dest_file_name,lines,dest=dest)
	return True



## Make an empty archive
def archive_make_empty(archive_path):
		zf = zipfile.ZipFile(archive_path, 'w',zipfile.ZIP_DEFLATED)
		zf.close()

## List the content of an archive 
def zip_lsdir(file_name,zf=None,sub_dir=None):
	"""Input: path to a .gpvdm file"""
	my_list=[]
	my_dir=os.path.dirname(file_name)

	if my_dir=="":
		my_dir=os.getcwd()
		file_name=os.path.join(os.getcwd(),file_name)

	if os.path.isdir(my_dir):
		my_list=os.listdir(my_dir)
	else:
		return False

	if os.path.isfile(file_name):
		do_close=False
		if zf==None:
			do_close=True
			zf = zipfile.ZipFile(file_name, 'r')

		items=zf.namelist()
		items.extend(my_list)

		#print(items)
		my_list=list(set(items))

		#print(my_list)
		#print(my_list[0])
		#asdsa


		if sub_dir!=None:
			l=[]
			level=sub_dir.count("/")
			for i in range(0,len(my_list)):
				archive_file=my_list[i]

				if archive_file.startswith(sub_dir)==True or sub_dir=="/":
					s=archive_file.split("/")
					if len(s)>level:
						s=s[0:level]
						l.append("/".join(s))

			my_list=list(set(l))

		if do_close==True:
			zf.close()

		my_list=sorted(my_list)
		return my_list

	return my_list

def zip_get_raw_data(file_name,bytes=None):
	found=False
	lines=b""

	zip_file=os.path.join(os.path.dirname(file_name),"sim.gpvdm")

	name=os.path.basename(file_name)
	if os.path.isfile(file_name)==True:
		f = open(file_name, mode='rb')
		if bytes==None:
			lines = f.read()
		else:
			lines = f.read(bytes)
		f.close()
		found=True

	if os.path.isfile(zip_file) and found==False:
		zf = zipfile.ZipFile(zip_file, 'r')
		items=zf.namelist()
		if items.count(name)>0:
			lines = zf.read(name)
			found=True
		zf.close()

	if found==False:
		return False

	lines=libcrypt_decrypt(lines)
	return lines

## Read lines from a file in an archive
def zip_get_data_file(file_name):
	lines=b""
	found=True
	lines=zip_get_raw_data(file_name)

	if lines==False:
		found=False

	try:
		lines=lines.decode('utf-8')
		lines=[str.strip() for str in lines.splitlines()]#lines.split("\n")
	except:
		lines=[]
		
	return [found,lines]

## Replace a file in an archive.
def replace_file_in_zip_archive(zip_file_name,target,lines,delete_first=True):
	if os.path.isfile(zip_file_name)==True:
		if delete_first==True:
			zip_remove_file(zip_file_name,target)

		zf = zipfile.ZipFile(zip_file_name, 'a',zipfile.ZIP_DEFLATED)

		if type(lines)==list:
			build='\n'.join(lines)
		else:
			build=lines

		zf.writestr(target, build)

		zf.close()

		#print(">>>>>>>>>>>>>>>>>>>>",abs_path, zip_file_name)
		return True
	else:
		return False

def zip_search_file(source,target):
	for file in source.filelist:
		if file.filename==target:
			return True
	return False



## Remove a file from an archive
def zip_remove_file(zip_file_name,target,act_only_on_archive=False):
	file_path=os.path.join(os.path.dirname(zip_file_name),target)
	#if has_handle(zip_file_name)==True:
	#	print("We are already open!",zip_file_name)
	#	exit(0)
	if act_only_on_archive==False:
		if os.path.isfile(file_path)==True:
			os.remove(file_path)


	if archive_isfile(zip_file_name,target)==True:
		source = zipfile.ZipFile(zip_file_name, 'r')

		found=zip_search_file(source,target)

		if found==True:
			fh, abs_path = mkstemp()
			zf = zipfile.ZipFile(abs_path, 'w',zipfile.ZIP_DEFLATED)

			for file in source.filelist:
				if not file.filename.startswith(target):
					info=source.getinfo(file.filename)
					zf.writestr(info, source.read(file))

			zf.close()
			os.close(fh)

		source.close()

		if found==True:
			#I think virus killers are opening the file on windows
			i=0
			while(i<3):
				try:
					if os.path.isfile(zip_file_name):
						os.remove(zip_file_name)
					shutil.move(abs_path, zip_file_name)
					return
				except:
					print("Waiting for the file to close: ",zip_file_name)
					print("... file a bug report if gpvdm does not work because of this.")
					time.sleep(1)
				i=i+1
			#failed three times to open the file, so try again and expect to fail
			if os.path.isfile(zip_file_name):
				os.remove(zip_file_name)
			shutil.move(abs_path, zip_file_name)
			return

## Write lines to an archive file.
def write_lines_to_archive(archive_path,file_name,lines,mode="l",dest="archive"):

	file_path=os.path.join(os.path.dirname(archive_path),file_name)
	if os.path.isfile(file_path)==True or os.path.isfile(archive_path)==False or dest=="file":
		zip_remove_file(archive_path,file_name)

		if mode=="l":
			dump=""
			for item in lines:
				dump=dump+item+"\n"

			dump=dump.rstrip("\n")
		if mode=="b":
			dump=lines
			
		f=open(file_path, mode='wb')
		if mode=="l":
			lines = f.write(str.encode(dump))
		if mode=="b":
			lines = f.write(dump)

		f.close()
		return True
	else:
		return replace_file_in_zip_archive(archive_path,file_name,lines)

## Move all .inp files into an archive, and remove them from the simulation directory.
def archive_compress(archive_path):
	if os.path.isfile(archive_path)==False:
		archive_make_empty(archive_path)

	if os.path.isfile(archive_path)==True:
		lines=[]
		dir_name=os.path.dirname(archive_path)
		for file_name in os.listdir(dir_name):
			full_name=os.path.join(dir_name,file_name)
			if file_name.endswith(".inp")==True:
				lines=read_lines_from_archive(archive_path,file_name,mode="b")
				os.remove(full_name)
				replace_file_in_zip_archive(archive_path,file_name,lines,delete_first=False)

## Move a file to an archive.
def archiv_move_file_to_archive(archive_path,file_name,base_dir):
	archive_add_file(archive_path,file_name,base_dir)
	os.remove(file_name)

## Add a file to an archive.
def archive_add_file(archive_path,file_name,base_dir,clean_archive_of_old_files=True):
		lines=[]
		name_of_file_in_archive=subtract_paths(base_dir,file_name)

		if clean_archive_of_old_files==True:
			zip_remove_file(archive_path,name_of_file_in_archive,act_only_on_archive=True)

		if os.path.isfile(file_name):
			f=open(file_name, mode='rb')
			lines = f.read()
			f.close()
		else:
			return False

		zf = zipfile.ZipFile(archive_path, 'a',zipfile.ZIP_DEFLATED)

		zf.writestr(name_of_file_in_archive, lines)
		zf.close()
		return True

## Add a directory to an archive.
def archive_add_dir(archive_path,dir_name,base_dir, remove_src_dir=False,zf=None,exclude=[]):

	close_file=False

	if zf==None:
		close_file=True
		zf = zipfile.ZipFile(archive_path, 'a',zipfile.ZIP_DEFLATED)

	for root, dirs, files in os.walk(dir_name):
		for name in files:
			file_name=os.path.join(root, name)

			if os.path.isfile(file_name):
				if archive_path!=file_name:
					f=open(file_name, mode='rb')
					lines = f.read()
					f.close()
					
					if os.path.basename(file_name) not in exclude:
						name_of_file_in_archive=subtract_paths(base_dir,file_name)
						zf.writestr(name_of_file_in_archive, lines)


	if close_file==True:
		zf.close()

	if remove_src_dir==True: 
		if base_dir=="" or base_dir==dir_name or dir_name=="/" or dir_name=="/home/rod" or dir_name=="/home/rod/" or dir_name=="c:\\":	#paranoia
			return

		gpvdm_delete_file(dir_name,allow_dir_removal=True)

def read_lines_from_file(file_name):
	if os.path.isfile(file_name)==True:
		f = open(file_name, mode='rb')
		lines = f.read()
		f.close()

	try:
		lines=lines.decode('utf-8')
		lines=lines.split("\n")
	except:
		lines=[]
		
	return lines


def archive_get_file_time(zip_file_path,file_name):
	epoch=False
	if os.path.isfile(zip_file_path):
		try:
			zf = zipfile.ZipFile(zip_file_path, 'r')
		except:
			return False
		if zip_search_file(zf,file_name)==True:
			t=zf.getinfo(file_name).date_time
			dos_time=str(t[0])+" "+str(t[1])+" "+str(t[2])+" "+str(t[3])+" "+str(t[4])+" "+str(t[5])
			epoch = int(time.mktime(time.strptime(dos_time, '%Y %m %d %H %M %S')))
			#print(">>>>>",epoch)

		zf.close()

	return epoch

## Read liens from an archive.
def read_lines_from_archive(zip_file_path,file_name,mode="l"):
	file_path=os.path.join(os.path.dirname(zip_file_path),file_name)

	read_lines=[]
	found=False

	if os.path.isfile(file_path):	#for /a/b/c/sim.gpvdm a.dat, check /a/b/c/a.dat
		f=open(file_path, mode='rb')
		read_lines = f.read()
		f.close()
		found=True
	
	if found==False:					#for /a/b/c/sim.gpvdm a.dat, check /a/b/c/sim/a.dat
		file_path=os.path.join(zip_file_path[:-4],file_name)
		if os.path.isfile(file_path):
			f=open(file_path, mode='rb')
			read_lines = f.read()
			f.close()
			found=True

	if found==False:
		if os.path.isfile(zip_file_path):
			zip_file_open_ok=True
			try:
				zf = zipfile.ZipFile(zip_file_path, 'r')
			except:
				zip_file_open_ok=False

			if zip_file_open_ok==True:
				if zip_search_file(zf,os.path.basename(file_path))==True:
					read_lines = zf.read(file_name)
					found=True
				elif zip_search_file(zf,file_name)==True:
					read_lines = zf.read(file_name)
					found=True

				zf.close()

	if found==False:
		return False

	#print(">",file_path,"<",read_lines)
	if mode=="l":
		read_lines=read_lines.decode('utf-8')#.decode("utf-8") 
		#read_lines=[str.strip() for str in read_lines.splitlines()]#read_lines.split("\n")
		read_lines=read_lines.split("\n")

		lines=[]

		for i in range(0, len(read_lines)):
			lines.append(read_lines[i].rstrip())

		if lines[len(lines)-1]=='\n':
			del lines[len(lines)-1]
	elif mode=="b":
		lines=read_lines


	return lines


## This will unpack an acrhive
def archive_decompress(zip_file_path,remove_gpvdm_file=True):

	if os.path.isfile(zip_file_path):
		zf = zipfile.ZipFile(zip_file_path, 'r')
		for file_name in zf.filelist:
			read_lines = zf.read(file_name)
			out_file=os.path.join(os.path.dirname(zip_file_path),file_name.filename)
			f=open(out_file, mode='wb')
			f.write(read_lines)
			f.close()

		zf.close()

		os.remove(zip_file_path)

		if remove_gpvdm_file==False:
			dir_name=os.path.dirname(zip_file_path)
			ver_file=os.path.join(dir_name,"ver.inp")
			archive_make_empty(zip_file_path)
			archiv_move_file_to_archive(zip_file_path,ver_file,dir_name)

	return

## This will extract a file from an archive and write it to disk.
def extract_file_from_archive(dest,zip_file_path,file_name):

	file_path=os.path.join(os.path.dirname(zip_file_path),file_name)

	read_lines=[]

	if os.path.isfile(file_path):
		f=open(file_path, mode='rb')
		read_lines = f.read()
		f.close()
	else:
		found=False

		if os.path.isfile(zip_file_path):
			zf = zipfile.ZipFile(zip_file_path, 'r')
			if zip_search_file(zf,os.path.basename(file_path))==True:
				read_lines = zf.read(file_name)
				found=True
			elif zip_search_file(zf,file_name)==True:
				read_lines = zf.read(file_name)
				found=True

			zf.close()

		if found==False:
			print("not found",file_name)
			return False

	if file_name.endswith("/")==True:
		if os.path.isdir(os.path.join(dest,file_name))==False:
			os.makedirs(os.path.join(dest,file_name))
		return True
	else:
		if os.path.isdir(os.path.join(dest,os.path.dirname(file_name)))==False:
			os.makedirs(os.path.join(dest,os.path.dirname(file_name)))

		f=open(os.path.join(dest,file_name), mode='wb')
		lines = f.write(read_lines)
		f.close()

	return True

## Extract a directory from an archive.
def extract_dir_from_archive(dest,zip_file_path,dir_name,zf=None):
	items=zf.namelist()
	for i in range(0,len(items)):
		if items[i].startswith(dir_name)==True:
			read_lines = zf.read(items[i])
			output_file=os.path.join(dest,subtract_paths(dir_name,items[i]))
			output_dir=os.path.dirname(output_file)

			if os.path.isdir(output_dir)==False:
				os.makedirs(output_dir)
#			print(output_file,"'"+items[i]+"'",zip_file_path)

			if items[i].endswith('/')==False:	#test if it is not a dir
				f=open(output_file, mode='wb')
				lines = f.write(read_lines)
				f.close()

## Does the file exist in an archive.
def archive_isfile(zip_file_name,file_name):
	ret=False

	file_path=os.path.join(os.path.dirname(zip_file_name),file_name)

	if os.path.isfile(file_path):
		ret=True
	else:
		if os.path.isfile(zip_file_name):
			zf = zipfile.ZipFile(zip_file_name, 'r')
			if file_name in zf.namelist():
				ret=True
			zf.close()
		else:
			ret=False

	return ret


## Zip up an entire directory.
def archive_zip_dir(path,extentions=[]):
	#print("zipping: ",path)
	all_files=[]
	for root, dirs, files in os.walk(path):
		for name in files:
			file_name=os.path.join(root, name)
			if len(extentions)==0:
				all_files.append(file_name)
			else:
				ext=os.path.splitext(file_name)
				if len(ext)>1:
					if ext[1] in extentions:
						all_files.append(file_name)

	archive_path=path+".zip"
	if os.path.isfile(archive_path)==True:
		os.remove(archive_path)


	zf = zipfile.ZipFile(archive_path, 'a',zipfile.ZIP_DEFLATED)
	for file_name in all_files:
		f=open(file_name, mode='rb')
		lines = f.read()
		f.close()

		name_of_file_in_archive=subtract_paths(path,file_name)

		zf.writestr(name_of_file_in_archive, lines)

	zf.close()

def archive_build_index_file(path,description="",target=None,ver="1.0"):
	data=[]
	for f in os.listdir(path):
		file_name=os.path.join(path,f)
		if f!="info.dat":

			a=update_file_info()
			a.file_name=f
			a.description=description

			if os.path.isfile(file_name)==True:
				a.size=os.path.getsize(file_name)
				hash_md5 = hashlib.md5()
				file_han=open(file_name, "rb")
				for chunk in iter(lambda: file_han.read(4096), b""):
					hash_md5.update(chunk)
				file_han.close()

				a.md5=hash_md5.hexdigest()
				a.ver=ver

			if description!=None:
				a.target=target
			data.extend(str(a).split("\n"))
	f=open(os.path.join(path,"info.dat"), mode='w')
	for item in data:
		f.write(item+"\n")

	f.write("#ver\n")
	f.write("1.0\n")
	f.write("#end\n")

	f.close()

def archive_extract(dest_path,archive_path,call_back=None):
	zf = zipfile.ZipFile(archive_path, 'r')
	items=zf.namelist()
	for i in range(0,len(items)):
		f=items[i]
		out_path=os.path.join(dest_path,f)
		dir_name=os.path.dirname(out_path)
		if os.path.isdir(dir_name)==False:
			os.makedirs(dir_name)

		data = zf.read(f)
		if call_back!=None:
			call_back(archive_path,int(100.0*(i/len(items))))

		f=open(out_path, mode='wb')
		lines = f.write(data)
		f.close()

	zf.close()
