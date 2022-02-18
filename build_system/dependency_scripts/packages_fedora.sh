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


dnf -y install zlib-devel libzip-devel libmatheval-devel suitesparse-devel openssl-devel gsl-devel libcurl-devel blas-devel librsvg2-tools

dnf -y install texlive ghostscript ImageMagick mencoder valgrind @development-tools fedora-packager mingw32-gcc gnuplot


dnf -y install python-crypto python-awake python3-qt5-devel python3-crypto python3-matplotlib-qt5 python3-openpyxl python3-pyopengl numpy notify-python python-inotify.noarch python-matplotlib python-inotify python-matplotlib python3-dialog

dnf -y install indent unifdef indent libcurl-devel poedit ElectricFence kcachegrind help2man

#windows build chain
dnf -y install mingw32-binutils
yum -y install mingw32-gcc
echo "Done"
