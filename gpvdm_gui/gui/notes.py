#!/usr/bin/env python2.7
# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2012-2017 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#
#   https://www.gpvdm.com
#   Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License v2.0, as published by
#   the Free Software Foundation.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
# 

## @package notes
#  A simple text editor - needs a rewrite.
#

#import sys
import os
#import shutil
#from numpy import *
#from matplotlib.figure import Figure
#from numpy import arange, sin, pi
#import gobject


articles = []
mesh_articles = []

class notes(gtk.Window):


	def callback_close(self, widget, data=None):
		self.hide()
		return True

	def callback_help(self, widget, data=None):
		cmd = 'firefox http://www.roderickmackenzie.eu/gpvdm_wiki.html'
		os.system(cmd)

	def enter_callback(self, widget, entry):
		print("edit")
		#entry_text = self.entry.get_buffer()
		entry_text=self.buf.get_text(*self.buf.get_bounds())
		a = open(os.path.join(self.sim_dir,"notes.inp"), "w")
		a.write(entry_text)
		a.close()

	def init(self,sim_dir):
		self.sim_dir=sim_dir
		self.set_size_request(500, 500)
		self.set_title("Notes")
		self.entry = gtk.TextView()
		self.buf=gtk.TextBuffer()
		self.buf.connect("changed", self.enter_callback, self.entry)
		lines=""
		if os.path.isfile(os.path.join(self.sim_dir,"notes.inp")):
			f = open(os.path.join(self.sim_dir,"notes.inp"))
			lines = f.read()
			f.close()
		self.buf.set_text(lines)
		self.entry.set_buffer(self.buf)
		self.add(self.entry)
		self.show_all()



