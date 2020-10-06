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

## @package cal_path
#  Calculate the where files are, and if you can't find them look harder.
#

import sys
import os
#import shutil
from pathlib import Path
import os

package_path=None
pub_path=None
rpm_build_dir=None

def subtract_paths(root,b_in):
	a=root.replace("/","\\")
	b=b_in.replace("/","\\")
	a=a.split("\\")
	b=b.split("\\")
	a_len=len(a)
	b_len=len(b)
	m=a_len
	if b_len<m:
		m=b_len
	pos=0

	for i in range(0,m):
		if a[i]!=b[i]:
			break
		pos=pos+1

	ret=[]
	for i in range(pos,b_len):
		ret.append(b[i])

	return "/".join(ret)


def get_ver():
	f = open("./gpvdm_core/include/version.h", mode='rb')
	lines = f.read()
	f.close()

	lines=lines.decode('utf-8')
	lines=[str.strip() for str in lines.splitlines()]
	ver=""
	sub_ver=""
	for l in lines:
		if l.count("gpvdm_ver")>0:
			ver=l.split()[2][1:-1]
		if l.count("gpvdm_sub_ver")>0:
			sub_ver=l.split()[2][1:-1]

	return ver#,sub_ver

def build_setup_paths():
	global package_path
	global pub_path
	global rpm_build_dir
	import distro
	ret=distro.linux_distribution(full_distribution_name=False)
	distro_name=ret[0]
	os_numer=ret[1]
	os_cute_name=ret[2].replace(" ","_")

	distro=ret[0]+"_"+ret[1]
	package_path=os.path.join(os.getcwd(),"package_lib",distro_name,os_numer+"_"+os_cute_name)
	if os.path.isdir(package_path)==False:
		os.makedirs(package_path)

	pub_path=os.path.join(os.getcwd(),"pub")
	rpm_build_dir=os.path.join(os.getcwd(),"rpm")

def get_package_path():
	global package_path
	return package_path

def get_pub_path():
	global pub_path
	return pub_path

def get_rpm_build_dir():
	global rpm_build_dir
	return rpm_build_dir
