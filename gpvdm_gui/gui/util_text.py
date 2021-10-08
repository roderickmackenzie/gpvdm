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

## @package util_text
#  Helper functinons for text manipulation.
#

def insert_tab(lines,n):
	build=""
	for i in range(0,n):
		build=build+"\t"
	for i in range(0,len(lines)):
		lines[i]=build+lines[i]

def is_number(data_in):
	try:
		float(data_in)
		return True
	except ValueError:
		return False

	return False
	if type(data_in)==str:
		if len(data_in)>0:
			s=data_in
			s=re.sub(' ','',s)
			s=re.sub("\+",'',s)
			s=re.sub('-','',s)
			s=re.sub('\t','',s)
	
			if len(s)>0:
				if s[0].isdigit()==True:
					return True
				else:
					return False

	return False

def gkt_title_to_gnu_plot_title(in_string):
	out_string=in_string.replace("<sub>","_{")
	out_string=out_string.replace("</sub>","}")
	return out_string


