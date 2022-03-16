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

## @package scan_ml
#  ML framework.
#

import sys
import os
import shutil

from safe_delete import gpvdm_delete_file

from math import log10
from math import isnan

import i18n
_ = i18n.language.gettext

from progress_class import progress_class
from process_events import process_events
from server import server_break
from util_zip import zip_lsdir
from util_zip import extract_dir_from_archive

from inp import inp_get_token_value
import zipfile
import random
import string
import numpy as np
from scans_io import scans_io
from gui_util import yes_no_dlg
from scan_io import scan_io
from yes_no_cancel_dlg import yes_no_cancel_dlg
from gpvdm_json import gpvdm_data
from scan_human_labels import get_json_from_human_path
from inp import inp
from scan_human_labels import get_json_path_from_human_path
from scan_human_labels import json_get_val

from json_base import json_base
from json_diff import json_diff
import json
from scan_human_labels import json_find_sim_obj
from dat_file import dat_file

def make_vector_from_file(file_name,x_values,norm_y=False):
	if os.path.isfile(file_name)==True:
		f=open(file_name,'r')
		lines = f.readlines()
		f.close()
	else:
		return False

	x=[]
	y=[]
	for i in range(0,len(lines)):
		if lines[i].startswith("#")==False:

			if lines[i].count("nan")!=0:
				return False

			if lines[i].count("inf")!=0:
				return False

			r=lines[i].split()
			if len(r)==2:
				try:
					x.append(float(r[0]))
					y.append(float(r[1]))
				except:
					return False
	
	if norm_y==True:
		max_y=max(y)
		for i in range(0,len(y)):
			y[i]=y[i]/max_y
			#print(x[i],y[i])

	x, y = zip(*sorted(zip(x, y)))

	if x_values==None:
		return y

	r=np.interp(x_values,x,y)
	#if norm_y==True:
	#	print(r)
	return r

def get_vectors(file_name,x_values,dolog=False,div=1.0,fabs=False,norm_y=False,mul=1.0):

	data=make_vector_from_file(file_name,x_values,norm_y=norm_y)

	if type(data)==bool:
		if data==False:
			return False


	n=[]
	for i in range(0,len(data)):
		#print(data[i])
		r=float(data[i])/div

		if fabs==True:
			r=abs(r)

		if dolog==True:
			#print(r)
			if r!=0.0:
				r=log10(r)
			else:
				r=0.0

		r=r*mul
		n.append(r)
	#print(n)

	s=""
	for ii in range(0,len(n)):
		s=s+'{:e}'.format(float(n[ii]))+" "

	return s



