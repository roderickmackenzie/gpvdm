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

def triangles_get_min(data):
	if len(data)==0:
		return None

	x=data[0].min_x()
	y=data[0].min_y()
	z=data[0].min_z()

	for t in data:
		if t.min_x()<x:
			x=t.min_x()

		if t.min_y()<y:
			y=t.min_y()

		if t.min_z()<z:
			z=t.min_z()

	ret=vec()
	ret.x=x
	ret.y=y
	ret.z=z

	return ret

def triangles_get_max(data):
	if len(data)==0:
		return None

	x=data[0].max_x()
	y=data[0].max_y()
	z=data[0].max_z()

	for t in data:
		if t.max_x()>x:
			x=t.max_x()

		if t.max_y()>y:
			y=t.max_y()

		if t.max_z()>z:
			z=t.max_z()

	ret=vec()
	ret.x=x
	ret.y=y
	ret.z=z

	return ret

def triangles_sub_vec(data,vec):
	ret=[]
	if len(data)==0:
		return None

	for i in range(0,len(data)):
		ret.append(data[i]-vec)
	return ret

def triangles_add_vec(data,vec):
	ret=[]
	if len(data)==0:
		return None

	for i in range(0,len(data)):
		ret.append(data[i]+vec)
	return ret

def triangles_remove_below(data,val):
	ret=[]
	if len(data)==0:
		return None

	for i in range(0,len(data)):
		if data[i].max_y()>val:		
			ret.append(data[i])
	return ret

def triangles_rotate_y(data,ang):
	ret=[]
	if len(data)==0:
		return None

	for i in range(0,len(data)):
		ret.append(data[i].rotate_y(ang))

	return ret

def triangles_div_vec(data,vec):
	if len(data)==0:
		return None

	ret=[]
	for i in range(0,len(data)):
		ret.append(data[i]/vec)

	return ret

def triangles_mul_vec(data,vec):
	if len(data)==0:
		return None
	ret=[]

	for i in range(0,len(data)):
		ret.append(data[i]*vec)

	return ret


def triangles_flip(data):
	v=vec()
	v.x=1.0
	v.y=-1.0
	v.z=1.0

	ret=triangles_mul_vec(data,v)

	return ret

def triangles_flip_in_box(data):
	v=vec()
	v.x=1.0
	v.y=-1.0
	v.z=1.0

	ret=triangles_mul_vec(data,v)

	m=triangles_get_min(ret)
	m.x=0
	m.z=0

	ret=triangles_sub_vec(ret,m)
	return ret

def triangles_print(data):
	if len(data)==0:
		return None

	for i in range(0,len(data)):
		print(data[i])

def triangles_scale_for_gl(data):
	if len(data)==0:
		return False

	min=triangles_get_min(data)
	ret=triangles_sub_vec(data,min)
	max=triangles_get_max(ret)
	max.x=max.x/10.0
	max.y=max.y/2.0
	max.z=max.z/10.0

	ret=triangles_div_vec(ret,max)
	#ret=triangles_flip(ret)
	#min=triangles_get_min(ret)
	#ret=triangles_sub_vec(ret,min)
	v=vec()
	v.x=5.0
	v.y=0.0
	v.z=5.0
	ret=triangles_sub_vec(ret,v)
	return ret

def triangles_norm_y(data,min_value,mav_value):
	v=vec()
	v.y=triangles_get_min(data)
	v.x=0.0
	v.z=0.0

	triangles_sub_vec(data,v)

	v.y=triangles_get_min(data)
	v.x=0.0
	v.z=0.0
	triangles_add_vec(data,min_value)

	v.y=triangles_get_max(data)
	v.x=0.0
	v.z=0.0

	triangles_div_vec(data,v)

	v.y=mav_value
	v.x=0.0
	v.z=0.0

	triangles_mul_vec(data,v)

	return data
