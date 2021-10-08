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

## @package gui_enable
#  Enable or disable the gui, used when QT is not installed and you want to guse gpvdm on the command line. 
#


import sys
import os

enabled=False
def gui_test():
	global enabled
	for arg in sys.argv:
		if arg.startswith("--") and arg!="--load":
			enabled=False
			return

	try:
		from PyQt5.QtWidgets import QWidget
		enabled=True
	except:
		enabled=False
		pass
	
	
def set_gui(value):
	global enabled
	enabled=value
	
def gui_get():
	global enabled
	return enabled

class QWidget():
	def __init__(self):
		return
