# 
# General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
# base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
# 
# Copyright (c) 2012-2019, Roderick C. I. MacKenzie
# All rights reserved.
# 
# 	https://www.gpvdm.com
# 	Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the GPVDM nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

