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

## @package scan_io
#  IO functions for the scanning simulation parameters.
#


import sys
import os
import shutil
import gc

from inp import inp_get_token_value
from scan_tree import tree_load_flat_list
from scan_tree import tree_gen_flat_list

from scan_tree import tree_gen
from scan_tree import tree_save_flat_list

from server_io import server_find_simulations_to_run


from server import server_break
from numpy import std

from error_dlg import error_dlg
from scan_tree import scan_tree_leaf
from process_events import process_events
from safe_delete import gpvdm_delete_file

import i18n
_ = i18n.language.gettext


import zipfile
from util_zip import archive_add_dir
from inp import inp
from scan_program_line import scan_program_line
from clean_sim import ask_to_delete
from progress_class import progress_class

def scan_next_archive(sim_dir):
	i=0
	while(1):
		name="archive"+str(i)+".zip"
		full_name=os.path.join(sim_dir,name)
		if os.path.isfile(full_name)==False:
			return name
		i=i+1

def scan_archive(sim_dir,progress_window=None):
	own_progress_window=False
	if progress_window==None:
		progress_window=progress_class()
		progress_window.show()
		progress_window.start()
		own_progress_window=True

	archive_path=os.path.join(sim_dir,"build_archive.zip")
	if os.path.isfile(archive_path)==True:
		os.remove(archive_path)
	zf = zipfile.ZipFile(archive_path, 'a',zipfile.ZIP_DEFLATED)

	l=os.listdir(sim_dir)
	for i in range(0,len(l)):
		dir_to_zip=os.path.join(sim_dir,l[i])
		if os.path.isdir(dir_to_zip)==True:
			archive_add_dir(archive_path,dir_to_zip,sim_dir,zf=zf,remove_src_dir=True,exclude=["gmon.out"])

		progress_window.set_fraction(float(i)/float(len(l)))
		progress_window.set_text(_("Adding: ")+l[i])

		#if server_break()==True:
		#	break
		process_events()
		
	zf.close()

	os.rename(archive_path, os.path.join(sim_dir,scan_next_archive(sim_dir)))

	if own_progress_window==True:
		progress_window.stop()



def scan_list_simulations(dir_to_search):
	found_dirs=[]
	for root, dirs, files in os.walk(dir_to_search):
		for name in files:
#			full_name=os.path.join(root, name)
			if name=="sim.gpvdm":
				found_dirs.append(root)
	return found_dirs

def scan_plot_fits(dir_to_search):
	files=os.listdir(dir_to_search)
	for i in range(0,len(files)):
		if files[i].endswith(".jpg"):
			os.remove(os.path.join(dir_to_search,files[i]))
			#print("remove",os.path.join(dir_to_search,files[i]))

	sim_dirs=tree_load_flat_list(dir_to_search)
	
	for i in range(0,len(sim_dirs)):
		os.chdir(sim_dirs[i])
		name=sim_dirs[i].replace("/","_")
		
		os.system("gnuplot fit.plot >plot.eps")
		os.system("gs -dNOPAUSE -r600 -dEPSCrop -sDEVICE=jpeg -sOutputFile="+os.path.join(dir_to_search,name+".jpg")+" plot.eps -c quit")
	os.chdir(dir_to_search)

def scan_get_converged_status(fit_log_path):
	error=False

	if os.path.isfile(fit_log_path):
		f = open(fit_log_path, "r")
		lines = f.readlines()
		f.close()

		for l in range(0, len(lines)):
			lines[l]=lines[l].rstrip()

		if len(lines)>4:
			error=float(lines[len(lines)-2].split()[1])
	
	return error

def scan_list_unconverged_simulations(dir_to_search):
	found_dirs=[]
	sim_dirs=tree_load_flat_list(dir_to_search)
	
	for i in range(0,len(sim_dirs)):
		add=False
		fit_log=os.path.join(sim_dirs[i],'fitlog.dat')

		error=scan_get_converged_status(fit_log)

		if error==False:
			add=True
		elif error>0.1:
			add=True

		if add==True:
			found_dirs.append(sim_dirs[i])

	return found_dirs



class report_token():
	def __init__(self,file_name,token):
		self.file_name=file_name
		self.token=token
		self.values=[]

