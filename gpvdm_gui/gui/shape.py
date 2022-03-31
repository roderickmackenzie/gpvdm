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

## @package shape
#  Shape object file.
#

import os
from str2bool import str2bool


from cal_path import gpvdm_paths
from dat_file import dat_file

from triangle_io import triangles_get_min
from triangle_io import triangles_get_max
from triangle_io import triangles_sub_vec
from triangle_io import triangles_div_vec
from triangle_io import triangles_mul_vec

from triangle import vec
from triangle_io import triangles_print 
from inp import inp
from json_base import json_base
from json_electrical import json_electrical
from math import fabs

class shape_homo_lumo_item(json_base):
	def __init__(self):
		json_base.__init__(self,"shape_homo_lumo_item")
		self.var_list=[]
		self.var_list.append(["function","exp"])
		self.var_list.append(["function_enable",True])
		self.var_list.append(["function_a",4.207452e+20])
		self.var_list.append(["function_b",100e-3])
		self.var_list.append(["function_c",0.0])
		self.var_list_build()

class shape_homo_lumo:
	def __init__(self,name):
		self.segments=[]
		self.segments.append(shape_homo_lumo_item())
		self.segments.append(shape_homo_lumo_item())
		self.name=name

	def gen_json(self):
		out=[]
		out.append("\""+self.name+"\": {")
		out.append("\"segments\":"+str(len(self.segments))+",")
		i=0
		for s in self.segments:
			s.base_name="function"+str(i)
			out.extend(s.gen_json())
			out[-1]=out[-1]+","
			i=i+1
		out[-1]=out[-1][:-1]
		out.append("}")

		return out

	def find_object_path_by_id(self,want_id,cur_path=""):
		return None,None

	def load_from_json(self,json):
		self.segments=[]
		nsegments=json['segments']
		for i in range(0,nsegments):
			a=shape_homo_lumo_item()
			function_name="function"+str(i)
			a.load_from_json(json[function_name])
			self.segments.append(a)

class shape_dos(json_base):
	def __init__(self):
		json_base.__init__(self,"shape_dos")
		self.var_list=[]
		self.var_list.append(["enabled",False])

		#free carrier
		self.var_list.append(["text_free_carrier",""])
		self.var_list.append(["symmetric_mobility_e","symmetric"])
		self.var_list.append(["mue_z",1e-15])
		self.var_list.append(["mue_x",1e-15])
		self.var_list.append(["mue_y",1.0e-05])
		self.var_list.append(["symmetric_mobility_h","symmetric"])
		self.var_list.append(["muh_z",1e-15])
		self.var_list.append(["muh_x",1e-15])
		self.var_list.append(["muh_y",1.0e-05])
		self.var_list.append(["Nc",5e25])
		self.var_list.append(["Nv",5e25])
		self.var_list.append(["free_to_free_recombination",0.000000e+00])
		self.var_list.append(["dos_free_carrier_stats","mb_equation"])

		#Non-dynamic srh
		self.var_list.append(["text_steay_srh",""])
		self.var_list.append(["ss_srh_enabled",False])
		self.var_list.append(["srh_n1",1e20])
		self.var_list.append(["srh_p1",1e20])
		self.var_list.append(["srh_tau_n",1e-15])
		self.var_list.append(["srh_tau_p",1e-15])

		#Dynamic traps
		self.var_list.append(["text_dynamic_traps",""])
		self.var_list.append(["dostype","exponential"])
		self.var_list.append(["complex_lumo",shape_homo_lumo("complex_lumo")])
		self.var_list.append(["complex_homo",shape_homo_lumo("complex_homo")])
		self.var_list.append(["Ntrape",1e20])
		self.var_list.append(["Ntraph",1e20])
		self.var_list.append(["Etrape",60e-3])
		self.var_list.append(["Etraph",60e-3])
		self.var_list.append(["ion_density",0.0])
		self.var_list.append(["ion_mobility",0.0])


		self.var_list.append(["doping_start",0.0])
		self.var_list.append(["doping_stop",0.0])
		self.var_list.append(["Na0",0.0])
		self.var_list.append(["Na1",0.0])
		self.var_list.append(["Nd0",0.0])
		self.var_list.append(["Nd1",0.0])
		self.var_list.append(["Tstart",300.0])
		self.var_list.append(["Tstop",305.0])
		self.var_list.append(["Tpoints",1])
		self.var_list.append(["nstart",-2.5])
		self.var_list.append(["nstop",1.0])
		self.var_list.append(["npoints",1000])
		self.var_list.append(["pstart",-2.5])
		self.var_list.append(["pstop",1.0])
		self.var_list.append(["ppoints",1000])

		self.var_list.append(["srh_start",-0.5])
		self.var_list.append(["srhsigman_e",2.131895e-21])
		self.var_list.append(["srhsigmap_e",3.142822e-22])
		self.var_list.append(["srhvth_e",1e5])
		self.var_list.append(["srhsigman_h",3.142822e-22])
		self.var_list.append(["srhsigmap_h",2.131895e-21])
		self.var_list.append(["srhvth_h",1e5])
		self.var_list.append(["srh_bands",5])

		self.var_list.append(["Esteps",1000])
		self.var_list.append(["dump_band_structure",0])


		#Auger
		self.var_list.append(["text_auger",""])
		self.var_list.append(["dos_enable_auger",False])
		self.var_list.append(["dos_auger_Cn",1e-26])
		self.var_list.append(["dos_auger_Cp",1e-26])

		self.var_list.append(["text_electro",""])
		self.var_list.append(["Xi",1.6])
		self.var_list.append(["Eg",1.2])
		self.var_list.append(["epsilonr",5.0])
		self.var_list.append(["id",self.random_id()])

		#Exciton solver
		self.var_list.append(["text_exciton",""])
		self.var_list.append(["exciton_enabled",False])
		self.var_list.append(["exciton_L",1e-08])
		self.var_list.append(["exciton_tau",1e-10])
		self.var_list.append(["exciton_kpl",4e9])
		self.var_list.append(["exciton_kfret",0.0])
		self.var_list.append(["exciton_alpha",1E-13])
		self.var_list.append(["exciton_kdis",1e11])

		#singlet solver
		self.var_list.append(["text_singlet",""])
		self.var_list.append(["singlet_enabled",False])
		self.var_list.append(["singlet_k_fret",1e10])
		self.var_list.append(["singlet_k_s",8e7])
		self.var_list.append(["singlet_k_isc",2.2e4])
		self.var_list.append(["singlet_k_ss",3.5e-18])
		self.var_list.append(["singlet_k_sp",3.0e-10])
		self.var_list.append(["singlet_k_st",1.9e-16])
		self.var_list.append(["singlet_k_dext",1e10])
		self.var_list.append(["singlet_k_t",6.5e2])
		self.var_list.append(["singlet_k_tp",2.8e-19])
		self.var_list.append(["singlet_k_tt",2.2e-18])
		self.var_list.append(["singlet_k_sd",1e9])
		self.var_list.append(["singlet_k_iscd",2.2e4])
		self.var_list.append(["singlet_k_spd",3.0e-16])
		self.var_list.append(["singlet_k_std",1.9e-16])
		self.var_list.append(["singlet_k_ssd",9.6e-19])
		self.var_list.append(["singlet_k_td",6.6e2])
		self.var_list.append(["singlet_k_ttd",2.4e-21])
		self.var_list.append(["singlet_k_tpd",5.6e-19])
		self.var_list.append(["singlet_gamma",0.29])
		self.var_list.append(["singlet_zeta",1.4e-11])
		self.var_list.append(["singlet_k_cav",1e12])
		self.var_list.append(["singlet_beta_sp",1e-4])
		self.var_list.append(["singlet_C",0.02])
		self.var_list.append(["singlet_N_dop",4.2e25])
		self.var_list.append(["singlet_W",2.6e-2])
		self.var_list_build()


