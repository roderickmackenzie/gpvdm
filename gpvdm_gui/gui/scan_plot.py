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

## @package scan_plot
#  Plotting functions for scanning material parameters.
#

import sys
import os
from search import return_file_list
from plot import check_info_file
from plot import plot_populate_plot_token
from error_dlg import error_dlg
from multiplot_from_tokens import multiplot_from_tokens

def scan_gen_plot_data(plot_token,base_path):
	ret=""
	plot_files=[]
	plot_labels=[]
	save_file=""

	file_name=plot_token.file0
	if file_name=="":
		print("You have given me no file name!!!")
		sys.exit(0)

	#search for the files
	return_file_list(plot_files,base_path,file_name)
	print("search_file=",plot_token.file0,file_name)
	print("plot_files0=",plot_files)
	#print("found_files=",plot_files)
	#print("found_files=",base_path)
	
	#now tidy up the list as we need to remove snapshots
	save_list=[]
	if base_path.split(os.sep).count("snapshots")==0:
		for i in range(len(plot_files)):
			if "snapshots" not in plot_files[i]:
				save_list.append(plot_files[i])
		plot_files=save_list

	print("plot_files=",plot_files)
	
	num_list=[]

	#remove the file name in the base_dir
	test_file=os.path.join(base_path,file_name)
	if test_file in plot_files:
	    plot_files.remove(test_file)

	#attempt to sort list in numeric order
	try:
		for i in range(0, len(plot_files)):
			dir_name=os.path.basename(os.path.dirname(plot_files[i]))
			if dir_name=="dynamic":
				dir_name=os.path.basename(os.path.dirname(os.path.dirname(plot_files[i])))
			num_list.append(float(dir_name))

		num_list, plot_files = zip(*sorted(zip(num_list, plot_files)))
	except:
		print("There are stings in the list I can not order it")

	#if it is an info file then deal with it
	print(check_info_file(file_name),file_name,plot_token.file0,plot_token.file1,plot_token.tag0,plot_token.tag1)

	if (check_info_file(file_name)==True):
		#print "Rod",plot_files,self.sim_dir

		print(plot_files,"r",plot_labels,"r",save_file,"r",plot_files,"r",base_path,"r",plot_token)

		multi_plot=multiplot_from_tokens()
		plot_files, plot_labels, save_file = multi_plot.gen_plot(base_path,plot_token.file0,plot_token.tag0,plot_token.file1,plot_token.tag1)

	else:
		if len(plot_files)>0:
			ret=plot_populate_plot_token(plot_token,plot_files[0])
			if ret==False:
				error_dlg(self,"This file "+file_name+" is not in the data base please file a bug report..")
				return

			#build plot labels
			for i in range(0,len(plot_files)):
				text=plot_files[i][len(base_path):len(plot_files[i])-1-len(os.path.basename(plot_files[i]))]
				if text.endswith("dynamic"):
					text=text[:-7]

				if text.endswith("optical_output"):
					text=text[:-10]

				if text[0]=="/" or "\\":
					text=text[1:]
					
				text=text.replace("\\","/")

				plot_labels.append(str(text))

			save_file=os.path.join(base_path,os.path.splitext(os.path.basename(plot_files[0]))[0])+".oplot"


	return plot_files, plot_labels, save_file
