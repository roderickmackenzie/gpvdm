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

## @package colors
#  Functions to deal with colors.
#

import os
import io
from numpy import *
from plot_io import plot_load_info

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
