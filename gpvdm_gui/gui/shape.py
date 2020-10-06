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

## @package shape
#  Shape object file.
#

import os
from cal_path import get_sim_path
from str2bool import str2bool


from cal_path import get_shape_path
from dat_file import dat_file

from triangle_io import triangles_get_min
from triangle_io import triangles_get_max
from triangle_io import triangles_sub_vec
from triangle_io import triangles_div_vec
from triangle_io import triangles_mul_vec
from triangle import vec
from triangle_io import triangles_print 
from inp import inp
import codecs

class shape():

	def __init__(self):

		self.shape_enabled=True
		self.type="none"
		self.triangles=None
		self.dx=1e-9
		self.dy=1e-9
		self.dz=1e-9

		self.x0=0.0
		self.z0=0.0
		self.y0=0.0

		self.dx_padding=0.0
		self.dy_padding=0.0
		self.dz_padding=0.0

		self.shape_nx=1
		self.shape_ny=1
		self.shape_nz=1
		self.file_name=None
		self.shape_dos="none"
		self.shape_electrical="none"
		self.name="none"
		self.shape_x0=0.0
		self.shape_y0=0.0
		self.shape_z0=0.0
		self.shape_remove_layer=False

		self.r=0.8
		self.g=0.8
		self.b=0.8
		self.alpha=1.0

		self.shape_path=""

		self.shape_flip_y=False
		self.shape_flip_x=False

		self.optical_material="none"
		self.id=codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()

		self.pl_file=""
		self.dos_file="none"
		self.lumo_file="none"
		self.homo_file="none"
		self.shapes=[]

	def do_load(self):
		f=inp()
		#print(self.file_name+".inp")
		if f.load(self.file_name+".inp")==False:
			print("shape file not found: ",self.file_name)
			return

		self.shape_enabled=str2bool(f.get_token("#shape_enabled"))

		self.type=f.get_token("#shape_type")


		self.dx=float(f.get_token("#shape_dx"))
		self.dy=float(f.get_token("#shape_dy"))
		self.dz=float(f.get_token("#shape_dz"))

		self.load_triangles()

		try:		
			self.dx_padding=float(f.get_token("#shape_padding_dx"))
			self.dy_padding=float(f.get_token("#shape_padding_dy"))
			self.dz_padding=float(f.get_token("#shape_padding_dz"))

			rgb=f.get_array("#red_green_blue")
			self.r=float(rgb[0])
			self.g=float(rgb[1])
			self.b=float(rgb[2])


			self.shape_nx=int(f.get_token("#shape_nx"))
			self.shape_ny=int(f.get_token("#shape_ny"))
			self.shape_nz=int(f.get_token("#shape_nz"))
			self.name=f.get_token("#shape_name")
			self.shape_dos=f.get_token("#shape_dos")
			self.shape_electrical=f.get_token("#shape_electrical")
			self.x0=float(f.get_token("#shape_x0"))
			self.y0=float(f.get_token("#shape_y0"))
			self.z0=float(f.get_token("#shape_z0"))

			self.shape_flip_y=str2bool(f.get_token("#shape_flip_y"))
			self.shape_flip_x=str2bool(f.get_token("#shape_flip_x"))

			self.optical_material=f.get_token("#shape_optical_material")
			self.optical_material.replace("\\", "/")

			#self.y0=0.0

			self.shape_remove_layer=str2bool(f.get_token("#shape_remove_layer"))
		except:
			pass

	def load_triangles(self):
		#print("reload")
		self.shape_path=os.path.join(get_shape_path(),self.type,"shape.inp")
		if os.path.isfile(self.shape_path)==True:
			self.triangles=dat_file()
			self.triangles.load(self.shape_path)
			if self.triangles.data!=None:
				min_vec=triangles_get_min(self.triangles.data)

				self.triangles.data=triangles_sub_vec(self.triangles.data,min_vec)

				max_vec=triangles_get_max(self.triangles.data)

				self.triangles.data=triangles_div_vec(self.triangles.data,max_vec)

	def load(self,file_name):
		if file_name=="none":
			return

		if file_name.endswith(".inp")==True:
			file_name=file_name[:-4]

		self.file_name=os.path.basename(file_name)
		self.do_load()


	def save(self):
		if self.file_name==None:
			return

		lines=[]
		lines.append("#shape_enabled")
		lines.append(str(self.shape_enabled))
		lines.append("#shape_type")
		lines.append(self.type)
		lines.append("#shape_dx")
		lines.append(str(self.dx))
		lines.append("#shape_dy")
		lines.append(str(self.dy))
		lines.append("#shape_dz")
		lines.append(str(self.dz))
		lines.append("#shape_padding_dx")
		lines.append(str(self.dx_padding))
		lines.append("#shape_padding_dy")
		lines.append(str(self.dy_padding))
		lines.append("#shape_padding_dz")
		lines.append(str(self.dz_padding))
		lines.append("#shape_nx")
		lines.append(str(self.shape_nx))
		lines.append("#shape_ny")
		lines.append(str(self.shape_ny))
		lines.append("#shape_nz")
		lines.append(str(self.shape_nz))
		lines.append("#shape_name")
		lines.append(self.name)
		lines.append("#shape_dos")
		lines.append(str(self.shape_dos))
		lines.append("#shape_electrical")
		lines.append(str(self.shape_electrical))
		lines.append("#shape_optical_material")
		lines.append(self.optical_material)
		lines.append("#shape_x0")
		lines.append(str(self.x0))
		lines.append("#shape_y0")
		lines.append(str(self.y0))
		lines.append("#shape_z0")
		lines.append(str(self.z0))
		lines.append("#shape_remove_layer")
		lines.append(str(self.shape_remove_layer))
		lines.append("#shape_flip_y")
		lines.append(str(self.shape_flip_y))
		lines.append("#shape_flip_x")
		lines.append(str(self.shape_flip_x))
		lines.append("#red_green_blue")
		lines.append(str(self.r))
		lines.append(str(self.g))
		lines.append(str(self.b))
		lines.append("#ver")
		lines.append("1.0")
		lines.append("#end")

		i=inp()
		i.lines=lines
		i.save_as(os.path.join(get_sim_path(),self.file_name+".inp"))
		#print("save",os.path.join(get_sim_path(),self.file_name+".inp"))

	def dump(self):
		print(self.file_name,self.type,self.width)

	def expand_xyz0(self,epi_layer):
		vectors=[]

		for x in range(0,self.shape_nx):
			for y in range(0,self.shape_ny):
				for z in range(0,self.shape_nz):
					if self.shape_enabled==True:
						pos=vec()
						if self.shape_flip_y==True:
							pos.x=(self.x0+(self.dx+self.dx_padding)*x)
							pos.y=epi_layer.end-(self.y0+(self.dy+self.dy_padding)*y)
							pos.z=(self.z0+(self.dz+self.dz_padding)*z)
						else:
							pos.x=(self.x0+(self.dx+self.dx_padding)*x)
							pos.y=(self.y0+(self.dy+self.dy_padding)*y)+epi_layer.start
							pos.z=(self.z0+(self.dz+self.dz_padding)*z)

						vectors.append(pos)
		return vectors

	def get_sub_files(self):
		ret=[]
		if self.dos_file.startswith("dos"):
			ret.append(self.dos_file+".inp")

		#pl files
		if self.file_name!="none" and self.file_name!="":
			ret.append(self.file_name+".inp")

		#pl files
		if self.pl_file!="none" and self.pl_file!="":
			ret.append(self.pl_file+".inp")

		#lumo files
		if self.lumo_file!="none" and self.lumo_file!="":
			ret.append(self.lumo_file+".inp")

		#homo files
		if self.homo_file!="none" and self.homo_file!="":
			ret.append(self.homo_file+".inp")

		#electrical files
		if self.shape_electrical!="none" and self.shape_electrical!="":
			ret.append(self.shape_electrical+".inp")

		for s in self.shapes:
			ret.extend(s.get_sub_files())

		return ret
