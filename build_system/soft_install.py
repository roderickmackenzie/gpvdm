#! /usr/bin/env python3
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
import shutil
try:
	from dialog import Dialog
except:
	from menu import Dialog

def install_icons(dest_dir):
	path=os.path.join(os.getcwd(),"gpvdm_gui","images","icons")

	for root, dirs, files in os.walk(path):
		for file in files:
			if file.endswith(".png") or file.endswith(".svg"):
				src=os.path.join(root, file)
				dest=os.path.join(dest_dir,root[len(path)+1:],"mimetypes",file)
				if os.path.islink(dest) or os.path.isfile(dest):
					os.unlink(dest)
				print(src,dest)
				try:
					shutil.copyfile(src, dest)
				except:
					pass
def soft_install(d):
	if os.geteuid() != 0:
		d.msgbox("You need to be root to do a soft install")
		return

	if d.yesno("Run soft install") == d.OK:
		bindir=open(os.path.join(os.getcwd(),"gpvdm_core","src","bindir")).readline().rstrip()
		datadir=open(os.path.join(os.getcwd(),"gpvdm_core","src","datadir")).readline().rstrip()


		bindir_gpvdm=os.path.join(bindir,"gpvdm")
		install_of_gpvdm_core=os.path.join(bindir,"gpvdm_core")

		bindir_thumbnailer=os.path.join(bindir,"gpvdm-thumbnailer")

		datadir_thumbnailer=os.path.join(datadir,"thumbnailers","gpvdm.thumbnailer")

		icon=os.path.join(datadir,"icons","hicolor","scalable","mimetypes","simulation-gpvdm.svg")
		mime_dir=os.path.join(datadir,"mime")
		mime=os.path.join(mime_dir,"packages","gpvdm-gpvdm.xml")
		desktop_dir=os.path.join(datadir,"applications")
		desktop=os.path.join(desktop_dir,"gpvdm.desktop")

		

		if os.path.islink(bindir_gpvdm) or os.path.isfile(bindir_gpvdm):
			os.unlink(bindir_gpvdm)
		os.symlink( os.path.join(os.getcwd(),"gpvdm"), bindir_gpvdm)

		if os.path.islink(install_of_gpvdm_core) or os.path.isfile(install_of_gpvdm_core):
			os.unlink(install_of_gpvdm_core)
		os.symlink( os.path.join(os.getcwd(),"gpvdm_core","gpvdm_core"), install_of_gpvdm_core)

		return

		if os.path.islink(bindir_thumbnailer) or os.path.isfile(bindir_thumbnailer):
			os.unlink(bindir_thumbnailer)
		os.symlink( os.path.join(os.getcwd(),"gui","gpvdm-thumbnailer.py"), bindir_thumbnailer)

		install_icons(os.path.join(datadir,"icons","hicolor"))

		if os.path.islink(datadir_thumbnailer) or os.path.isfile(datadir_thumbnailer):
			os.unlink(datadir_thumbnailer)
		shutil.copyfile(os.path.join(os.getcwd(),"desktop","gpvdm.thumbnailer"), datadir_thumbnailer)

		if os.path.islink(mime) or os.path.isfile(mime):
			os.unlink(mime)
		shutil.copyfile(os.path.join(os.getcwd(),"desktop","gpvdm-gpvdm.xml"), mime)

		if os.path.islink(desktop) or os.path.isfile(desktop):
			os.unlink(desktop)
		shutil.copyfile(os.path.join(os.getcwd(),"desktop","gpvdm.desktop"), desktop)

		os.system("update-desktop-database "+desktop_dir)

		os.system("update-mime-database "+mime_dir)

		os.system("gtk-update-icon-cache /usr/share/icons/hicolor/")


def soft_install_menu(d):
	while(1):
		menu=[]
		menu.append(("(softinstall)", "Do soft install"))
		menu.append(("(exit)", "Exit"))

		code, tag = d.menu("gpvdm build system:", choices=menu)
		if code == d.OK:
			if tag=="(softinstall)":
				soft_install(d)

			if tag=="(exit)":
				break


	