class shape_pl(json_base):
	def __init__(self):
		json_base.__init__(self,"shape_pl")
		self.var_list=[]
		self.var_list.append(["pl_emission_enabled",False])
		self.var_list.append(["pl_use_experimental_emission_spectra",False])
		self.var_list.append(["pl_input_spectrum","none"])
		self.var_list.append(["pl_experimental_emission_efficiency",1.0])
		self.var_list.append(["pl_fe_fh",1.0])
		self.var_list.append(["pl_fe_te",1.0])
		self.var_list.append(["pl_te_fh",1.0])
		self.var_list.append(["pl_th_fe",1.0])
		self.var_list.append(["pl_fh_th",1.0])
		self.var_list.append(["text_ray_tracing",False])
		self.var_list.append(["ray_theta_steps",200])
		self.var_list.append(["ray_theta_start",0])
		self.var_list.append(["ray_theta_stop",360])
		self.var_list.append(["ray_phi_steps",5])
		self.var_list.append(["ray_phi_start",0])
		self.var_list.append(["ray_phi_stop",360])
		self.var_list.append(["ray_emission_source","ray_emission_single_point"])
		self.var_list_build()

class shape_heat(json_base):
	def __init__(self):
		json_base.__init__(self,"shape_heat")
		self.var_list=[]
		self.var_list.append(["thermal_kl",1.0])
		self.var_list.append(["thermal_tau_e",1.0])
		self.var_list.append(["thermal_tau_h",1.0])
		self.var_list_build()

class shape_display_options(json_base):
	def __init__(self):
		json_base.__init__(self,"display_options")
		self.var_list=[]
		self.var_list.append(["show_solid",True])
		self.var_list.append(["show_mesh",True])
		self.var_list.append(["show_cut_through_x",False])
		self.var_list.append(["show_cut_through_y",False])
		self.var_list.append(["hidden",False])
		self.var_list_build()

