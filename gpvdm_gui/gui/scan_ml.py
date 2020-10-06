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

## @package scan_ml
#  ML framework.
#

import sys
import os
import shutil

from util import gpvdm_delete_file
from inp import inp_get_token_value
from scan_tree import tree_load_flat_list

from server_io import server_find_simulations_to_run
from util_zip import read_lines_from_archive
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


def make_vector_from_file(file_name,x_values):
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
	

	x, y = zip(*sorted(zip(x, y)))

	if x_values==None:
		return y

	r=np.interp(x_values,x,y)

	return r

def get_vectors(file_name,x_values,dolog=False,div=1.0,fabs=False,do_norm=False,mul=1.0):

	data=make_vector_from_file(file_name,x_values)

	if type(data)==bool:
		if data==False:
			return False
	

	if do_norm==True:
		mi=1e6
		for i in range(0,len(data)):
			if float(data[i])<mi:
				mi=float(data[i])
		div=mi

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



class scan_ml:

	def __init__(self,scan_dir):
		self.scan_dir=scan_dir
		self.jv_sample_points=[0.0,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]
		self.eqe_sample_points=[]
		start=300e-9
		stop=670e-9
		dl=(stop-start)/10.0
		pos=start
		while(pos<stop):
			self.eqe_sample_points.append(pos)
			pos=pos+dl
			
		scans=scans_io(os.path.dirname(scan_dir))
		config_file=scans.find_path_by_name(os.path.basename(scan_dir))

		self.scan=scan_io()
		self.scan.load(config_file)

	def get_sub_sims(self,archive_path):
		sub_sims=[]
		zf = zipfile.ZipFile(archive_path, 'r')
		items=zf.namelist()
		for file_name in items:
			if "sim.gpvdm" in file_name:
				parts=file_name.split("/")
				found_sim="/".join(parts[1:-1])

				if sub_sims.count(found_sim)==False:
					sub_sims.append(found_sim)

		return sub_sims

	def make_tmp_dir(self):
		rnd = [random.choice(string.ascii_letters + string.digits) for n in range(0,32)]
		rnd = "".join(rnd)
		tmp_dir="/dev/shm/gpvdm_"+rnd
		if os.path.isdir(tmp_dir)==True:
			shutil.rmtree(tmp_dir)

		os.mkdir(tmp_dir)

		return tmp_dir

	def build_vector(self):
		output_file=os.path.join(self.scan_dir,"vectors.dat")
		if os.path.isfile(output_file)==True:
			response=yes_no_cancel_dlg(None,"The file "+output_file+" already exists.  Continue? ")

			if response!="yes":
				sys.exit(0)

		
		out=open(output_file,'wb')

		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		tot_archives=0
		for archive_name in os.listdir(self.scan_dir):
			if archive_name.startswith("archive")==True and archive_name.endswith(".zip")==True:
				tot_archives=tot_archives+1

		done=0

		errors=0
		for archive_name in os.listdir(self.scan_dir):

			if archive_name.startswith("archive")==True and archive_name.endswith(".zip")==True:

				archive_path=os.path.join(self.scan_dir,archive_name)

				if done==0:		#on the first archive search it for sub sims and build a list
					sub_sims=self.get_sub_sims(archive_path)
					

				zf = zipfile.ZipFile(archive_path, 'r')
				simulations=zip_lsdir(archive_path,zf=zf,sub_dir="/")

				for simulation in simulations:

					tmp_dir=self.make_tmp_dir()

					extract_dir_from_archive(tmp_dir,"",simulation,zf=zf)
					
					written=False
					#print(simulation)

					error=False
					v=[]
					v.append("#ml_id")
					v.append(simulation)

					sub_sim_folder=None
					for scan_folder in sub_sims:
						sub_sim_folder=os.path.join(tmp_dir,scan_folder)
						#print(sub_sim_folder,tmp_dir)
						dolog=False
						div=1.0
						mul=1.0
						do_fabs=False

						sim_mode=inp_get_token_value(os.path.join(sub_sim_folder,"sim.inp"), "#simmode")
						if sim_mode==None:
							error=True
							break

						sim_mode=sim_mode.lower()

						light=float(inp_get_token_value(os.path.join(tmp_dir,scan_folder,"light.inp"), "#Psun"))
						scan_folder_token=scan_folder.replace("/","_")

						if sim_mode.endswith("jv") or sim_mode.startswith("jv"):
							file_name=["jv.dat"]
							sim_mode="jv"
							vector=[self.jv_sample_points]
							token_ext=[scan_folder_token]
							#if light>0.0:
							#	div=1e2

							#if light==0.0:
							#	dolog=True
						elif sim_mode.startswith("eqe") or sim_mode.endswith("qe"):
							file_name=["qe.dat"]
							sim_mode="eqe"
							vector=[self.eqe_sample_points]
							token_ext=[scan_folder_token]

						elif sim_mode=="sun_voc":
							file_name=["suns_voc.dat"]
							vector=[[0.02,0.04,0.05,0.1,0.5,0.7,1.0]]
							token_ext=[scan_folder_token]
							#dolog=True
							#mul=-10.0

						elif sim_mode.startswith("cv"):
							file_name=["cv.dat"]
							vector=[[-2.0, -1.8 ,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4]]
							token_ext=[scan_folder_token]

						elif sim_mode.startswith("is"):
							file_name=["fx_imag.dat"]
							vector=[self.fx_points]
							token_ext=[scan_folder_token+"_fx_imag"]

							file_name.append("fx_real.dat")
							vector.append(self.fx_points)
							token_ext.append(scan_folder_token+"_fx_real")

						elif sim_mode.startswith("imps"):
							file_name=["fx_imag.dat"]
							vector=[None]
							token_ext=[scan_folder_token+"_fx_imag"]

							file_name.append("fx_real.dat")
							vector.append(None)
							token_ext.append(scan_folder_token+"_fx_real")

							file_name.append("fx_phi.dat")
							vector.append(None)
							token_ext.append(scan_folder_token+"_fx_phi")

						elif sim_mode.startswith("imvs"):
							file_name=["fx_imag.dat"]
							vector=[self.fx_points]
							token_ext=[scan_folder_token+"_fx_imag"]

							file_name.append("fx_real.dat")
							vector.append(self.fx_points)
							token_ext.append(scan_folder_token+"_fx_real")

						elif sim_mode.startswith("tpc")==True:
							file_name=["time_i.dat"]
							vector=[[1.1e-6,2e-6,2e-5,1e-4,0.02,0.1]]
							token_ext=[scan_folder_token]
							#dolog=True
							#do_fabs=True

						elif sim_mode.startswith("celiv")==True:
							file_name=["time_i.dat"]
							vector=[[2e-6,3e-6,4e-6,5e-6,6e-6,7e-6,8e-6]]
							token_ext=[scan_folder_token]
							#do_fabs=True
							#mul=1000.0

						elif sim_mode.startswith("tpv")==True:
							file_name=["time_v.dat"]
							vector=[[10e-6,20e-6,30e-6,40e-6,50e-6,60e-6,80e-6]]
							token_ext=[scan_folder_token]
							#do_fabs=True
							#mul=10.0
						else:
							print(sim_mode)
							asdas

						for c in range(0,len(file_name)):
							ret=get_vectors(os.path.join(tmp_dir,scan_folder,file_name[c]),vector[c], dolog=dolog,div=div,mul=mul, fabs=do_fabs)
							#print(file_name[c],ret)
							if ret==False:
								error=True
								break

							v.append("#ml_input_"+token_ext[c])
							v.append(ret)

						if sim_mode=="jv" and light>0.0:
							for t in ["#jv_pmax_tau","#jv_pmax_mue","#pce"]:
								ret=inp_get_token_value(os.path.join(tmp_dir,scan_folder,"sim_info.dat"), t)
								if ret==False:
									error=True
									break

								v.append(t+"_"+scan_folder)
								v.append(ret)

					for p in self.scan.program_list:
						if p.values.count("random")>0:
							full_inp_file_name=os.path.join(tmp_dir,sub_sims[0],p.file)

							temp=inp_get_token_value(full_inp_file_name, p.token)
							if temp==False:
								error=True
								break
							else:
								v.append(p.token)
								v.append(temp)

					#print(v,error)
					if error==False:
						out.write(str.encode("\n".join(v)+"\n"))
						written=True
					else:
						errors=errors+1

					done=done+1

					progress_window.set_fraction(float(done)/float(len(simulations)*tot_archives))
					if written==True:
						progress_window.set_text(simulation)
					else:
						progress_window.set_text("                         /Last error: "+simulation+" tot errors="+str(errors)+" "+str(round(100.0*errors/done,1))+"%")

					progress_window.set_text(simulation)

					#if server_break()==True:
					#	break
					process_events()
					#return

					shutil.rmtree(tmp_dir)

		out.close()
		progress_window.stop()


