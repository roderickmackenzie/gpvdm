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

## @package scan_tree
#  Logic to itterate though a scan tree.
#
import os
import shutil
import glob
import random
from random import randint

from inp import inp_update_token_value
from inp import inp_get_token_array
from inp import inp

from str2bool import str2bool
from util import get_cache_path
from cal_path import get_materials_path
from scan_human_labels import get_scan_human_labels
from clone import gpvdm_clone

#windows
import codecs
from util_zip import archive_decompress
from util_zip import archive_compress

from cal_path import subtract_paths

from progress_class import progress_class
from process_events import process_events
import math
from util_zip import zip_lsdir

from scan_program_line import scan_program_line

def random_log(in_start,in_stop):
	start=math.log10(in_start)
	stop=math.log10(in_stop)

	r=random.uniform(start, stop)

	val=math.pow(10,r)

	ret="{:.8E}".format(val)
	return ret

class scan_tree_leaf:

	def __init__(self):
		self.scan_human_labels=get_scan_human_labels()
		#self.scan_human_labels.dump()
		self.directory=None
		self.program_list=[]

	def duplicate_params(self):
		
		for program_line in self.program_list:
			#print(program_list[i])
			if program_line.values=="duplicate":
				#print(">>>",program_line.opp,"<<",program_line.values)
				f=self.scan_human_labels.get_file_from_human_label(program_line.opp)
				src_token=self.scan_human_labels.get_token_from_human_label(program_line.opp)

				src_file=os.path.join(self.directory,f)
	
				in_file=inp()
				in_file.load(src_file)
				src_value=in_file.get_token(src_token)

				inp_update_token_value(os.path.join(self.directory,program_line.file), program_line.token, src_value)

		return True

	def apply_constants(self):
		for program_line in self.program_list:
			if program_line.opp=="constant":
				if program_line.file.endswith("*"):
					search_file=program_line.file[:-1]
					file_list=zip_lsdir(os.path.join(self.directory,"sim.gpvdm"))
					#print(self.directory,file_list)
					for f in file_list:
						if f.startswith(search_file)==True:
							inp_update_token_value(os.path.join(self.directory,f), program_line.token, program_line.values)
							#print(">>> edited",f)
				else:
					inp_update_token_value(os.path.join(self.directory,program_line.file), program_line.token, program_line.values)

		return True

	def apply_python_scripts(self):
		for program_line in self.program_list:
			if program_line.opp=="python_code":

				ret=""
				command=program_line.values
				ret=eval(command)
				file_path=os.path.join(self.directory,program_line.file)
				#print("EXEC=",command,">",ret,"<")
				error_status=inp_update_token_value(file_path, program_line.token, ret)
				if error_status==False:
					return False

		return True

def tree_load_flat_list(sim_dir):
	config=[]
	file_name=os.path.join(sim_dir,'flat_list.inp')

	if os.path.isfile(file_name)==False:
		return False

	f = open(file_name)
	lines = f.readlines()
	f.close()

	for i in range(0, len(lines)):
		lines[i]=lines[i].rstrip()

	number=int(lines[0])

	for i in range(1,number+1):
		lines[i]=os.path.join(sim_dir,lines[i])
		if os.path.isdir(lines[i]):
			config.append(lines[i])

	return config

def tree_save_flat_list(sim_dir,flat_list):
	config=[]
	file_name=os.path.join(sim_dir,'flat_list.inp')

	a = open(file_name, "w")
	a.write(str(len(flat_list))+"\n")
	for i in range(0,len(flat_list)):
		rel_dir=subtract_paths(sim_dir,flat_list[i])
		a.write(rel_dir+"\n")

	a.close()

	return config

def tree_gen_flat_list(dir_to_search,level=0):
	found_dirs=[]
	for root, dirs, files in os.walk(dir_to_search):
		for name in files:

			if name=="sim.gpvdm":
				full_name=os.path.join(root, name)
				f=subtract_paths(dir_to_search,full_name)
				f=os.path.dirname(f)
				if len(f.split("/"))>level:
					found_dirs.append(f)
	return found_dirs


def build_scan_tree(program_list):
	tree_items=[[],[],[]]	#file,token,values,opp
	for program_line in program_list:
		#print(i,program_line.file,program_line.token,program_line.values,line.opp)
		if program_line.opp=="scan":
			tree_items[0].append(program_line.file)
			tree_items[1].append(program_line.token)
			values=program_line.values
			#This expands a [ start stop step ] command.
			if len(values)>0:
				if values[0]=='[' and values[len(values)-1]==']':
					values=values[1:len(values)-1]
					data=values.split()
					if len(data)==3:
						a=float(data[0])
						b=float(data[1])
						c=float(data[2])
						values=""
						pos=a
						while pos<b:
							values=values+str(pos)+" "
							pos=pos+c
						values=values[0:len(values)-1]

			tree_items[2].append(values)

	return tree_items

