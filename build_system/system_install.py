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
from soft_install import soft_install

try:
	from dialog import Dialog
except:
	from menu import Dialog

def install(d):
	if os.geteuid() != 0:
		d.msgbox("You need to be root to install packages")
		return

	if d.yesno("Run install") == d.OK:
		os.system("make install  >out.dat 2>out.dat &")
		et=d.tailbox("out.dat", height=None, width=150)


def uninstall(d):
	if os.geteuid() != 0:
		d.msgbox("You need to be root to install packages")
		return

	if d.yesno("Run uninstall") == d.OK:
		os.system("make uninstall  >out.dat 2>out.dat &")
		et=d.tailbox("out.dat", height=None, width=150)

def desktopinstall(d):
	soft_install(d)

def system_install_menu(d):
	while(1):
		menu=[]
		menu.append(("(install)", "Install"))
		menu.append(("(uninstall)", "Uninstall"))
		menu.append(("(desktop)", "Link xdesktop hooks to this dir"))
		menu.append(("(exit)", "Exit"))

		code, tag = d.menu("gpvdm build system:", choices=menu)
		if code == d.OK:
			if tag=="(install)":
				install(d)

			if tag=="(uninstall)":
				uninstall(d)

			if tag=="(desktop)":
				desktopinstall(d)

			if tag=="(exit)":
				break





