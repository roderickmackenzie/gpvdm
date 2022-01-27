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

## @package colors
#  Functions to deal with colors.
#

import os
import io
from numpy import *

color=[]
color_black=[]
marker=[]

def gen_colors_black():
	global color_black
	global marker
	marker_base=["","x","o"]

	base=[[0.0,0.0,0.0]]
	marker=[]
	color_black=[]
	for i in range(0,100):
		color_black.append([base[0][0],base[0][1],base[0][2]])
		marker.append("")

def gen_colors():
	global color
	global marker
	base=[[0.0,0.0,1.0],[1.0,0.0,0.0],[0.0,1.0,0.0],[0.0,1.0,1.0],[1.0,1.0,0.0],[1.0,0.0,1.0]]
	marker=[]
	marker_base=["","x","o"]
	mul=1.0
	color=[]
	for rounds in range(0,20):
		for i in range(0,len(base)):
			color.append([base[i][0]*mul,base[i][1]*mul,base[i][2]*mul])
			marker.append("")
		mul=mul*0.5

gen_colors()
gen_colors_black()

def get_color(i):
	global color
	return color[i]

def get_color_black(i):
	global color_black
	return color_black[i]

def get_marker(i):
	global marker
	return marker[i]

class color_map():

	def __init__(self,map_name="inferno"):
		#[(0.0,0.0,0.0), (255.0,255.0,0.0), (0.0,255.0,0.0), (0.0,255.0,255.0), (0.0,0.0,255.0)]
		#points = [(0.0,0.0,0.0),  (0.0,0.0,255.0),  (255.0,0.0,0.0)]
		#inferno
		points=[]
		if map_name=="inferno":
			points.append("000004") # black
			points.append("1f0c48") # dark purple
			points.append("550f6d") # dark purple
			points.append("88226a") # purple
			points.append("a83655") # red-magenta
			points.append("e35933") # red
			points.append("f9950a") # orange
			points.append("f8c932") # yellow-orange
			points.append("fcffa4") # light yellow

		if map_name=="blues":
			points.append("440154")
			points.append("472c7a")
			points.append("3b518b")
			points.append("2c718e")
			points.append("21908d")
			points.append("27ad81")
			points.append("5cc863")
			points.append("aadc32")
			points.append("fde725")

		self.map = []
		delta=255/(len(points)-1)
		count=0
		pos=0
		for i in range(0, 256):
			if count>delta:
				pos=pos+1
				count=0

			#print(i,pos)
			ratio=count/delta

			c0 = [int(points[pos][0:2], base=16),int(points[pos][2:4], base=16),int(points[pos][4:6], base=16)]
			c1 = [int(points[pos+1][0:2], base=16),int(points[pos+1][2:4], base=16),int(points[pos+1][4:6], base=16)]
			#c0=points[pos]
			#c1=points[pos+1]

			r=c0[0]+(c1[0]-c0[0])*ratio
			g=c0[1]+(c1[1]-c0[1])*ratio
			b=c0[2]+(c1[2]-c0[2])*ratio

			count=count+1

			self.map.append([int(r), int(g), int(b)])


