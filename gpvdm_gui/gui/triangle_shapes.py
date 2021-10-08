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

## @package triangle
#  A base triangle class
#

import os
from triangle import vec
from triangle import triangle
from math import pi,acos,sin,cos


	#a=dat_file()
	#from triangle_shapes import dome
	#a.data=dome()
	#v=vec()
	#v.x=1.0
	#v.y=-1.0
	#v.z=1.0

	#a.data=triangles_mul_vec(a.data,v)

	#a.type="poly"
	#a.save("a.inp")

def dome():

	dx=1.0
	dy=1.0
	dz=1.0

	ret=[]
	dx0=dx/2
	dy0=-dy/2
	dz0=dz/2
	
	x=dx0
	y=0.0
	z=dz0

	#top
	theta=0.0
	theta_max=2.0*3.1415
	dtheta=theta_max/15

	phi=0.0
	phi_max=3.1415/2.0
	dphi=phi_max/10.0
	print("bing")
	while(phi<phi_max):
		theta=0
		while (theta<theta_max):
			t0=triangle()
			t1=triangle()

			dx=dx0*cos(theta)*cos(phi)
			dz=dz0*sin(theta)*cos(phi)
			dy=dy0*sin(phi)

			#print(dx,dy,dz)
			t0.xyz0.x=x+dx
			t0.xyz0.y=y+dy
			t0.xyz0.z=z+dz

			dx=dx0*cos(theta)*cos(phi+dphi)
			dz=dz0*sin(theta)*cos(phi+dphi)
			dy=dy0*sin(phi+dphi)

			#print(dx,dy,dz)
			t0.xyz1.x=x+dx
			t0.xyz1.y=y+dy
			t0.xyz1.z=z+dz

			t1.xyz0.x=x+dx
			t1.xyz0.y=y+dy
			t1.xyz0.z=z+dz

			dx=dx0*cos(theta+dtheta)*cos(phi)
			dz=dz0*sin(theta+dtheta)*cos(phi)
			dy=dy0*sin(phi)


			#print(dx,dy,dz)
			t0.xyz2.x=x+dx
			t0.xyz2.y=y+dy
			t0.xyz2.z=z+dz

			t1.xyz1.x=x+dx
			t1.xyz1.y=y+dy
			t1.xyz1.z=z+dz

			dx=dx0*cos(theta+dtheta)*cos(phi+dphi)
			dz=dz0*sin(theta+dtheta)*cos(phi+dphi)
			dy=dy0*sin(phi+dphi)

			t1.xyz2.x=x+dx
			t1.xyz2.y=y+dy
			t1.xyz2.z=z+dz

			ret.append(t0)
			ret.append(t1)

			#do the bottom
			#print(theta)
			theta=theta+dtheta
		
		phi=phi+dphi

	return ret

def btm():
	dx=1.0
	dy=1.0
	dz=1.0

	ret=[]
	dx0=dx/2
	dy0=dy/2
	dz0=dz/2
	
	x=dx0
	y=0.0
	z=dz0

	#top

	#btm
	theta=0.0
	theta_max=2.0*3.1415
	dtheta=theta_max/15

	phi=0.0
	phi_max=3.1415/2.0
	dphi=phi_max/10.0
	theta=0
	while (theta<theta_max):
		t2=triangle()

		dx=dx0*cos(theta)*cos(phi)
		dz=dz0*sin(theta)*cos(phi)
		dy=dy0*sin(phi)

		#print(dx,dy,dz)
		t2.xyz0.x=x+dx
		t2.xyz0.y=y
		t2.xyz0.z=z+dz

		dx=dx0*cos(theta+dtheta)*cos(phi)
		dz=dz0*sin(theta+dtheta)*cos(phi)
		dy=dy0*sin(phi)

		#print(dx,dy,dz)
		t2.xyz1.x=x+dx
		t2.xyz1.y=y
		t2.xyz1.z=z+dz

		t2.xyz2.x=x
		t2.xyz2.y=y
		t2.xyz2.z=z

		ret.append(t2)
		#print(phi)

		theta=theta+dtheta
	return ret
