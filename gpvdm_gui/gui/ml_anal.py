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

## @package scan_ml
#  ML framework.
#

import sys
import os
import shutil

from math import log10
from math import isnan

import i18n
_ = i18n.language.gettext

from progress_class import progress_class
from process_events import process_events
from server import server_break
from util_zip import zip_lsdir
from util_zip import extract_dir_from_archive

from inp import inp_get_token_value
import zipfile
import random
import string
import numpy as np
from scans_io import scans_io
from gui_util import yes_no_dlg
from scan_io import scan_io
from yes_no_cancel_dlg import yes_no_cancel_dlg
from gpvdm_json import gpvdm_data
from scan_human_labels import get_json_from_human_path
from inp import inp
from scan_human_labels import get_json_path_from_human_path
from scan_human_labels import json_get_val

from json_base import json_base
from json_diff import json_diff
import json
from scan_human_labels import json_find_sim_obj
from dat_file import dat_file
import math
from token_lib import tokens
import re

class ml_anal:

	def __init__(self):
		pass

	def gather(self,json_in,x_token,y_token,array_item=None):
		ret=[]
		for obj in json_in:
			if obj != "item_type":
				obj_x=None
				obj_y=None
				if x_token!=None:
					obj_x=json_in[obj]
					tokens=x_token.split("/")
					
					for t in tokens:
						obj_x=obj_x[t]

				if y_token!=None:
					obj_y=json_in[obj]
					tokens=y_token.split("/")
					
					for t in tokens:
						obj_y=obj_y[t]

					if array_item!=None:
						s=obj_y.split()
						obj_y=s[array_item]

				ret.append([obj_x,obj_y])
		#print(ret)
		return ret

	def plot_xy(self,file_name,x_token,y_token,array_item=None):
		t=tokens()
		my_short_token=x_token
		if my_short_token.count("/")+my_short_token.count(".")>0:
			my_short_token=re.split('\/|\.', my_short_token)[-1]
		xt=t.find(my_short_token)

		my_short_token=y_token
		if my_short_token.count("/")+my_short_token.count(".")>0:
			my_short_token=re.split('\/|\.', my_short_token)[-1]
		yt=t.find(my_short_token)

		f=inp()
		json_data=f.load_json(file_name)
		vals=self.gather(json_data,x_token,y_token,array_item=array_item)

		out_file=dat_file()
		out_file.y_mul=1.0
		out_file.y_units=xt.units
		out_file.y_label=xt.info
		out_file.data_mul=1.0
		if yt!=False:
			out_file.data_units=yt.units
			out_file.data_label=yt.info
		out_file.y_scale=[]
		out_file.data=[[[]]]

		for v in vals:
			try:
				if v[0].count("nan")==0 and v[1].count("nan")==0:
					f0=float(v[0])
					f1=float(v[1])
					out_file.y_scale.append(f0)
					out_file.data[0][0].append(f1)
			except:
				pass

		out_file.y_len=len(out_file.data[0][0])

		return out_file

	def get_bin(self,x,val):
		for i in range(0,len(x)):
			if x[i]>=val:
				return i
		return len(x)-1

	def plot_hist(self,file_name,my_token,log_scale=False):
		t=tokens()
		my_short_token=my_token
		if my_short_token.count("/")+my_short_token.count(".")>0:
			my_short_token=re.split("\/|\.", my_token)[-1]
		xt=t.find(my_short_token)

		f=inp()
		json_data=f.load_json(file_name)
		vals=self.gather(json_data,my_token,None)
		out_file=dat_file()
		out_file.y_mul=1.0
		out_file.y_units=xt.units
		out_file.y_label=xt.info
		out_file.data_mul=1.0
		out_file.data_units=_("au")
		out_file.data_label=_("Counts")
		out_file.y_scale=[]
		out_file.data=[[[]]]
		data=[]
		for v in vals:
			f=float(v[0])
			if math.isinf(f)==False:
				if math.isnan(f)==False:
					data.append(f)

		bins=10
		if log_scale==False:
			my_min=min(data)
			my_max=max(data)
			db=(my_max-my_min)/bins
			xpos=my_min

			xpos=xpos-db/2.0
			my_max=my_max+db/2.0

			x=[]
			bin=[]
			while xpos<my_max:
				x.append(xpos)
				xpos=xpos+db
				bin.append(0)
		else:
			out_file.logy=True
			my_min=data[0]
			for d in data:
				if abs(d)<my_min:
					my_min=abs(d)

			#print(my_min)
			if my_min!=0:
				my_min=math.log10(my_min)
			my_max=math.log10(max(data))
			db=(my_max-my_min)/bins
			xpos=my_min

			my_max=my_max+db/2.0

			x=[]
			bin=[]
			while xpos<my_max:
				x.append(math.pow(10,xpos))
				xpos=xpos+db
				bin.append(0)

		for i in range(0,len(data)):
			b=self.get_bin(x,abs(data[i]))
			bin[b]=bin[b]+1

		for i in range(0,len(bin)):
			out_file.y_scale.append(x[i])
			out_file.data[0][0].append(bin[i])
		out_file.y_len=len(out_file.data[0][0])

		return out_file