class ml_vectors:

	def __init__(self):
		self.jv_sample_points=[0.0,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]
		self.sample_points_suns_voc=[0.02,0.04,0.05,0.1,0.5,0.7,1.0]
		self.sample_points_suns_jsc=[0.02,0.04,0.05,0.1,0.5,0.7,1.0]
		self.sample_points_cv=[-2.0, -1.8 ,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4]
		self.fx_sample_points=[1.000000,1.800000,3.240000,5.832000,1.049760e1,1.889568e1,3.401222e1,6.122200e1,1.101996e2,1.983593e2,3.570467e2,6.426841e2,1.156831e3,2.082296e3,3.748134e3,6.746641e3,1.214395e4,2.185912e4,3.934641e4,7.082353e4,1.274824e5,2.294683e5,4.130429e5,7.434771e5]
		self.eqe_sample_points=[]
		start=300e-9
		stop=670e-9
		dl=(stop-start)/10.0
		pos=start
		while(pos<stop):
			self.eqe_sample_points.append(pos)
			pos=pos+dl
			
	def json_get_ml_tokens(self,archive_path,sub_sim):
		zf = zipfile.ZipFile(archive_path, 'r')
		items=zf.namelist()
		sims=[]
		for file_name in items:
			if file_name.endswith("sim.json"):
				if os.path.basename(os.path.dirname(file_name))==sub_sim:
					sims.append(file_name)

		f0=inp()

		read_lines0 = zf.read(sims[0])
		read_lines1 = zf.read(sims[1])
		json_data0=json.loads(read_lines0)
		json_data1=json.loads(read_lines1)
		print(sims[0])
		print(sims[1])

		zf.close()
		return json_diff(json_data0,json_data1)


	def get_sub_sims(self,archive_path):
		sub_sims=[]
		zf = zipfile.ZipFile(archive_path, 'r')
		items=zf.namelist()
		for file_name in items:
			if file_name.endswith("sim.gpvdm"):
				sim_name=os.path.basename(os.path.dirname(file_name))

				if sim_name not in sub_sims:
					if sim_name!="base":
						sub_sims.append(sim_name)

		return sub_sims


	def make_tmp_dir(self):
		rnd = [random.choice(string.ascii_letters + string.digits) for n in range(0,32)]
		rnd = "".join(rnd)
		tmp_dir="/dev/shm/gpvdm_"+rnd
		if os.path.isdir(tmp_dir)==True:
			gpvdm_delete_file(tmp_dir,allow_dir_removal=True)

		os.mkdir(tmp_dir)

		return tmp_dir

	def build_vector(self,scan_dir,file_name):
		output_file=os.path.join(scan_dir,file_name)
		#if os.path.isfile(output_file)==True:
		#	response=yes_no_cancel_dlg(None,"The file "+output_file+" already exists.  Continue? ")

		#	if response!="yes":
		#		sys.exit(0)

		error_file=open(os.path.join(scan_dir,"errors.dat"),'w')
		error_file.write("")
		error_file.close()

		out_json=open(output_file,'wb')
		out_json.write(str.encode("{\n"))
		out_json.write(str.encode("\"item_type\":\"ml_vectors\",\n"))

		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		archives=[]
		for archive_name in os.listdir(scan_dir):
			if archive_name.startswith("archive")==True and archive_name.endswith(".zip")==True:
				archives.append(archive_name)


		done=0

		errors=0
		for archive_name in archives:

			if archive_name.startswith("archive")==True and archive_name.endswith(".zip")==True:

				archive_path=os.path.join(scan_dir,archive_name)

				if done==0:		#on the first archive search it for sub sims and build a list
					sub_sims=self.get_sub_sims(archive_path)
					self.ml_tokens=self.json_get_ml_tokens(archive_path,sub_sims[0])

				zf = zipfile.ZipFile(archive_path, 'r')
				simulations=zip_lsdir(archive_path,zf=zf,sub_dir="/")

				for simulation in simulations:

					tmp_dir="gpvdm_"+simulation

					extract_dir_from_archive(tmp_dir,"",simulation,zf=zf)
					
					written=False
					#print(simulation)

					error=False

					base=json_base(simulation)
				
					base.val_list=[]
					base.var_list.append(["params",json_base("params")])
					for sub_sim in sub_sims:
						base.var_list.append([sub_sim,json_base(sub_sim)])
					base.var_list_build()


					sub_sim_folder=None

					for sub_sim in sub_sims:
						sub_sim_folder=os.path.join(tmp_dir,sub_sim)
						#print(sub_sim_folder,tmp_dir)
						dolog=False
						div=1.0
						mul=1.0
						do_fabs=False
						d=gpvdm_data()
						norm_y=False
						#print(os.path.join(sub_sim_folder,"sim.json"))
						if d.load(os.path.join(sub_sim_folder,"sim.json"))==False:
							error=True
							break

						sim_mode=d.sim.simmode.lower()

						light=d.light.Psun

						if sim_mode.endswith("jv") or sim_mode.startswith("jv"):
							file_name=["jv.dat"]
							sim_mode="jv"
							vector=[self.jv_sample_points]
							token_ext=[sub_sim]
							#if light>0.0:
							#	div=1e2

							#if light==0.0:
							#	dolog=True
						elif sim_mode.endswith("eqe") or sim_mode.endswith("qe"):
							file_name=["qe.dat"]
							sim_mode="eqe"
							vector=[self.eqe_sample_points]
							token_ext=[sub_sim]

						elif sim_mode.endswith("suns_voc"):
							file_name=["suns_voc.dat"]
							vector=[self.sample_points_suns_voc]
							token_ext=[sub_sim]
							#dolog=True
							#mul=-10.0
						elif sim_mode.endswith("suns_jsc"):
							file_name=["suns_jsc.dat"]
							vector=[self.sample_points_suns_jsc]
							token_ext=[sub_sim]
							#dolog=True
							#mul=-10.0
						elif sim_mode.endswith("cv"):
							file_name=["cv.dat"]
							vector=[self.sample_points_cv]
							token_ext=[sub_sim]

						elif sim_mode.endswith("fx_domain"):
							obj=json_find_sim_obj(d,sim_mode)
							if obj.config.fx_modulation_type=="voltage" and obj.config.fxdomain_measure=="measure_current":	#IS
								file_name=["fx_imag.dat"]
								vector=[self.fx_sample_points]
								token_ext=[sub_sim+"_fx_imag"]

								file_name.append("fx_real.dat")
								vector.append(self.fx_sample_points)
								token_ext.append(sub_sim+"_fx_real")
							elif obj.config.fx_modulation_type=="optical" and obj.config.fxdomain_measure=="measure_current":	#IMPS
								file_name=["fx_C.dat"]
								vector=[self.fx_sample_points]
								token_ext=[sub_sim+"_fx_C"]

								file_name.append("fx_R.dat")
								vector.append(self.fx_sample_points)
								token_ext.append(sub_sim+"_fx_R")
							elif obj.config.fx_modulation_type=="optical" and obj.config.fxdomain_measure=="measure_voltage":	#IMVS
								file_name=["fx_C.dat"]
								vector=[self.fx_sample_points]
								token_ext=[sub_sim+"_fx_C"]

								file_name.append("fx_R.dat")
								vector.append(self.fx_sample_points)
								token_ext.append(sub_sim+"_fx_R")
							else:
								print(sim_mode)
								asdas
						elif sim_mode.endswith("time_domain"):
							obj=json_find_sim_obj(d,sim_mode)
							sim_name=obj.english_name.lower()
	
							if sim_name.startswith("tpc")==True:
								file_name=["time_i_abs.dat"]
								t=2e-8
								t_vectors=[]
								while (t<1e-3):
									#print("{:e}".format(pos))
									t=t*2.5
									t_vectors.append(t)
								vector=[t_vectors]
								token_ext=[sub_sim]
								norm_y=True
								#dolog=True
								#do_fabs=True
							else:
								print(sim_mode)
								asdas

						elif sim_mode.startswith("celiv")==True:
							file_name=["time_i_abs.dat"]
							vector=[[2e-6,3e-6,4e-6,5e-6,6e-6,7e-6,8e-6]]
							token_ext=[sub_sim]
							#do_fabs=True
							#mul=1000.0

						elif sim_mode.startswith("tpv")==True:
							file_name=["time_v.dat"]
							vector=[[10e-6,20e-6,30e-6,40e-6,50e-6,60e-6,80e-6]]
							token_ext=[sub_sim]
							#do_fabs=True
							#mul=10.0
						else:
							print(sim_mode)
							asdas

						for c in range(0,len(file_name)):
							vector_path=os.path.join(tmp_dir,sub_sim,file_name[c])
							ret=get_vectors(vector_path,vector[c], dolog=dolog,div=div,mul=mul, fabs=do_fabs,norm_y=norm_y)
							if ret==False:
								error=True
								error_file=open(os.path.join(scan_dir,"errors.dat"),'a')
								error_file.write(archive_path+"\\"+simulation+","+sub_sim+"\n")
								error_file.close()
								break
							getattr(base,sub_sim).var_list.append(["input"+str(c),ret])


						if sim_mode=="jv" and light>0.0:
							f=inp()
							json_data=f.load_json(os.path.join(tmp_dir,sub_sim,"sim_info.dat"))
							tokens=["pce","ff", "voc", "voc_R", "jsc", "theta_srh_free", "theta_srh_free_trap"]
							tokens.append("mu_jsc")
							tokens.append("mu_pmax")
							tokens.append("mu_voc")
							tokens.append("mu_geom_jsc")
							tokens.append("mu_geom_pmax")
							tokens.append("mu_geom_voc")
							tokens.append("mu_geom_micro_jsc")
							tokens.append("mu_geom_micro_pmax")
							tokens.append("mu_geom_micro_voc")
							tokens.append("mue_jsc")
							tokens.append("muh_jsc")
							tokens.append("mue_pmax")
							tokens.append("muh_pmax")
							tokens.append("tau_voc")
							tokens.append("tau_pmax")
							tokens.append("tau_all_voc")
							tokens.append("tau_all_pmax")

							for t in tokens:
								val=json_get_val(json_data,t)
								if val==None:
									error=True
									break
								getattr(base,sub_sim).var_list.append([t,val])

					f=inp()
					json_file_name=os.path.join(tmp_dir,sub_sims[0],"sim.json")
					json_data=f.load_json(json_file_name)

					base.params.var_list=[]
					for token_path in self.ml_tokens:
						temp=str(json_get_val(json_data,token_path))
						base.params.var_list.append([token_path,temp])
						#print(simulation,token_path,temp,json_file_name)

					#print(v,error)
					if error==False:
						base.params.var_list_build()
						for sub_sim in sub_sims:
							getattr(base,sub_sim).var_list_build()
						base.var_list_build()
						out_json.write(str.encode("\n".join(base.gen_json())+",\n"))
						written=True
					else:
						errors=errors+1

					done=done+1

					progress_window.set_fraction(float(done)/float(len(simulations)*len(archives)))
					if written==True:
						progress_window.set_text(simulation)
					else:
						progress_window.set_text("                         /Last error: "+simulation+" tot errors="+str(errors)+" "+str(round(100.0*errors/done,1))+"%")

					progress_window.set_text(simulation)

					#if server_break()==True:
					#	break
					process_events()
					#return

					gpvdm_delete_file(tmp_dir,allow_dir_removal=True)

		out_json.seek(-2, os.SEEK_END)
		out_json.write(str.encode("\n}"))
		out_json.close()
		progress_window.stop()


