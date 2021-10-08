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

## @package multiplot_from_tokens
#  This will generate plot files from tokens
#


import sys
import os
import shutil

import i18n
_ = i18n.language.gettext


import zipfile
from util_zip import archive_add_dir
from cal_path import subtract_paths
from inp import inp
from cal_path import get_sim_path
from search import search_simulations
from dat_file import dat_file
from token_lib import tokens

class multiplot_from_tokens:

	def __init__(self):
		self.sims=[]

	def gen_plot(self,base_path,file0,token0,file1,token1,output_file=""):
		x=[]
		y=[]
		plot_labels=[]
		my_token_lib=tokens()

		print(base_path,output_file)
		if output_file=="":
			out_file_name=os.path.join(base_path,os.path.splitext(file0)[0]+token0+"#"+os.path.splitext(file1)[0]+token1+".dat")
		else:
			out_file_name=output_file

		sims=search_simulations(base_path)
		out=dat_file()

		t0=my_token_lib.find(token0)
		t1=my_token_lib.find(token1)

		out.title=t0.info+" v.s. "+t1.info
		out.y_label=t0.info
		out.data_label=t1.info
		out.y_units=t0.units
		out.data_units=t1.units

		for s in sims:
			f=inp()
			f.load(os.path.join(s,file0))
			x.append(float(f.get_token(token0)))

			f=inp()
			f.load(os.path.join(s,file1))
			y.append(float(f.get_token(token1)))

		x, y = zip(*sorted(zip(x, y)))


		out.x_len=1
		out.y_len=len(y)
		out.z_len=1

		out.init_mem()

		for i in range(0,len(x)):
			out.y_scale[i]=x[i]
			out.data[0][0][i]=y[i]

		out.save(out_file_name)

		return [out_file_name], plot_labels, ""
