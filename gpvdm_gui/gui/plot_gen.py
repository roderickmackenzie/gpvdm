# -*- coding: utf-8 -*-
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


import os
from plot_window import plot_window

window=None

destroy=False

def set_plot_auto_close(value):
	global destroy
	destroy=value

def plot_gen(input_files,plot_labels,config_file):

	if (len(input_files)==1):
		if os.path.splitext(input_files[0])[1]==".plot":
			plot_file=input_files[0]
			cmd = 'gnuplot -persist '+plot_file
			os.system(cmd)
			return

	global window
	global destroy
	if window!=None:
		if window.shown==True:
			if destroy==True:
				window.input_files=input_files
				window.plot.load_data(input_files)
				window.plot.set_labels(plot_labels)
				window.plot.do_plot()
				window.plot.fig.canvas.draw()
				window.window.present()
				window.window.set_keep_above(True)
				return

	window=plot_window()
	window.init(input_files,plot_labels)


