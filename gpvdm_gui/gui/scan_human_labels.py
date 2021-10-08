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

## @package scan_human_labels
#  A class to turn the .inp into human labels
#
#import sys
import os
from token_lib import tokens
from epitaxy import get_epi

import re
import json

#import shutil
class scan_item:
	human_label=""
	token=""
	filename=""

def json_find_sim_obj(data,sim_name):
	seg,sim_mode=sim_name.split("@")
	simulation_obj=getattr(data, sim_mode)
	seg_number=int(seg[len("segment"):])
	return simulation_obj.segments[seg_number]

def get_json_from_human_path(json,path):
	pointer=json
	s=path.replace('\\','/')
	s=s.split("/")
	for token in s:
		found=False
		if token in pointer:
			true_token=token
			found=True

		if found==False:
			for json_obj in pointer:
				sub_obj=pointer[json_obj]
				shape_name=False
				try:
					shape_name=sub_obj['shape_name']
				except:
					pass

				if shape_name!=False:
					if shape_name==token:
						true_token=json_obj
						found=True
						break

		if found==False:
			lib_token=tokens().reverse_lookup(token)
			if lib_token!=False:
				token=lib_token.token
				if token in pointer:
					true_token=token
					found=True

				
		if found==False:
			print("lost at ",token)
			return False
		else:
			#print(true_token)
			pointer=pointer[true_token]

	if found==True:
		return pointer

def json_get_val(json_data,path):
	try:
		pointer=json_data
		for seg in path.split("."):
			pointer=pointer[seg]
		return pointer
	except:
		return None

def set_json_from_human_path(json,path,val):
	pointer=json
	s=path.replace('\\','/')
	s=s.split("/")
	for token in s:
		found=False
		if token in pointer:
			true_token=token
			found=True

		if found==False:
			for json_obj in pointer:
				sub_obj=pointer[json_obj]
				shape_name=False
				try:
					shape_name=sub_obj['shape_name']
				except:
					pass

				if shape_name!=False:
					if shape_name==token:
						true_token=json_obj
						found=True
						break

		if found==False:
			lib_token=tokens().reverse_lookup(token)
			if lib_token!=False:
				token=lib_token.token
				if token in pointer:
					true_token=token
					found=True

				
		if found==False:
			print("lost at ",token)
			return False
		else:
			#print(true_token)
			last_pointer=pointer
			pointer=pointer[true_token]

	if found==True:
		last_pointer[true_token]=val

def get_json_path_from_human_path(json_in,path):
	if type(json_in)!=dict:
		json_in=json.loads("\n".join(json_in.gen_json()))

	pointer=json_in
	s=path.replace('\\','/')
	s=s.split("/")
	out_path=[]
	for token in s:
		found=False
		if token in pointer:
			true_token=token
			found=True

		if found==False:
			for json_obj in pointer:
				sub_obj=pointer[json_obj]
				shape_name=False
				try:
					shape_name=sub_obj['shape_name']
				except:
					pass

				if shape_name!=False:
					if shape_name==token:
						true_token=json_obj
						found=True
						break

		if found==False:
			lib_token=tokens().reverse_lookup(token)
			if lib_token!=False:
				token=lib_token.token
				if token in pointer:
					true_token=token
					found=True

				
		if found==False:
			print("lost at ",token)
			return False
		else:
			#print(true_token)
			out_path.append(true_token)
			pointer=pointer[true_token]

	if found==True:
		return "/".join(out_path)




