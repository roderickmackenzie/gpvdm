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

## @package window_list
#  Store/load the position of windows. 
#


import os
from inp import inp_load_file
from inp import inp_save

from cal_path import get_sim_path
from inp import inp_read_next_item
from PyQt5.QtWidgets import QWidget, QDesktopWidget
from cal_path import get_user_settings_dir

def resize_window_to_be_sane(window,x,y):
	shape=QDesktopWidget().screenGeometry()
	w=shape.width()*x
	h=shape.height()*y
	window.resize(w,h)

class window_item:
	name=""
	x=0.0
	y=0.0

wlist=[]

def wpos_set_window(window,name):
	global wlist
	for i in range(0,len(wlist)):
		if wlist[i].name==name:
			shape=QDesktopWidget()#.screenGeometry()

			desktop_w=shape.width()
			desktop_h=shape.height()

			w=window.width()
			h=window.height()

			x=int(wlist[i].x)
			y=int(wlist[i].y)
			if (x+w>desktop_w) or x<0:
				x=desktop_w/2-w/2
				print("Reset with",x,desktop_w)
			if (y+h>desktop_h) or y<0:
				y=desktop_h/2-h/2
				print("Reset height",y)
			window.move(x,y)
			print("moving to",x,y)
			break

def wpos_dump():
	for i in range(0,len(wlist)):
		print(wlist[i].name,wlist[i].x,wlist[i].y)

def wpos_update(window,name):
	global wlist
	found=False
	x=window.x()
	y=window.y()
	#print(x,y)
	for i in range(0,len(wlist)):
		if wlist[i].name==name:
			found=True
			wlist[i].x=x
			wlist[i].y=y
			break

	if found==False:
		a=window_item()
		a.name=name
		a.x=x
		a.y=y
		wlist.append(a)
			
def wpos_load():
	#print("load")
	global wlist
	wlist=[]
	lines=[]
	pos=0
	lines=inp_load_file(os.path.join(get_user_settings_dir(),"window_list.inp"))

	if lines==False:
		return

	if len(lines)<2:
		return

	while(1):
		token,name,pos=inp_read_next_item(lines,pos)
		if token=="#end" or token=="#ver":
			break

		token,x,pos=inp_read_next_item(lines,pos)

		token,y,pos=inp_read_next_item(lines,pos)
	
		a=window_item()
		a.name=name
		a.x=float(x)
		a.y=float(y)
		wlist.append(a)


def wpos_save():
	global wlist
	lines=[]

	for i in range(0,len(wlist)):
		lines.append("#window_name_"+wlist[i].name)
		lines.append(wlist[i].name)
		lines.append("#window_x_"+wlist[i].name)
		lines.append(str(wlist[i].x))
		lines.append("#window_y_"+wlist[i].name)
		lines.append(str(wlist[i].y))
	
	lines.append("#ver")
	lines.append("1.0")
	lines.append("#end")

	ret=inp_save(os.path.join(get_user_settings_dir(),"window_list.inp"),lines)
	return ret
