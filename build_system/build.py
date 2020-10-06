#! /usr/bin/env python3
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
import sys

sys.path.append('./build_system/')

import os
import locale
import argparse
import platform


from cal_path import build_setup_paths

from package_menu import package_menu
from compile_menu import compile_menu
from system_install import system_install_menu
from git_pull import git_pull

build_setup_paths()

parser = argparse.ArgumentParser()
parser.add_argument("--text", help="Text mode", action='store_true')

args = parser.parse_args()

if args.text:
	from menu import Dialog
else:
	try:
		from dialog import Dialog
	except:
		from menu import Dialog

if os.geteuid() != 0:
	git_pull()

# You may want to use 'autowidgetsize=True' here (requires pythondialog >= 3.1)
d = Dialog(dialog="dialog")

# Dialog.set_background_title() requires pythondialog 2.13 or later
d.set_background_title("https://www.gpvdm.com build configure, Roderick MacKenzie 2018")


while(1):
	menu=[]

	if os.geteuid() == 0:
		menu.append(("(packages)", "Install dependencies to compile"))
		menu.append(("(systeminstall)", "Install/Remove gpvdm"))
	else:
		menu.append(("(compile)", "Compile gpvdm"))
		menu.append(("(packages)", "Install dependencies to compile"))

	menu.append(("(about)", "About"))

	menu.append(("(exit)", "Exit"))

	code, tag = d.menu("gpvdm build system:", choices=menu)
	if code == d.OK:
		if tag=="(publish)":
			publish_menu(d)

		if tag=="(packages)":
			package_menu(d)

		if tag=="(compile)":
			compile_menu(d)

		if tag=="(systeminstall)":
			system_install_menu(d)


		if tag=="(buildpackage)":
			buildpackage_menu(d)

		if tag=="(about)":
			d.msgbox("This is the gpvdm build system, use it to configure the build system, make, and install gpvdm. Copyright Roderick MacKenzie 2018.  Released under the GPL v2 license.")


		if tag=="(exit)":
			break

		print(tag)
	else:
		break

