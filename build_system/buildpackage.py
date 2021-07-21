#!/usr/bin/env python3
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
import shutil
try:
	from dialog import Dialog
except:
	from menu import Dialog


from shutil import copyfile
from shutil import rmtree
from util import copytree
from flat_install import flat_install

def build_windows_install(d):
	if d.yesno("Build files for windows installer?") == d.OK:
		dll_opengl_path="/home/rod/windll/opengl_dlls/"
		dll_compiled_path="/home/rod/windll/compiled_dlls2/"
		windows_share_path="/home/rod/windows/share"

		output_path=flat_install(d)	
	
		#copy dlls
		for file in os.listdir(dll_opengl_path):
			copyfile(os.path.join(dll_opengl_path,file), os.path.join(output_path,file))

		for file in os.listdir(dll_compiled_path):
			copyfile(os.path.join(dll_compiled_path,file), os.path.join(output_path,file))


		if os.path.isdir(windows_share_path)==True:
			ret=d.inputbox("Copy files to "+windows_share_path+"?", init=windows_share_path)
			if ret[0]=="ok":
				windows_share_path=ret[1]
				print
				#yesno("Copy files to "+windows_share_path+"?") == d.OK:
				publish_dir=os.path.join(windows_share_path,"pub")
				if os.path.isdir(publish_dir)==True:
					rmtree(publish_dir)

				#os.mkdir(publish_dir)
				shutil.copytree(output_path, publish_dir, symlinks=False)
				d.msgbox("The build has been placed in: "+windows_share_path)

def buildpackage_menu(d):
	if os.geteuid() == 0:
		d.msgbox("Don't run me as root.")
		return
	menu=[]

	menu.append(("(rpm)", "Build rpm"))
	menu.append(("(deb)", "Build deb"))
	menu.append(("(flat)", "Flat direct"))
	menu.append(("(zip)", "Non windows zip file"))
	menu.append(("(win)", "Windows installer"))


	while(1):
		code, tag = d.menu("Build:", choices=menu)
		if code == d.OK:
			if tag=="(rpm)":
				from build_rpm import make_rmp_dir
				make_rmp_dir(d)

			if tag=="(deb)":
				from deb import make_deb
				make_deb(d)

			if tag=="(win)":
				build_windows_install(d)

			if tag=="(flat)":
				flat_install(d)

		else:
			return

