#!/usr/bin/env python3
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


## @package gpvdm_zip
#  A command line tool to add files to a zip file - not sure what this is used for.
#


import sys
import zipfile
from util_zip import zip_remove_file
archive=sys.argv[1]
infile=sys.argv[2]
new_name=sys.argv[3]

zip_remove_file(archive,new_name)

zf = zipfile.ZipFile(archive, mode='a')
try:
    zf.write(infile, arcname=new_name)
finally:
    zf.close()