class shape(json_base):

	def __init__(self):
		json_base.__init__(self,"shape")
		self.var_list=[]
		self.var_list.append(["shape_enabled",True])
		self.var_list.append(["display_options",shape_display_options()])
		self.var_list.append(["shape_type","box"])
		self.var_list.append(["rotate_y",0])
		self.var_list.append(["rotate_x",0])


		self.var_list.append(["dx",1e-9])
		self.var_list.append(["dy",1e-9])
		self.var_list.append(["dz",1e-9])

		self.var_list.append(["x0",0.0])
		self.var_list.append(["z0",0.0])
		self.var_list.append(["y0",0.0])

		self.var_list.append(["dx_padding",0.0])
		self.var_list.append(["dy_padding",0.0])
		self.var_list.append(["dz_padding",0.0])

		self.var_list.append(["shape_nx",1])
		self.var_list.append(["shape_ny",1])
		self.var_list.append(["shape_nz",1])

		self.var_list.append(["shape_dos",shape_dos()])
		self.var_list.append(["shape_electrical",json_electrical()])

		self.var_list.append(["shape_remove_layer",False])
		self.var_list.append(["color_r",0.8])
		self.var_list.append(["color_g",0.8])
		self.var_list.append(["color_b",0.8])
		self.var_list.append(["color_alpha",0.8])

		self.var_list.append(["optical_material","blends/p3htpcbm"])
		self.var_list.append(["shape_pl",shape_pl()])
		self.var_list.append(["shape_heat",shape_heat()])
		self.var_list.append(["shape_name","none"])
		self.var_list.append(["Gnp",0.0])
		self.var_list.append(["moveable",False])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()
		self.loaded_from_json=False
		self.triangles=None
		self.file_name=None
		self.shape_path=""
		self.shapes=[]

	def decode_from_json(self,json):
		self.load_from_json(json)
		#self.shape_enabled=str2bool(self.shape_enabled)
		self.loaded_from_json=True
		#backwards compatability take out after 22/08/22
		if self.shape_dos.doping_start>0.0:
			self.shape_dos.Nd0=fabs(self.shape_dos.doping_start)
			self.shape_dos.Na0=0.0

		if self.shape_dos.doping_stop>0.0:
			self.shape_dos.Nd1=fabs(self.shape_dos.doping_stop)
			self.shape_dos.Na1=0.0

		if self.shape_dos.doping_start<0.0:
			self.shape_dos.Na0=fabs(self.shape_dos.doping_start)
			self.shape_dos.Nd0=0.0
			

		if self.shape_dos.doping_stop<0.0:
			self.shape_dos.Na1=fabs(self.shape_dos.doping_stop)
			self.shape_dos.Nd1=0.0

		self.shape_dos.doping_start=0.0
		self.shape_dos.doping_stop=0.0


	def load_triangles(self):
		self.shape_path=os.path.join(gpvdm_paths.get_shape_path(),self.shape_type,"shape.inp")
		if os.path.isfile(self.shape_path)==True:
			self.triangles=dat_file()
			self.triangles.load(self.shape_path)
			#for t in self.triangles.data:
			#	print(t.gen_json())
			if self.triangles.data!=None:
				min_vec=triangles_get_min(self.triangles.data)
				self.triangles.data=triangles_sub_vec(self.triangles.data,min_vec)
				max_vec=triangles_get_max(self.triangles.data)
				self.triangles.data=triangles_div_vec(self.triangles.data,max_vec)
				
	def find_object_by_id(self,id):
		if self.id==id:
			return self

		if self.shape_dos.id==id:
			return self.shape_dos

		if self.shape_electrical.id==id:
			return self.shape_electrical

		return None

	def dump(self):
		print(self.file_name,self.shape_type,self.width)

	def expand_xyz0(self,epi_layer):
		vectors=[]

		for x in range(0,self.shape_nx):
			for y in range(0,self.shape_ny):
				for z in range(0,self.shape_nz):
						pos=vec()
						pos.x=(self.x0+(self.dx+self.dx_padding)*x)
						pos.y=(self.y0+(self.dy+self.dy_padding)*y)
						pos.z=(self.z0+(self.dz+self.dz_padding)*z)

						vectors.append(pos)
						if self.shape_enabled==False:
							break
		return vectors

	def shape_gen_json(self):
		lines=[]
		self.include_name=False
		lines=self.gen_json()

		return lines

	def get_min_max(self,my_min,my_max):
		#x
		if self.x0<my_min.x:
			my_min.x=self.x0
		if self.x0>my_max.x:
			my_max.x=self.x0

		if self.x0+self.dx<my_min.x:
			my_min.x=self.x0+self.dx
		if self.x0+self.dx>my_max.x:
			my_max.x=self.x0+self.dx

		#y
		if self.y0<my_min.y:
			my_min.y=self.y0
		if self.y0>my_max.y:
			my_max.y=self.y0

		if self.y0+self.dy<my_min.y:
			my_min.y=self.y0+self.dy
		if self.y0+self.dy>my_max.y:
			my_max.y=self.y0+self.dy

		#z
		if self.z0<my_min.z:
			my_min.z=self.z0
		if self.z0>my_max.z:
			my_max.z=self.z0

		if self.z0+self.dz<my_min.z:
			my_min.z=self.z0+self.dz
		if self.z0+self.dz>my_max.z:
			my_max.z=self.z0+self.dz

		return my_min,my_max