def tree_gen(output_dir,flat_simulation_list,program_list,base_dir):
	output_dir=os.path.abspath(output_dir)	# we are about to traverse the directory structure better to have the abs path
	#print("here",program_list)
	found_scan=False
	found_random=False
	for program_line in program_list:
		if program_line.opp=="scan":
			found_scan=True

		if program_line.opp=="random_file_name":
			found_random=True

	if found_scan==True and found_random==True:
		return False

	if found_random==True:
		tree_gen_random_files(output_dir,flat_simulation_list,program_list,base_dir)
		return

	tree_items=build_scan_tree(program_list)			#tree_items[3].append(program_line.opp)

	ret=tree(flat_simulation_list,program_list,tree_items,base_dir,0,output_dir,"","")

	return ret



def copy_simulation(base_dir,cur_dir):
	gpvdm_clone(cur_dir,src_archive=os.path.join(base_dir, "sim.gpvdm"),dest="file")
	


def tree_gen_random_files(sim_path,flat_simulation_list,program_list,base_dir):
	length=0

	for program_line in program_list:
		if program_line.opp=="random_file_name":
			length=int(program_line.values)

	progress_window=progress_class()
	progress_window.show()
	progress_window.start()

	process_events()

	print("length",length)

	for i in range(0,length):
		rand=codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()
		cur_dir=os.path.join(sim_path,rand)

		if not os.path.exists(cur_dir):
			os.makedirs(cur_dir)
			gpvdm_clone(cur_dir,src_archive=os.path.join(base_dir, "sim.gpvdm"),dest="file")

			os.chdir(cur_dir)
			archive_decompress(os.path.join(cur_dir,"sim.gpvdm"))

			t=scan_tree_leaf()
			t.program_list=program_list
			t.directory=cur_dir
			
			if t.apply_constants()==False:
				return False

			if t.apply_python_scripts()==False:
				return False


			t.duplicate_params()
			#tree_apply_duplicate(cur_dir,program_list)
			
			archive_compress(os.path.join(cur_dir,"sim.gpvdm"))

			flat_simulation_list.append(cur_dir)

			progress_window.set_fraction(float(i)/float(length))
			progress_window.set_text("Adding "+cur_dir)
			process_events()

	progress_window.stop()

def tree(flat_simulation_list,program_list,tree_items,base_dir,level,path,var_to_change,value_to_change):
	#print(level,tree_items)
	values=tree_items[2][level]
	values=values.split()

	if tree_items[0][level]=="notknown":
		return False

	if tree_items[1][level]=="notknown":
		return False

	pass_var_to_change=var_to_change+" "+str(level)
	#print(pass_var_to_change)
	for ii in values:
		cur_dir=os.path.join(path,ii)

		if not os.path.exists(cur_dir):
			os.makedirs(cur_dir)

		pass_value_to_change=value_to_change+" "+ii

		if ((level+1)<len(tree_items[0])):
				ret=tree(flat_simulation_list,program_list,tree_items,base_dir,level+1,cur_dir,pass_var_to_change,pass_value_to_change)
				if ret==False:
					return False
		else:
			flat_simulation_list.append(cur_dir)
			new_values=pass_value_to_change.split()
			pos=pass_var_to_change.split()

			config_file=os.path.join(cur_dir,"sim.gpvdm")
			if os.path.isfile(config_file)==False:	#Don't build a simulation over something that exists already
				copy_simulation(base_dir,cur_dir)
				os.chdir(cur_dir)
				t=scan_tree_leaf()
				t.program_list=program_list
				t.directory=cur_dir

				if t.apply_constants()==False:
					return False

				if t.apply_python_scripts()==False:
					return False

				for i in range(0, len(pos)):
					file_path=os.path.join(cur_dir,tree_items[0][int(pos[i])])
					inp_update_token_value(file_path, tree_items[1][int(pos[i])], new_values[i])
					#print("updating", file_path, tree_items[1][int(pos[i])], new_values[i])


				t.duplicate_params()

				#tree_apply_duplicate(cur_dir,program_list)

				inp_update_token_value(os.path.join(cur_dir,"dump.inp"), "#plot", "0")

		if level==0:
			f = open(os.path.join(cur_dir,'scan.inp'),'w')
			f.write("data")
			f.close()
	return True
