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

## @package check_lib_in_bash_rc
#  Check the bash rc for the lib path
#

import sys
import os
#import shutil
from pathlib import Path
from win_lin import running_on_linux
from inp import inp
from cal_path import get_home_path
from cal_path import get_exe_path

def check_lib_in_bash_rc():
	if running_on_linux()==True:
		f=inp()
		gpvdm_installed=-1
		if f.load(os.path.join(get_home_path(),".bashrc"))!=False:
			for i in range(0,len(f.lines)):
				if f.lines[i].startswith("export LD_LIBRARY_PATH")==True and f.lines[i].count("gpvdm")!=0:
					gpvdm_installed=i
					if f.lines[i].endswith(get_exe_path()):
						return
		line="export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"+get_exe_path()

		if gpvdm_installed==-1:
			f.lines.append(line)
		else:
			f.lines[i]=line
		f.save()

