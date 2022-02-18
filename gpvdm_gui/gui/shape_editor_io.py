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
from math import sqrt

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

	def draw_honeycomb(self):
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

		im=im.rotate(self.honeycomb.honeycomb_rotate)


		return im

	def draw_xtal(self):
		dx=self.xtal.xtal_dx
		dy=self.xtal.xtal_dy
		offset=self.xtal.xtal_offset
		dr=self.xtal.xtal_dr

		im= Image.new("RGB", (self.image_xlen, self.image_ylen), "#000000")
		
		x=dx/2
		y=dy/2
		shift=False
		while(y<self.image_ylen):
			x=0
			if shift==True:
				x=x+offset
			while(x<self.image_xlen):
				drawer=ImageDraw.Draw(im)
				drawer.ellipse([(x-dr, y-dr), (x+dr, y+dr)], fill="white")
				x=x+dx

			shift = not shift

			y=y+dy

		return im

	def draw_lens(self):
		convex=True
		if self.lens.lens_type=="convex":
			convex=True
		else:
			convex=False

		dr=self.image_xlen/2
		im= Image.new("RGB", (self.image_xlen, self.image_ylen), "#000000")
		
		for y in range(0,self.image_ylen):
			for x in range(0,self.image_xlen):
				mag=dr*dr-(x-dr)*(x-dr)-(y-dr)*(y-dr)
				if mag<0:
					mag=0.0
				else:
					mag=sqrt(mag)

				mag=mag/dr
				if convex==True:
					mag=int(255*(mag))
				else:
					mag=int(255-255*(mag))

				im.putpixel((x,y),(mag, mag, mag))

		return im


	def add_job_to_server(self,sim_path,server):
		path=os.path.dirname(self.file_name)
		server.add_job(sim_path,"--simmode data@mesh_gen --path "+path)

	def load_image(self):
		file_to_load=os.path.join(os.path.dirname(self.file_name),"image.png")
		if os.path.isfile(file_to_load)==False:
			return None

		img=Image.open(file_to_load)
		if img.mode!="RGB":
			img=img.convert('RGB')

		return img.convert('RGB')

	def draw_gauss(self):

		sigma=self.gauss.gauss_sigma
		gauss_offset_x=self.gauss.gauss_offset_x
		gauss_offset_y=self.gauss.gauss_offset_y

		im= Image.new("RGB", (self.image_xlen, self.image_ylen), "#FF0000")

		for y in range(0,self.image_ylen):
			for x in range(0,self.image_xlen):
				mag=int(255*exp(-((x-gauss_offset_x-self.image_xlen/2)/sigma)**2 -((y-gauss_offset_y-self.image_ylen/2)/sigma)**2))

				im.putpixel((x,y),(mag, mag, mag))

		return im
		
