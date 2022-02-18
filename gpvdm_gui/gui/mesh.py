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

## @package mesh
#  Extra mesh function on top of json object
#


import os
from json_mesh import json_mesh_segment
from epitaxy import get_epi
from gpvdm_json import gpvdm_data

class mesh_zxy:
	def __init__(self,data,xyz):
		self.xyz=xyz
		self.data=data
		self.points=[]
		self.tot_points=0

	def calculate_points(self):
		out_x=[]
		out_y=[]
		epi=get_epi()
		self.tot_points=0
		pos=0.0
		if self.xyz=="y" and gpvdm_data().electrical_solver.solver_type=="circuit":
				for l in epi.layers:
					if l.layer_type=="active" or l.layer_type=="contact":
						out_x.append(pos)
						out_y.append(1.0)
						pos=pos+l.dy/2
						out_x.append(pos)
						out_y.append(1.0)
						pos=pos+l.dy/2

				out_x.append(pos)
				out_y.append(1.0)

		else:
			for l in self.data.segments:
				pos=0.0
				if l.points!=0:
					dx=l.len/l.points
					temp_x=[]
					while(pos<l.len):
						if pos+dx/2>l.len:
							break
						pos=pos+dx/2
						temp_x.append(pos)
						pos=pos+dx/2

						dx=dx*l.mul
						self.tot_points=self.tot_points+1
						if dx==0 or self.tot_points>2000:
							break

					l.mesh=[]
					for i in range(0,len(temp_x)):
						if l.left_right=="left":
							l.mesh.append((temp_x[i]))
						else:
							l.mesh.append((l.len-temp_x[i]))

					l.mesh.sort()
					#print(l.mesh)


			out_x=[]
			out_y=[]
			last_l=0.0
			for l in self.data.segments:
				#print(l.mesh,last_l)
				for p in l.mesh:
					out_x.append(p+last_l)

					out_y.append(1.0)
				last_l=last_l+l.len
		self.points=out_x

		return out_x,out_y

	def do_remesh(self,to_size):
		if len(self.data.segments)!=1:
			return False

		if self.data.segments[0].len!=to_size:
			print("changing ",self.data.segments[0].len,to_size)

			self.data.segments[0].len=to_size

			return True


class mesh:
	def __init__(self):
		self.x=mesh_zxy(gpvdm_data().mesh.mesh_x,"x")
		self.y=mesh_zxy(gpvdm_data().mesh.mesh_y,"y")
		self.z=mesh_zxy(gpvdm_data().mesh.mesh_z,"z")
	

mesh_data=mesh()
def get_mesh():
	global mesh_data
	return mesh_data


