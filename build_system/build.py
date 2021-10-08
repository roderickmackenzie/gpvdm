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
import sys

sys.path.append('./build_system/')

import os
import locale
import argparse
import platform
from code_ctrl import is_rod


from buildpackage import buildpackage_menu
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
		if is_rod()==True:
			menu.append(("(buildpackage)", "Build package"))
			menu.append(("(publish)", "Publish to web"))


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

