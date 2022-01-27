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


## @package util
#  General helper functions.
#



import os
import shutil
import re
import hashlib
import glob
from util_zip import zip_get_data_file
from util_zip import zip_get_raw_data

from math import pow

def peek_data(file_name):
	try:
		text=zip_get_raw_data(file_name,bytes=100)
		if text==False:
			return b"none"
		return text
	except:
		pass
	return b"none"

def set_file_ext(file_name,ext):
	if ext[0]!=".":
		ext="."+ext
	return os.path.splitext(file_name)[0]+ext

def wrap_text(text,width):
	count=0
	r=False
	out=""
	for i in range(0,len(text)):
		if count>width:
			r=True
		else:
			r=False
		
		add=text[i]
		if r==True:
			if text[i]==" ":
				add="\n"
				count=0
		
		out=out+add
				
		count=count+1
	return out

def gui_print_path(text,path,length):
	remove=len(text)+len(path)-length
	if remove>0:
		ret=text+path[remove:]
	else:
		ret=text+path

	return ret

def isnumber(s):
	try:
		float(s)
		return True
	except ValueError:
		return False

def isfiletype(file_name,ext_in):
	ext=ext_in
	if ext.startswith(".")==False:
		ext="."+ext
	if file_name.endswith(ext):
		return True
	return False


def copy_scan_dir(new_dir,old_dir):
	print("trying to copy",old_dir,new_dir)
	if not os.path.exists(new_dir):
		os.makedirs(new_dir)
	for filename in glob.glob(os.path.join(old_dir, '*.*')):
		if os.path.isfile(filename):
			shutil.copy(filename, new_dir)



def pygtk_to_latex_subscript(in_string):
	out_string=in_string.replace("<sub>","_{")
	out_string=out_string.replace("</sub>","}")
	out_string=out_string.replace("<sup>","^{")
	out_string=out_string.replace("</sup>","}")
	return out_string

def latex_to_html(in_string):
	out=re.compile(r"_\{([^\]]*?)\}").sub("<sub>\\1</sub>", in_string)
	out=re.compile(r"\^\{([^\]]*?)\}").sub("<sup>\\1</sup>", out)
	return out


def time_with_units(time):
	ret=str(time)
	mul=1.0
	if (time<1000e-15):
		ret="fs"
		mul=1e15
	elif (time<1000e-12):
		ret="ps"
		mul=1e12
	elif (time<1000e-9):
		ret="ns"
		mul=1e9
	elif (time<1000e-6):
		ret="us"
		mul=1e6
	elif (time<1000e-3):
		ret="ms"
		mul=1e3
	else:
		ret="s"
		mul=1.0
	return mul,ret

def fx_with_units(fx):
	ret=str(fx)
	if (fx<1e3):
		ret=" Hz"
		mul=1.0
	elif (fx<1e6):
		ret="kHz"
		mul=1e-3
	elif (fx<1e9):
		ret="MHz"
		mul=1e-6
	elif (fx<1e12):
		ret="GHz"
		mul=1e-9

	return mul,ret

def distance_with_units(distance):
	ret=str(distance)
	if (distance<1e-9):
		ret="pm"
		mul=1e12
	elif (distance<1e-6):
		ret="nm"
		mul=1e9
	elif (distance<1e-3):
		ret="um"
		mul=1e6
	else:
		ret="mm"
		mul=1e3


	return mul,ret


def gpvdm_copy_src(new_dir):
	pwd=os.getcwd()
	file_list=glob.glob(os.path.join(pwd,"*"))

	if not os.path.exists(new_dir):
		os.makedirs(new_dir)
	print(file_list)
	for name in file_list:
		gui_file_name=os.path.join(name,"gpvdm_gui_config.inp")

		if os.path.isfile(gui_file_name)==False:
			fname=os.path.basename(name)
			out=os.path.join(new_dir,fname)
			if os.path.isfile(name):
				 shutil.copy(name, out)
			else:
				print("Copy dir:", name)
				shutil.copytree(name, out,symlinks=True)
		else:
			print("I will not copy",name)



def strextract_interger(val):
	build=""
	for i in range(0,len(val)):
		if val[i].isdigit()==True:
			build=build+val[i]

	if len(build)==0:
		return False
	else:
		return int(build)

def wavelength_to_rgb(wavelength):
	gamma = 0.80
	intensity_max = 1.0
	factor=0

	red=0
	green=0
	blue=0

	if ((wavelength >= 380) and (wavelength<440)):
		red = -(wavelength - 440) / (440 - 380);
		green = 0.0
		blue = 1.0
	elif ((wavelength >= 440) and  (wavelength<490)):
		red = 0.0
		green = (wavelength - 440) / (490 - 440)
		blue = 1.0
	elif((wavelength >= 490) and  (wavelength<510)):
		red = 0.0
		green = 1.0
		blue = -(wavelength - 510) / (510 - 490)
	elif((wavelength >= 510) and  (wavelength<580)):
		red = (wavelength - 510) / (580 - 510);
		green = 1.0
		blue = 0.0
	elif ((wavelength >= 580) and  (wavelength<645)):
		red = 1.0
		green = -(wavelength - 645) / (645 - 580)
		blue = 0.0
	elif ((wavelength >= 645) and  (wavelength<781)):
		red = 1.0
		green = 0.0
		blue = 0.0
	else:
		red = 0.0
		green = 0.0
		blue = 0.0
	#(wavelength >= 380) and  
	if (wavelength<420):
		factor = 0.3 + 0.7*(wavelength - 380) / (420 - 380)
	elif ((wavelength >= 420) and  (wavelength<701)):
		factor = 1.0
	elif ((wavelength >= 701) and  (wavelength<781)):
		factor = 0.3 + 0.7*(780 - wavelength) / (780 - 700)
	else:
		factor = 0.0

	if red != 0:
		red = (intensity_max * pow(red * factor, gamma))
		
	if green != 0:
		green = (intensity_max * pow(green * factor, gamma))
	
	if blue != 0:
		blue = (intensity_max * pow(blue * factor, gamma))
	

	return red,green,blue
