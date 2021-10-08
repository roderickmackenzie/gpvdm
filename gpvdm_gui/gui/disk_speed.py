#!/usr/bin/env python3
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

## @package disk_speed
#  Check the disk speed.
#

import os
import shutil
import time
import random

calculated_disk_speed=0


def disk_test(path):
	global calculated_disk_speed
	nfiles=1000
	written_files=0
	test_dir=os.path.join(path,"io_test")
	start_time=time.time()
	if os.path.isdir(test_dir)==False:
		os.mkdir(test_dir)	
	else:
		return -1

	test=""
	for i in range (0,100):
		test=test+"speed test"

	for i in range(0,nfiles):
		N = 10
		bits = random.getrandbits(N)

		file_name=os.path.join(test_dir,str(i))
		f = open(file_name, "w")
		f.write(str(bits))
		f.close()

		written_files=written_files+1
		#os.stat(file_name).st_size
		if (time.time()-start_time)>0.2:
			break

	shutil.rmtree(test_dir)
	stop_time=time.time()

	delta=int(written_files/(stop_time-start_time))

	calculated_disk_speed=str(delta)
	#print("disk_speed=",calculated_disk_speed)
	return delta

def get_disk_speed():
	global calculated_disk_speed
	return str(calculated_disk_speed)

if __name__ == '__main__':
	print("doing disk test")
	disk_test("./")
	print(get_disk_speed())

