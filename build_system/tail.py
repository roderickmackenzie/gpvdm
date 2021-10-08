# 
# General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
# base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
# The model can simulate OLEDs, Perovskite cells, and OFETs.
# 
# Copyright 2008-2022 Roderick C. I. MacKenzie https://www.gpvdm.com
# r.c.i.mackenzie at googlemail.com
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
# SOFTWARE.
# 
import os
import sys
import re
import time

def tail(fname):
	while(os.path.isfile(fname)==False):
		print("wait for "+fname)

	f=open(fname,"r", encoding="latin-1")
	sys.stdout.write(f.read())
	sys.stdout.flush()
	p = f.tell()
	f.close()

	mod_time=os.stat(fname).st_mtime
	last_mod_time=mod_time
	while(1):
		mod_time=os.stat(fname).st_mtime
		if last_mod_time!=mod_time:
			f=open(fname,"r", encoding="latin-1")
			f.seek(0,2)
			size = f.tell()
			if p>size:
				p=0
			f.seek(p)
			r=f.read()
			pos=r.rfind('\n')
			if pos!=-1:
				r=r[:pos]
				f.seek(p+len(r))
				r=re.sub(r'[^\x00-\x7F]+',' ', r)
				print(r, end='', flush=True)
				if r.count("gpvdm_build_finished")>0:
					break
				p = f.tell()
			f.close()

		time.sleep(0.05) 
		if os.path.isfile(fname)==True:
			if (time.time() - mod_time)>10.0:
				print("tail finished..")
				break
		else:
			break

		last_mod_time=mod_time

#	input("Press Enter to continue...")
	print("")

