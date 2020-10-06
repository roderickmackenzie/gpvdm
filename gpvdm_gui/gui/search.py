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


## @package search
#  General search functions.
#

import os, fnmatch
import glob

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

def find_fit_speed_log(out_file,path):
	pattern='fitlog_time_speed.dat'
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
		 string=string+"'"+my_file+"' using ($1/60/60):($2) with lp,"

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



def find_fits():
	pattern='plot_file.dat'
	path='./'
	result = []
	for root, dirs, files in os.walk(path):
		for name in files:
			if fnmatch.fnmatch(name, pattern):
				result.append(os.path.join(root, name))
	files=result
	string="plot \\"

	for my_file in files:
		f = open(my_file)
		lines = f.readlines()
		f.close()

		for i in range(0, len(lines)):
			string=string+'\n'+lines[i].rstrip()
	string = string[:-2]

	text_file = open("./plot/fit.plot", "w")
	text_file.write(string)
	text_file.close()

def find_fit_error():
	#path='./'
	#result = []
	string="plot \\\n"
	i=0
	for name in glob.glob('error_*exp*.dat'):
		i=i+1
		string=string+"'"+name+"' with lp lt "+str(i)+",\\\n"

	i=0
	for name in glob.glob('error_*sim*.dat'):
		i=i+1
		string=string+"'"+name+"' with lp lt "+str(i)+",\\\n"

	string = string[:-3]

	text_file = open("./plot/fit_errors.plot", "w")
	text_file.write(string)
	text_file.close()



