#!/bin/bash
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012-2016 Roderick C. I. MacKenzie
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

#Tested in Ubuntu 18.04

#build system
apt-get install python3-dialog


apt-get -y install libsuitesparse-dev indent unifdef libsuitesparse-dev libssl-dev  libzip-dev libgsl-dev libmatheval-dev pluma build-essential  autoconf codespell librsvg2-bin gnuplot libsuperlu-dev

apt-get -y install rsync pluma build-essential autoconf python-bashate codespell complexity apt-file gettext-lint gettext-lint inkscape pep8 i18nspector python-bashate automake pbuilder python3-dev


apt-get -y install python3 python3-matplotlib l python3-opengl python3-numpy python3-crypto python3-dbus.mainloop.pyqt5 python3-psutil python3-dateutil

apt-get -y install libqt5multimedia5-plugins 
apt-get -y install python3-pyqt5.qtmultimedia python3-pyqt5.qtopeng

#removed
#libedbus-dev
apt-get install libdbus-1-dev
apt-get -y install dh-virtualenv debhelper texlive 
apt-get -y install mencoder
apt-get install libzip-dev
apt-get install help2man
apt-get install gettext
apt-get install imagemagick
apt-get install license-reconcile
apt-get install libcurl4-openssl-dev
#because ubuntu seems odd
apt-get install python3-pip
pip3 install --upgrade PyOpenGL
pip3 install --upgrade numpy
pip3 install --upgrade matplotlib
pip3 install pyopengl
pip3 install pydbus
pip3 install dbus-python
pip3 install pyqt5

#image processing
apt-get install libpng-dev

#mpi/mumps
apt-get install libopenmpi-dev libopenmpi2
apt-get install libmumps-dev

#apt-get install libopenblas-base for paralel blas

#debug tools
apt-get install electric-fence kdiff3
apt-get install valgrind kcachegrind graphviz
echo "Done"
