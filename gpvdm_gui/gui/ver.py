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

## @package ver
#  Read the ver.inp file and figure out which version of gpvdm we are running.
#


import os
from cal_path import get_inp_file_path
from cal_path import get_share_path
from cal_path import get_bin_path
from inp import inp_load_file
from inp import inp_update_token_value
from inp import inp_search_token_value
from util_zip import read_lines_from_archive

from cal_path import get_sim_path


global core
global ver_error
global subver
global gpvdm_version

def ver_error():
	global ver_error
	return ver_error

def ver_core():
	global core
	return core

def ver_subver():
	global subver
	return subver

def version():
	global core
	global subver
	string="core: Version "+core+"."+subver
	return string

def ver():
	global core
	global subver
	return str(core)+"."+subver

class ver_ctrl():
	def is_gpvdm_next():
		global gpvdm_version
		if gpvdm_version=="gpvdm_next":
			return True
		return False

	def is_gpvdm_open():
		global gpvdm_version
		if gpvdm_version=="gpvdm_open":
			return True
		return False

def ver_load_info():
	lines=[]
	global core
	global ver_error
	global subver
	global gpvdm_version
	core=""
	ver_error=""


	ver_file_path=os.path.join(get_inp_file_path(),"ver.inp")

	lines=inp_load_file(ver_file_path,archive="base.gpvdm")
	if lines!=False:
		core=inp_search_token_value(lines,"#core")
		subver=inp_search_token_value(lines,"#sub_ver")
		gpvdm_version=inp_search_token_value(lines,"#gpvdm_version")
		return True
	else:
		ver_error="I can not find the file sim.gpvdm/ver.inp.\n\nI have tried looking in "+ver_file_path+"\n\nThe share path is"+get_share_path()+"\n\nThe bin path is"+get_bin_path()+"\n\nThe current working dir is "+get_sim_path()+"\n\nTry reinstalling a new version of gpvdm and/or report the bug to me at  roderick.mackenzie@nottingham.ac.uk."
		return False

def ver_sync_ver():
	file_name=os.path.join(os.getcwd(),"include","version.h")
	found=False

	if os.path.isfile(file_name)==True:
		f = open(file_name, "r")
		lines = f.readlines()
		f.close()
		for l in range(0, len(lines)):
			lines[l]=lines[l].rstrip()

			if lines[l].startswith("#define")==True:
				print(lines[l])
				text=(lines[l].split("\t")[2].strip("\""))
				found=True
				break

	if found==True:
		print("seting ver to: ",text)
		inp_update_token_value("ver.inp","#core",text,archive="sim.gpvdm")
		inp_update_token_value("ver.inp","#core",text,archive="base.gpvdm")
		
	else:
		print(_("version.h not found"))


def ver_check_compatibility(file_name):
	lines=[]
	core=""

	lines=read_lines_from_archive(file_name,"ver.inp")
	if lines!=False:
		core=inp_search_token_value(lines,"#core")
		if core==ver_core():
			return True
		else:
			return False

	return False


