#!/usr/bin/env python3
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

## @package clean_sim
#  Clean the simulation
#

#import sys
import os
from cal_path import get_sim_path
from safe_delete import gpvdm_delete_file

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

def clean_sim_dir(path,clean_exp_data=True):
	if clean_exp_data==True:
		for f in os.listdir(path):
			full_path=os.path.join(path,f)
			if f.startswith("fit_data")==True:
				if f.endswith(".inp")==True:
					os.unlink(full_path)

	return
	remove_bin("light")
	remove_bin("plugins")
	remove_bin("solvers")

	del_file=os.path.join(get_sim_path(),"pub")
	if os.path.isdir(del_file):
		print("Deleteing "+del_file)
		gpvdm_delete_file(del_file,allow_dir_removal=True)

	del_file=os.path.join(get_sim_path(),"dynamic")
	if os.path.isdir(del_file):
		print("Deleteing "+del_file)
		gpvdm_delete_file(del_file,allow_dir_removal=True)

	del_file=os.path.join(get_sim_path(),"snapshots")
	if os.path.isdir(del_file):
		print("Deleteing "+del_file)
		gpvdm_delete_file(del_file,allow_dir_removal=True)

	files = os.listdir(get_sim_path())
	for file in files:
		if file.startswith("snapshots_"):
			print("deleting dir",file)
			gpvdm_delete_file(file,allow_dir_removal=True)

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

def delete_files(dirs_to_del,parent_window=None):
	if parent_window!=None:
		from yes_no_cancel_dlg import yes_no_cancel_dlg
		from progress_class import progress_class
		from process_events import process_events

		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		process_events()
	
	for i in range(0, len(dirs_to_del)):
		gpvdm_delete_file(dirs_to_del[i],allow_dir_removal=True)
		if parent_window!=None:
			progress_window.set_fraction(float(i)/float(len(dirs_to_del)))
			progress_window.set_text("Deleting"+dirs_to_del[i])
			process_events()

	if parent_window!=None:
		progress_window.stop()


def ask_to_delete(parent_window,dirs_to_del,interactive=True):
	if len(dirs_to_del)!=0:

		if interactive==True:
			from yes_no_cancel_dlg import yes_no_cancel_dlg
			text_del_dirs=""
			n=0
			for dir_item in dirs_to_del:
				text_del_dirs=text_del_dirs+dir_item+"\n"
				if n>15:
					text_del_dirs=text_del_dirs+_("and ")+str(len(dirs_to_del)-30)+_(" more.")
					break
				n=n+1

			text=_("Should I delete these files?:\n")+"\n"+text_del_dirs

			response = yes_no_cancel_dlg(parent_window,text)

			if response == "yes":
				delete_files(dirs_to_del,parent_window)
				return "yes"
			elif response == "no":
				return "no"
			elif response == "cancel":
				return "cancel"
		else:
			delete_files(dirs_to_del,parent_window)
