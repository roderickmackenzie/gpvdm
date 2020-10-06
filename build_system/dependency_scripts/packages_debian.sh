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


apt-get -y install indent unifdef libsuitesparse-dev libssl-dev libedbus-dev libzip-dev libgsl-dev libmatheval-dev help2man pluma build-essential imagemagick license-reconcile autoconf librsvg2-bin zip libsuperlu-dev


apt-get -y install rsync pluma build-essential imagemagick license-reconcile autoconf python-bashate codespell apt-file gettext-lint inkscape pep8 i18nspector gettext git texlive-latex-recommended latex2html


apt-get -y install python3-numpy python3 python3-matplotlib python3-pyqt5 python3-pyqt5.qtopengl python3-opengl python3-numpy python3-crypto python3-dbus.mainloop.pyqt5 python3-psutil python3-openpyxl

apt-get -y install gnuplot

apt-get -y install python3-pyqt5.qtmultimedia python3-pyqt5.qtopeng

#build enviroment
apt-get -y install octave gnuplot rlwrap
echo "Done"
