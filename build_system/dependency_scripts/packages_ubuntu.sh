#!/bin/bash
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2008-2021 Roderick C. I. MacKenzie
#
#	r.c.i.mackenzie at googlemail.com
#	https://www.gpvdm.com
#	Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License v2.0, as published by
#    the Free Software Foundation.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#Tested in Ubuntu 20.04

#This script will install all the dependencies needed by gpvdm for Ubuntu
#it will install everything needed to build it and run it.
#running the script on a decent VM with a decent internet connection takes about 20 min
#from a fresh version of ubuntu.  The script must be run as root.
#Rod 21/7/21

#build system
apt-get -y install python3-dialog

#gpvdm_core
if [ "1" -eq "1" ]
then
	apt-get -y install libsuitesparse-dev
	apt-get -y install libzip-dev 
	apt-get -y install autoconf
	apt-get -y install codespell
	apt-get -y install librsvg2-bin
	apt-get -y install gnuplot
	apt-get -y install libsuperlu-dev
	apt-get -y install automake
	apt-get -y install libdbus-1-dev
	apt-get -y install libzip-dev
	apt-get -y install libpng-dev
fi

#development enviroment
if [ "1" -eq "1" ]
then
	apt-get -y install pluma     
	apt-get -y install indent
	apt-get -y install unifdef
	apt-get -y install rsync
	apt-get -y install inkscape
	apt-get -y install complexity
	apt-get -y install help2man
	apt-get -y install gettext
	apt-get -y install electric-fence
	apt-get -y install kdiff3
	apt-get -y install valgrind
	apt-get -y install kcachegrind
	apt-get -y install graphviz
fi

#gpvdm_gui
apt-get -y install python3
apt-get -y install python3-opengl python3-numpy python3-dbus.mainloop.pyqt5 python3-psutil python3-dateutil
apt-get -y install libqt5multimedia5-plugins 
apt-get -y install python3-pyqt5.qtmultimedia python3-pyqt5.qtopeng
apt-get -y install python3-pip
apt-get -y install python3-matplotlib

#gpvdm_data
apt-get -y install texlive
apt-get install texlive-latex-extra

#for latex minted for nice source code in boxes in manual
apt-get install python-pygments
apt install python3-pygments

#use these to fix python distro
#pip3 install --upgrade PyOpenGL
#pip3 install --upgrade numpy
#pip3 install --upgrade matplotlib
#pip3 install pyopengl
#pip3 install pydbus
#pip3 install dbus-python
#pip3 install pyqt5

#image processing

apt-get -y install mencoder
apt-get -y install imagemagick

#mpi/mumps
apt-get -y install libopenmpi-dev libopenmpi2
apt-get -y install libmumps-dev

#apt-get install libopenblas-base for paralel blas

#building deb packages for ubuntu
apt-get -y install devscripts
apt-get -y install debhelper
apt-get -y install build-essential
apt-get -y install dh-python
apt-get -y install python-bashate
apt-get -y install apt-file
apt-get -y install gettext-lint
apt-get -y install pep8
apt-get -y install i18nspector
apt-get -y install pbuilder
apt-get -y install python3-dev
apt-get -y install dh-virtualenv
#apt-get -y install license-reconcile
echo "Done"

#remove apt-get remove libopenblas0-pthread
