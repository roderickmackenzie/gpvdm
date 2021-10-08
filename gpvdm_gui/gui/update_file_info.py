# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2008-2022 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#   
#   https://www.gpvdm.com
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

## @package update_file_info
#  A structure to store the status of updated files in.
#


class update_file_info():
	def __init__(self):
		self.file_name=""
		self.size=-1
		self.md5_web="none"
		self.md5_disk="none"
		self.status="none"
		self.text="none"
		self.downloaded=0
		self.target="none"
		self.progress=0.0
		self.ver="0.0"
		self.cache_dir="none"

	def get_status(self):
		if self.status=="downloading":
			return "Downloading"+" "+str(int(100.0*(self.progress/self.size)))+"%"
		elif self.status=="on-disk":
			return "Downloaded"
		elif self.status=="installing":
			return "Installing "+str(int(self.progress))+"%"
		elif self.status=="up-to-date":
			return "Installed"
		elif self.status=="gpvdm-too-old":
			return "Upgrade gpvdm to install the update."			
		else:
			return self.status

	def __str__(self):
		a=""
		a=a+self.file_name+"\n"
		a=a+self.text+"\n"
		a=a+self.ver+"\n"
		a=a+str(self.size)+"\n"
		a=a+str(self.target)+"\n"
		a=a+self.status+"\n"
		a=a+self.cache_dir
		return a

	def __eq__(self,f):
		return self.file_name==f.file_name

	def get_status_line(self):
		return self.file_name+","+self.text+","+self.target+","+self.md5_web+","+self.md5_disk+","+self.status+","+str(self.size)

	def decode_from_disk(self,data):
		s=data.split(",")
		self.file_name=s[0]
		self.text=s[1]
		self.target=s[2]
		self.md5_web=s[3]
		self.md5_disk=s[4]
		self.status=s[5]
		self.size=int(s[6])


	def decode_from_web(self,data):
		s=data.split(",")
		self.file_name=s[0]
		self.text=s[1]
		self.target=s[2]
		self.md5_web=s[3]
		self.size=int(s[4])


		#self.file_name=i[0][1:]
		#self.description=i[1]
		#self.md5=i[2]
		#self.ver=i[3]
		#self.size=int(i[4])
		#self.target=i[5]
		#self.status=i[6]
		#self.cache_dir=i[7]

