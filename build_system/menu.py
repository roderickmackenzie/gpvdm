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