def scan_gen_report(path):
	tokens=[]
	tokens.append(report_token("dos0.inp","#Etrape"))
	tokens.append(report_token("dos0.inp","#mueffe"))
	tokens.append(report_token("dos0.inp","#Ntrape"))
	tokens.append(report_token("dos0.inp","#srhsigman_e"))
	tokens.append(report_token("dos0.inp","#srhsigmap_e"))
	tokens.append(report_token("dos0.inp","#srhsigman_h"))
	tokens.append(report_token("dos0.inp","#srhsigmap_h"))
	tokens.append(report_token("sim/thick_light/sim_info.dat","#jv_pmax_tau"))
	tokens.append(report_token("sim/thick_light/sim_info.dat","#jv_pmax_mue"))
	tokens.append(report_token("sim/thick_light/sim_info.dat","#jv_pmax_muh"))
	tokens.append(report_token("jv1.inp","#jv_Rcontact"))
	tokens.append(report_token("jv1.inp","#jv_Rshunt"))


	simulation_dirs=tree_load_flat_list(path)
	errors=[]
	for i in range(0,len(simulation_dirs)):
		print(simulation_dirs[i])

		error=scan_get_converged_status(os.path.join(simulation_dirs[i],"fitlog.dat"))
		print("error",error)
		errors.append(error)

		for ii in range(0,len(tokens)):
			value=inp_get_token_value(os.path.join(simulation_dirs[i],tokens[ii].file_name), tokens[ii].token)
			#print(os.path.join(simulation_dirs[i],tokens[ii].file_name), tokens[ii].token,value)
			if value!=None:
				print(tokens[ii].token,str(value))

				tokens[ii].values.append(float(value))

	print("Errors:",errors)
	for ii in range(0,len(tokens)):
		print(tokens[ii].token,tokens[ii].values,sum(tokens[ii].values)/len(tokens[ii].values),std(tokens[ii].values))

	for ii in range(0,len(tokens)):
		print(tokens[ii].token,sum(tokens[ii].values)/len(tokens[ii].values),std(tokens[ii].values))


def scan_build_nested_simulation(root_simulation,nest_simulation):

	if os.path.isdir(nest_simulation)==False:
		print("Path ",nest_simulation,"does not exist")
		sys.exit(0)

	progress_window=progress_class()
	progress_window.show()
	progress_window.start()

	process_events()

	nest_simulation_name=os.path.basename(nest_simulation) 
	program_list=tree_load_program(nest_simulation)
		
	files = os.listdir(root_simulation)
	simulations=[]
	for i in range(0,len(files)):
		if os.path.isfile(os.path.join(root_simulation,files[i],"sim.gpvdm"))==True:
			simulations.append(files[i])

	flat_simulation_list=[]

	path=os.getcwd()
	for i in range(0,len(simulations)):
		dest_name=os.path.join(root_simulation,simulations[i],nest_simulation_name)
		base_dir=os.path.join(root_simulation,simulations[i])
		#print(">>>",dest_name,base_dir,"<<",nest_simulation_name)
		tree_gen(dest_name,flat_simulation_list,program_list,base_dir)

		progress_window.set_fraction(float(i)/float(len(simulations)))
		progress_window.set_text(simulations[i])
		process_events()

	progress_window.stop()
	
	os.chdir(path)

	flat_simulation_list=tree_gen_flat_list(root_simulation,level=1)

	#print(flat_simulation_list)
	tree_save_flat_list(root_simulation,flat_simulation_list)

	return

def scan_clean_nested_simulation(root_simulation,nest_simulation):
	files = os.listdir(root_simulation)
	simulations=[]
	for i in range(0,len(files)):
		if os.path.isfile(os.path.join(root_simulation,files[i],"sim.gpvdm"))==True:
			simulations.append(files[i])

	for i in range(0,len(simulations)):
		dest_name=os.path.join(root_simulation,simulations[i])

		files = os.listdir(dest_name)
		for file in files:
			if file.endswith(".inp") or file.endswith(".gpvdm") or file.endswith(".dat") :
				os.remove(os.path.join(dest_name,file))


	return

def scan_clean_unconverged(parent,dir_to_clean):
		dirs_to_del=[]
		dirs_to_del=scan_list_unconverged_simulations(dir_to_clean)

		ask_to_delete(parent,dirs_to_del)

def scan_push_to_hpc(base_dir,only_unconverged):
	config_file=os.path.join(os.getcwd(),"server.inp")
	#print(config_file)
	hpc_path=inp_get_token_value(config_file, "#hpc_dir")
	hpc_path=os.path.abspath(hpc_path)

	if os.path.isdir(hpc_path)==True:
		hpc_files=[]
		hpc_files=scan_list_simulations(hpc_path)
		#print("hpc files=",hpc_files)
		delete_files(hpc_files)
		files=[]

		if only_unconverged==True:
			files=scan_list_unconverged_simulations(base_dir)
		else:
			files=scan_list_simulations(base_dir)

		#print("copy files=",files)
		for i in range(0,len(files)):
			dest_path=os.path.join(hpc_path,files[i][len(base_dir)+1:])
			#print(dest_path)
			shutil.copytree(files[i], dest_path,symlinks=True)
	else:
		print("HPC dir not found",hpc_path)

def scan_import_from_hpc(base_dir):
	config_file=os.path.join(os.getcwd(),"server.inp")
	hpc_path=inp_get_token_value(config_file, "#hpc_dir")
	hpc_path=os.path.abspath(hpc_path)

	if os.path.isdir(hpc_path)==True:

		hpc_files=scan_list_simulations(hpc_path)

		for i in range(0,len(hpc_files)):
			if hpc_files[i].endswith("orig")==False:
				src_path=hpc_files[i]
				dest_path=os.path.join(base_dir,hpc_files[i][len(hpc_path)+1:])
				if os.path.isdir(dest_path):
					gpvdm_delete_file(dest_path,allow_dir_removal=True)
				shutil.copytree(src_path, dest_path, symlinks=False, ignore=None)
				#print(src_path,dest_path)
	else:
		print("HPC dir not found",hpc_path)


