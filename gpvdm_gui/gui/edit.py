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

## @package edit
#  I have no idea what this is for - delete it?
#

import subprocess
import shutil
import sys

if len(sys.argv)==1:
	print("argument needed")
	sys.exit(0)

file_name=sys.argv[1]
lines=[]
output = subprocess.Popen(["pyflakes", file_name], stdout=subprocess.PIPE).communicate()[0]
a=output.split("\n")
for i in range(0,len(a)):
	s=a[i].split(":")
	if len(s)==3:
		lines.append(int(s[1]))

print(lines)

f = open(file_name, "r")
copy = open("out.dat", "w")
pos=1
for line in f:
	if lines.count(pos)==0:
		copy.write(line)
	else:
		copy.write("#"+line)
	pos=pos+1

f.close()
copy.close()

shutil.move("out.dat", file_name)
