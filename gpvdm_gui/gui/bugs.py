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

## @package bugs
#  About Keep track of bugs
#

import os

bug_list=[]
class bug():
	def __init__(self):
		id=""

def bugs_add_action(id):
	global bug_list
	a=bug()
	a.id=id
	bug_list.append(a)
	

def bugs_clear():
	global bug_list
	bug_list=[]

def bugs_print():
	global bug_list
	for i in range(0,len(bug_list)):
		print(bug_list[i].id)

def bugs_to_url():
	out=""
	global bug_list
	for i in range(0,len(bug_list)):
		out=out+bug_list[i].id+","
	out=out[:-1]

	return out
