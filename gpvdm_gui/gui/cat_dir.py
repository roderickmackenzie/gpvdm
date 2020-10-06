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

## @package cat_dir
#  Catalog a gpvdm directory
#


import os
from inp import inp
from inp import inp_lsdir

from cal_path import get_sim_path
from html_gen import html_gen
from token_lib import tokens

def cat_dir():
	my_token_lib=tokens()
	banned=["dump_file.inp"]
	files=inp_lsdir(os.path.join(get_sim_path(),"sim.gpvdm"))
	h=html_gen()
	h.add_h1(_("Files within sim.gpvdm"))
	for file_name in files:
		if file_name.endswith(".inp")==True and file_name not in banned:
			h.add_collapsible(file_name)
			h.add_div("content")
			f=inp()
			f.load(os.path.join(get_sim_path(),file_name))
			tab=[]
			while(1):
				token,vals=f.get_next_token_array()
				if token==False:
					break

				result=my_token_lib.find(token)
				if result!=False:
					text_info=result.info
					unit=result.units
				else:
					text_info=""
					unit=""

				if len(vals)>0:
					val=vals[0]
				else:
					val=""

				tab.append([token,text_info,val,unit])
			h.add_tab(tab)
			h.end_div()


	h.gen_list()
	h.save(os.path.join(get_sim_path(),"index.html"))
