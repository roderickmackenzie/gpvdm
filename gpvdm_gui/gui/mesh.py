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

## @package mesh
#  The underlying mesh class.
#


#import sys
import os
#import shutil
from inp import inp_save

from cal_path import get_sim_path
from str2bool import str2bool
from inp import inp

from newton_solver import newton_solver_get_type

class mlayer():
	def __init__(self):
		self.thick=0.0
		self.points=0
		self.mul=1.0
		self.mesh=[]
		self.left_right="left"
		self.start=0.0
		self.end=0.0

class mesh_zxy:
	def __init__(self,direction):
		self.direction=direction
		self.layers=[]
		self.remesh=False
		self.points=[]
		self.circuit_model=False
		self.tot_points=0
		self.epi=None

	def calculate_points(self):
		total_pos=0.0
		out_x=[]
		out_y=[]

		self.tot_points=0
		pos=0.0
		if self.circuit_model==True:
			for i in range(0,len(self.epi.layers)):
				l=self.epi.layers[i]
				out_x.append(pos)
				out_y.append(1.0)
				pos=pos+l.dy/2
				out_x.append(pos)
				out_y.append(1.0)
				pos=pos+l.dy/2

			out_x.append(pos)
			out_y.append(1.0)

		else:
			for i in range(0,len(self.layers)):
				l=self.layers[i]

				layer_length=l.thick
				layer_points=l.points
				layer_mul=l.mul
				layer_left_right=l.left_right

				if layer_points!=0:
					dx=layer_length/layer_points
					pos=0.0
					ii=0
					temp_x=[]
					temp_mag=[]
					while(pos<layer_length):
						pos=pos+dx/2
						temp_x.append(pos)
						temp_mag.append(1.0)
						pos=pos+dx/2

						#pos=pos+dx*pow(layer_mul,ii)

						ii=ii+1
						dx=dx*layer_mul
						self.tot_points=self.tot_points+1
						if dx==0 or self.tot_points>2000:
							break

					l.mesh=[]
					for i in range(0,len(temp_x)):
						if layer_left_right=="left":
							l.mesh.append((temp_x[i]+total_pos))
						else:
							l.mesh.append((layer_length-temp_x[i]+total_pos))


						out_y.append(temp_mag[i])

					l.mesh.sort()

				total_pos=total_pos+layer_length

			out_x=[]	
			for l in self.layers:
				out_x.extend(l.mesh)

			#out_x.sort()

		self.points=out_x
		return out_x,out_y

	def mesh_cal_epi_layers(self,epi):
		ret=[]
		device_start=epi.get_device_start()
		layer=epi.get_next_dos_layer(-1)
		for p in self.points:
			if p+device_start>epi.layers[layer].end:
				layer=layer+1

			ret.append(layer)

		return ret

	def check_curcuit_sim(self,epi):
		newton_solver=newton_solver_get_type()

		if newton_solver=="newton_simple" and self.direction=="y":
			for l in epi.layers:
				self.add_layer(l.dy,1,1.0,"left")
			self.circuit_model=True
			return True

		return False

	def load(self,epi):
		self.epi=epi
		self.file=inp()
		file_name=os.path.join(get_sim_path(),"mesh_"+self.direction+".inp")

		self.clear()

		if self.check_curcuit_sim(epi)==True:
			self.update()
			return True

		if self.file.load(file_name)!=False:

			remesh=str2bool(self.file.get_next_val())

			self.remesh=remesh

			mesh_layers=int(self.file.get_next_val())

			for i in range(0, mesh_layers):
				#thick
				thick=float(self.file.get_next_val())	#length

				points=float(self.file.get_next_val()) 	#points
			
				mul=float(self.file.get_next_val()) 		#mul

				left_right=self.file.get_next_val() 		#left_right

				self.add_layer(thick,points,mul,left_right)

		self.update()
		return True

	def clear(self):
		self.remesh=True
		self.layers=[]


	def add_layer(self,thick,points,mul,left_right):
		if len(self.layers)==0:
			start=0.0
			end=thick
		else:
			start=self.layers[-1].end
			end=self.layers[-1].end+thick

		a=mlayer()
		a.thick=thick
		a.points=points
		a.mul=mul
		a.left_right=left_right
		a.start=start
		a.end=end
		self.layers.append(a)

	def update(self):
		self.calculate_points()

	def save(self):
		file_name=os.path.join(get_sim_path(),"mesh_"+self.direction+".inp")
		lines=[]
		lines.append("#remesh_enable")
		lines.append(str(self.remesh))	
		lines.append("#mesh_layers")
		lines.append(str(len(self.layers)))
		i=0
		for item in self.layers:
			lines.append("#mesh_layer_length"+str(i))
			lines.append(str(item.thick))
			lines.append("#mesh_layer_points"+str(i))
			lines.append(str(item.points))
			lines.append("#mesh_layer_mul"+str(i))
			lines.append(str(item.mul))
			lines.append("#mesh_layer_left_right"+str(i))
			lines.append(str(item.left_right))
			i=i+1
		lines.append("#ver")
		lines.append("1.0")
		lines.append("#end")
		inp_save(file_name,lines,id="mesh")

	def do_remesh(self,to_size):
		if len(self.layers)!=1:
			return

		if self.layers[0].thick!=to_size:
			print("changing ",self.layers[0].thick,to_size)

			self.layers[0].thick=to_size
			self.save()



