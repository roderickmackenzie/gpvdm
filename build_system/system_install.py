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





