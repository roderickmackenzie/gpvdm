#!/usr/bin/env python3
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
				shutil.copyfile(os.path.join(os.getcwd(),"build_system","dependency_scripts","packages_ubuntu.sh"), os.path.join("pub","flat","install_packages.sh"))

				shutil.copyfile(os.path.join(os.getcwd(),"gpvdm_data","docs","readme","readme_flat_install_linux.md"), os.path.join("pub","flat","README.md"))

				os.system("tar -czvf ./pub/gpvdm-8.0-for-ubuntu-20.04.tar.gz -C ./pub/flat/ .")


		else:
			return

