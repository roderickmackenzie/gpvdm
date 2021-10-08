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


import os
import locale
from tail import tail

class Dialog():

	def __init__(self,dialog="dialog"):
		self.OK=0
		self.CANCEL=1
		return

	def clear(self):
		print(chr(27) + "[2J")

	def set_background_title(self,text):
		print(text)

	def tailbox(self,fname, height=None, width=100):
		tail(fname)
		return

	def yesno(self,text):
		#self.clear()
		n=input(text+" (y/n) [y]")
		if n=="y" or n=="":
			return self.OK
		return self.cancel

	def msgbox(self,text):
		print(text)
		n=input('Press enter:')
		return

	def menu(self,text, choices=[]):
		#self.clear()
		print("The python3 module Dialog is not installed")
		print("If you are on Ubuntu/Debian system, try apt-get install python3-dialog")
		print("If you are on Fedora/Redhat system, try yum install python3-dialog\n\n")
		for i in range(0,len(choices)):
			print(i,choices[i][1])

		code=self.OK

		n=input('Enter your input: [0]')
		if n!="":
			n=int(n)
		else:
			n=0	
		tag=choices[n][0]	
		return code, tag

	def inputbox(self,text):
		print(text)
		text_out=input(':')

		return self.OK,text_out
