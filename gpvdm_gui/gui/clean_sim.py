#!/usr/bin/env python3
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

## @package clean_sim
#  Clean the simulation
#

#import sys
import os
import shutil
from cal_path import get_sim_path

#import glob

def remove_bin(directory):
	for root, dirs, files in os.walk(os.path.join(os.getcwd(),directory)):
		for file in files:
			if file.endswith(".o"):
				del_file=os.path.join(root, file)
				print("Deleteing "+del_file)
				os.remove(del_file)

			if file.endswith(".so"):
				del_file=os.path.join(root, file)
				print("Deleteing "+del_file)
				os.remove(del_file)

			if file.endswith(".a"):
				del_file=os.path.join(root, file)
				print("Deleteing "+del_file)
				os.remove(del_file)

			if file.endswith(".dll"):
				del_file=os.path.join(root, file)
				print("Deleteing "+del_file)
				os.remove(del_file)

def clean_sim_dir():
	remove_bin("light")
	remove_bin("plugins")
	remove_bin("solvers")

	del_file=os.path.join(get_sim_path(),"pub")
	if os.path.isdir(del_file):
		print("Deleteing "+del_file)
		shutil.rmtree(del_file)

	del_file=os.path.join(get_sim_path(),"dynamic")
	if os.path.isdir(del_file):
		print("Deleteing "+del_file)
		shutil.rmtree(del_file)

	del_file=os.path.join(get_sim_path(),"snapshots")
	if os.path.isdir(del_file):
		print("Deleteing "+del_file)
		shutil.rmtree(del_file)

	files = os.listdir(get_sim_path())
	for file in files:
		if file.startswith("snapshots_"):
			print("deleting dir",file)
			shutil.rmtree(file)

	files = os.listdir(get_sim_path())
	for file in files:
		remove=False
		if file.endswith(".txt"):
			remove=True
		if file.endswith("~"):
			remove=True
		if file.endswith(".dat"):
			remove=True
		if file.endswith(".o"):
			remove=True
		if file.endswith(".orig"):
			remove=True
		if file.endswith(".back"):
			remove=True
		if file.endswith(".bak"):
			remove=True
		if file.endswith("gmon.out"):
			remove=True
		if remove==True:
			print("Deleting",file)
			os.remove(file)
