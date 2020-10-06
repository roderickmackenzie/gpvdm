#!/usr/bin/env python3

import os
import sys
try:
	from dialog import Dialog
except:
	from menu import Dialog


from generic_install import generic_install
import platform

def package_menu(d):
	if os.geteuid() != 0:
		d.msgbox("You need to be root to install packages")
		return
	menu=[]
	plat=platform.dist()[0].lower()
	chipset=os.uname().machine
	configured=False

	if d.yesno("Install packages for "+plat+" "+chipset) == d.CANCEL:
		return
	if plat=="fedora":
		configured=True
		generic_install(d,"build_system/dependency_scripts/packages_fedora.sh")
	elif plat=="debian":
		configured=True
		generic_install(d,"build_system/dependency_scripts/packages_debian.sh")
		ret=d.tailbox("out.dat", height=100, width=100)
	elif plat=="ubuntu":
		configured=True
		generic_install(d,"build_system/dependency_scripts/packages_ubuntu.sh")
		ret=d.tailbox("out.dat", height=None, width=100)
	elif plat=="centos":
		configured=True
		generic_install(d,"build_system/dependency_scripts/packages_centos.sh")
		ret=d.tailbox("out.dat", height=None, width=100)
	elif plat=="arch":
		configured=True
		generic_install(d,"build_system/dependency_scripts/arch_install.sh")
	elif plat=="mint":
		configured=True
		generic_install(d,"build_system/dependency_scripts/packages_mint.sh")
		ret=d.tailbox("out.dat", hieght=None, width=100)
	elif plat=="suse":
		configured=True
		generic_install(d,"build_system/dependency_scripts/packages_suse.sh")
		ret=d.tailbox("out.dat",height=None, width=100)
	elif plat=="raspbian":
		configured=True
		generic_install(d,"build_system/dependency_scripts/packages_raspbian.sh")
		ret=d.tailbox("out.dat",height=None, width=100)
	else:
		d.msgbox("I don't know how to install packages for your distribution, file a bug report if you would like your distro added to gpvdm.")


if __name__ == "__main__":
	d = Dialog(dialog="dialog")

	d.set_background_title("gpvdm build configure")
	package_menu(d)