class mesh:
	def __init__(self):
		self.x=mesh_zxy("x")
		self.y=mesh_zxy("y")
		self.z=mesh_zxy("z")

	def save(self):
		self.x.save()
		self.y.save()
		self.z.save()
	

	def load(self,epi):
		self.epi=epi
		ret=True
		r=self.x.load(epi)
		ret=ret and r
		r=self.y.load(epi)
		ret=ret and r
		r=self.z.load(epi)
		ret=ret and r
		return ret
	
	def set_xlen(self,value):
		if len(self.x.layers)==1:
			self.x.layers[0].thick=value
			return True
		else:
			return False

	def set_zlen(self,value):
		if len(self.z.layers)==1:
			self.z.layers[0].thick=value
			return True
		else:
			return False


	def get_xlen(self):
		tot=0.0
		for a in self.x.layers:
			tot=tot+a.thick
		return tot

	def get_ylen(self):
		tot=0.0
		for a in self.y.layers:
			tot=tot+a.thick
		return tot

	def get_zlen(self):
		tot=0.0
		for a in self.z.layers:
			tot=tot+a.thick
		return tot

	def get_xpoints(self):
		tot=0.0
		for a in self.x.layers:
			tot=tot+a.points
		return tot

	def get_ypoints(self):
		tot=0.0
		for a in self.y.layers:
			tot=tot+a.points
		return tot

	def get_zpoints(self):
		tot=0.0
		for a in self.z.layers:
			tot=tot+a.points
		return tot

	def get_xlayers(self):
		return len(self.x.layers)

	def get_ylayers(self):
		return len(self.y.layers)

	def get_zlayers(self):
		return len(self.z.layers)
		
	def clear(self):
		self.x.clear()
		self.y.clear()
		self.z.clear()

	def build_device_shadow(self):
		y,temp=self.y.calculate_points()
		x,temp=self.x.calculate_points()
		z,temp=self.z.calculate_points()
		ret=[]
		build_x=[]
		build_y=[]

		#print(self.epi.device_mask(x[0],y[14],z[0]))
		#import sys
		#sys.exit(0)
		for zi in range(0,len(z)):
			build_x=[]
			for xi in range(0,len(x)):
				build_y=[]
				for yi in range(0,len(y)):
					#print(self.x.tot_points,self.z.tot_points)
					if self.x.tot_points==1 and self.z.tot_points==1:
						val=True
					else:
						val=self.epi.device_mask(x[xi],y[yi],z[zi])
					build_y.append(val)
					#if xi==0:
						
				build_x.append(build_y)
				#sys.exit(0)
			ret.append(build_x)
		#print(ret)
		return ret


mesh_data=mesh()
def get_mesh():
	global mesh_data
	return mesh_data


