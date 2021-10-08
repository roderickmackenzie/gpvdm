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
try:
	from dialog import Dialog
except:
	from menu import Dialog



def git_pull():
	if os.path.isdir("gpvdm_core")==False or os.path.isdir("gpvdm_gui")==False or os.path.isdir("gpvdm_data")==False:
		#print()
		a=input("Should I download the source code to gpvdm? [y/n]")
		if a == "y":
			os.system("git clone https://github.com/roderickmackenzie/gpvdm_core")
			os.system("git clone https://github.com/roderickmackenzie/gpvdm_gui")
			os.system("git clone https://github.com/roderickmackenzie/gpvdm_data")
		else:
			print("For me to compile the source code you need to download it first.")
			exit()