class scan_io:

	def __init__(self):
		self.parent_window=None
		self.interactive=True
		self.scan_dir=None
		self.base_dir=None
		self.human_name=None
		self.config_file=None
		self.program_list=[]
		self.myserver=None

	def load(self,file_name):
		self.program_list=[]
		self.config_file=file_name
		f=inp()
		f.load(self.config_file)
		self.human_name=f.get_token("#scan_name")
		self.scan_dir=os.path.join(os.path.dirname(self.config_file),self.human_name)

		pos=2
		mylen=int(f.lines[pos])
		pos=pos+1

		for i in range(0, mylen):
			item=scan_program_line()
			#item.file=f.lines[pos]
			#item.token=f.lines[pos+1]
			item.human_name=f.lines[pos+2]
			item.values=f.lines[pos+3]
			item.opp=f.lines[pos+4]
			self.program_list.append(item)
			pos=pos+6

		#print(f.lines)

	def save(self):
		f=inp()
		f.lines=[]
		f.lines.append("#scan_name")
		f.lines.append(self.human_name)
		#print(self.tab.rowCount())
		f.lines.append(str(len(self.program_list)))
		for item in self.program_list:
			#print(i)
			f.lines.append("notused")
			f.lines.append("notused")
			f.lines.append(item.human_name)
			f.lines.append(item.values)
			f.lines.append(item.opp)
			f.lines.append("notused")

		f.save_as(self.config_file)


		if os.path.isfile(os.path.join(self.scan_dir,"scan_config.inp"))==False:
			a = open(os.path.join(self.scan_dir,"scan_config.inp"), "w")
			a.write("#scan_config_args\n")
			a.write("\n")
			a.write("#scan_config_compress\n")
			a.write("false\n")
			a.write("#end\n")

			a.close()

	def set_path(self,scan_dir):
		self.scan_dir=scan_dir

	def set_base_dir(self,base_dir):
		self.base_dir=base_dir

	def clean_dir(self):
		dirs_to_del=[]
		listing=os.listdir(self.scan_dir)

		for i in range(0,len(listing)):
			full_path=os.path.join(self.scan_dir,listing[i])
			if os.path.isdir(full_path)==True:
				dirs_to_del.append(full_path)

		ask_to_delete(self.parent_window,dirs_to_del,interactive=self.interactive)

	def apply_constants_to_dir(self,folder):
		leaf=scan_tree_leaf()
		leaf.json_load(os.path.join(folder,"sim.json"))
		leaf.directory=folder
		leaf.program_list=self.program_list
		leaf.apply_constants()
		leaf.apply_python_scripts()
		leaf.json_save()

	def run(self,run_simulation=True,generate_simulations=True,args=""):
		f=inp()
		f.load(os.path.join(self.scan_dir,"scan_config.inp"))
		args=f.get_token("#scan_config_args")

		if args==False:
			args=""

		args=args+" --mindbustx"

		if self.scan_dir=="":
			error_dlg(self.parent_window,_("No sim dir name"))
			return

		self.make_dir()

		if generate_simulations==True:
			self.build_scan()

		if run_simulation==True:
			commands=tree_load_flat_list(self.scan_dir)
			if commands==False:
				error_dlg(self.parent_window,_("I can't load flat_list.inp.  This usually means there is a problem with how you have set up your scan."))
				return

			for i in range(0, len(commands)):
				self.myserver.add_job(commands[i],args)
				#print("Adding job"+commands[i])

			self.myserver.start()

		gc.collect()

	def build_scan(self):
		self.clean_dir()

		flat_simulation_list=[]

		path=os.getcwd()

		#print(self.scan_dir,flat_simulation_list,self.program_list,self.base_dir)
		if tree_gen(self.scan_dir,flat_simulation_list,self.program_list,self.base_dir)==False:
			error_dlg(self.parent_window,_("Problem generating tree."))
			return False
		os.chdir(path)

		tree_save_flat_list(self.scan_dir,flat_simulation_list)

	def make_dir(self):
		if os.path.isdir(self.scan_dir)==False:
			os.makedirs(self.scan_dir)

	def rename(self,new_name):
		new_path=os.path.join(os.path.dirname(self.scan_dir),new_name)
		f=inp()
		f.load(self.config_file)
		f.set_token("#scan_name",new_name)
		f.save()
		self.human_name=new_name

		shutil.move(self.scan_dir, new_path)
		self.scan_dir=new_path

	def clone(self,new_human,new_config_file):
		self.scan_dir=os.path.join(os.path.dirname(self.scan_dir),new_name)
		print(self.config_file)

