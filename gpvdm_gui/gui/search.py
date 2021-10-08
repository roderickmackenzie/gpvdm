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


## @package search
#  General search functions.
#

import os, fnmatch
import glob

def find_sims(search_path):
	sims=[]
	for root, dirs, files in os.walk(search_path):
		for name in files:
			full_name=os.path.join(root, name)
			if full_name.endswith("sim.gpvdm") and os.path.dirname(os.path.dirname(full_name)).endswith("sim")==0:		
				sims.append(os.path.dirname(full_name))
	return sims

def search_simulations(path):
	ret=[]
	for root, dirs, files in os.walk(path):
		for name in files:
			if name=="sim.gpvdm":
				ret.append(os.path.dirname(os.path.join(root, name)))
	return ret

def find_fit_log(out_file,path):
	pattern='fitlog_time_error.dat'
	fitlog = []
	for root, dirs, files in os.walk(path):
		for name in files:
			if fnmatch.fnmatch(name, pattern):
				fitlog.append(os.path.join(root, name))

	pattern='fitlog_time_speed.dat'
	fitlog_time_speed = []
	for root, dirs, files in os.walk(path):
		for name in files:
			if fnmatch.fnmatch(name, pattern):
				fitlog_time_speed.append(os.path.join(root, name))

	string="plot "
	for my_file in fitlog:
		 string=string+"'"+my_file+"' using ($1):($2) with lp,"

	#for my_file in fitlog_time_speed:
		# string=string+"'"+my_file+"' using ($2) axis x1y2 with lp,"

	string = string[:-1]
	text_file = open(out_file, "w")
	text_file.write(string)
	text_file.close()


def return_file_list(result,start_dir,file_name):
	#print(start_dir, file_name)
	pattern=file_name
	path=start_dir
	for root, dirs, files in os.walk(path, followlinks=True):
		for name in files:
			if fnmatch.fnmatch(name, pattern):
				result.append(os.path.join(root, name))

	for i in range(0, len(result)):
		result[i]=result[i].rstrip()




def find_shapshots(path):
	out=[]
	for root, dirs, files in os.walk(path):
		for name in files:
			if name.endswith("json.dat")==True:
				sub_snapshot_dir=os.path.dirname(os.path.join(root, name))
				snapshot_dir=os.path.dirname(sub_snapshot_dir)
				if os.path.basename(snapshot_dir)=="snapshots":
					if snapshot_dir not in out:
						out.append(snapshot_dir)

	print(out)
	return out
