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

## @package inp_util
#  utility functions for inp these functions should not touch the disk.
#

def inp_file_to_list(lines):
	sub_items=[]
	items=[]
	for l in lines:
		if l.startswith("#") and len(sub_items)!=0:
			items.append(sub_items)
			sub_items=[]

		if l=="#end" or l=="#ver":
			break

		sub_items.append(l)

	return items

def inp_get_all_tokens(lines):
	ret=[]
	for l in lines:
		if l.startswith("#"):
			if l!="#end" and l!="#ver":
				ret.append(l)

	return ret

def inp_search_token_value_multiline(lines, token):
	ret=[]
	for i in range(0, len(lines)):
		if lines[i]==token:
			pos=i+1
			while (lines[pos][0]!="#"):
				ret.append(lines[pos])
				pos=pos+1

			return ret

	return False


def inp_check_ver(file_path, ver):
	"""Check ver of file"""
	lines=inp_load_file(file_path)
	if lines==False:
		return False

	for i in range(0, len(lines)):
		if lines[i]=="#ver":
			if len(lines)>i+2:
				if lines[i+1]==ver:
					if lines[i+2]=="#end":
						return True
			return False

	return False
