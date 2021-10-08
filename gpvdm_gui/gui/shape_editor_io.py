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

## @package shape_editor_io
#  This builds the images which are then used to generate shapes
#

import os

from PIL import Image, ImageFilter,ImageOps, ImageDraw
from PIL.ImageQt import ImageQt

from cal_path import get_exe_command
from server import server_get
from cal_path import get_sim_path

from math import sin
from math import cos
from math import exp
from math import pow

from json_shape_db_item import shape_db_item

class shape_editor_io(shape_db_item):

	def __init__(self):
		shape_db_item.__init__(self)

	def gen_poly(self,x0,y0,dx0,dy0):
		ret=[]
		start=0.0
		stop=360
		steps=6
		dphi_deg=(stop-start)/steps
		phi_deg=start
		while (phi_deg<stop):
			phi=3.1415926*2*phi_deg/360
			dx=sin(phi)*dx0
			dy=cos(phi)*dy0
			ret.append((x0+dx, y0+dy))
			phi_deg=phi_deg+dphi_deg
			#print(phi_deg)
		ret.append(ret[0])
		x_min=1e6
		x_max=0
		y_min=1e6
		y_max=0

		for r in ret:
			if x_min>r[0]:
				x_min=r[0]

			if x_max<r[0]:
				x_max=r[0]

			if y_min>r[1]:
				y_min=r[1]

			if y_max<r[1]:
				y_max=r[1]

		w=x_max-x_min
		#h=y_max-y_min
		h=abs(ret[0][1]-ret[1][1])+abs(ret[1][1]-ret[2][1])
		#seg_len=pow(pow((ret[0][0]-ret[1][0]),2.0)+pow((ret[0][1]-ret[1][1]),2.0),0.5)
		return w,h,ret

	def apply_boundary(self,im):
		x0=self.boundary.image_boundary_x0
		x1=self.boundary.image_boundary_x1
		y0=self.boundary.image_boundary_y0
		y1=self.boundary.image_boundary_y1
		w, h = im.size
		dr=ImageDraw.Draw(im)
		dr.rectangle([(0, 0), (x0, h)], fill="white")
		dr.rectangle([(w-x1, 0), (w, h)], fill="white")

		if y0!=0:
			dr.rectangle([(0, 0), (w, y0)], fill="white")

		if y1!=0:
			dr.rectangle([(0, h-y1), (w, h)], fill="white")


	def draw_honeycomb(self):
		path=os.path.dirname(self.file_name)
		dx=self.honeycomb.honeycomb_dx
		dy=self.honeycomb.honeycomb_dy
		shift_x=self.honeycomb.honeycomb_x_shift
		shift_y=self.honeycomb.honeycomb_y_shift
		line_width=self.honeycomb.honeycomb_line_width

		im= Image.new("RGB", (self.image_xlen, self.image_ylen), "#000000")

		x_start=-20
		x_stop=self.image_xlen
		n_x=(x_stop-x_start)/dx

		y_start=-20
		y_stop=self.image_ylen
		n_y=(y_stop-y_start)/dy

		x_pos=0
		y_pos=0
		delta=0.0
		while(y_pos<y_stop):
			x_pos=delta
			while(x_pos<x_stop):
				#ImageDraw.Draw(im).polygon(, width=7)
				w,h,points=self.gen_poly(shift_x+x_pos,shift_y+y_pos,dx,dy)
				dr=ImageDraw.Draw(im)
				dr.line(points, fill="white", width=line_width)

				x_pos=x_pos+w

			#v_seg_len=2*dy
			y_pos=y_pos+h

			if delta==0.0:
				delta=w/2
			else:
				delta=0

		self.apply_boundary(im)
		#im=self.apply_rotate(im)
		im=self.apply_blur(im)
		im.save(os.path.join(path,"image.png"))

	def apply_rotate(self,im):
		rotate=self.import_config.shape_import_rotate
		if rotate!=0:
			im=im.rotate(360-rotate)
		return im

	def add_job_to_server(self,sim_path,server):
		path=os.path.dirname(self.file_name)
		server.add_job(sim_path,"--simmode data@mesh_gen --path "+path)

	def apply_blur(self,im):
		if self.blur.shape_import_blur_enabled==True:
			im = im.filter(ImageFilter.GaussianBlur(radius = self.blur.shape_import_blur))
		return im

	def draw_gauss(self):
		path=os.path.dirname(self.file_name)

		sigma=self.gauss.gauss_sigma
		gauss_offset_x=self.gauss.gauss_offset_x
		gauss_offset_y=self.gauss.gauss_offset_y

		im= Image.new("RGB", (self.image_xlen, self.image_ylen), "#FF0000")

		for y in range(0,self.image_ylen):
			for x in range(0,self.image_xlen):
				mag=int(255*exp(-((x-gauss_offset_x-self.image_xlen/2)/sigma)**2 -((y-gauss_offset_y-self.image_ylen/2)/sigma)**2))

				im.putpixel((x,y),(mag, mag, mag))

		im=self.apply_rotate(im)
		im=self.apply_blur(im)
		im.save(os.path.join(path,"image.png"))
		
