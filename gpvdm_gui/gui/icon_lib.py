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

## @package icon_lib
#  An icon cache.
#

import sys
import os
from cal_path import get_icon_path
from win_lin import running_on_linux
from cal_path import get_image_file_path
from cal_path import gpvdm_paths
from gpvdm_local import gpvdm_local

try:
	from PyQt5.QtCore import QSize, Qt
	from PyQt5.QtGui import QPainter,QIcon
except:
	pass


icon_db=[]

use_theme=None
class icon:
	def __init__(self):
		self.name=[]
		self.file_name=""
		self.icon16x16=None
		self.icon32x32=None
		self.icon64x64=None

class icon_data_base:
	def __init__(self):
		self.db=[]
		self.load()

	def load(self):
		data=gpvdm_local()
		use_theme=data.gui_config.gui_use_icon_theme

		path_16=os.path.join(get_image_file_path(),"16x16")
		path_32=os.path.join(get_image_file_path(),"32x32")
		path_64=os.path.join(get_image_file_path(),"64x64")

		for f in os.listdir(path_32):
			if f.endswith("png"):
				my_icon=icon()
				my_icon.name.append(f.split(".")[0])
				my_icon.file_name=f.split(".")[0]		#no ext
				found=False
				if running_on_linux()==True and use_theme==True:
					image=QIcon()
					if image.hasThemeIcon(my_icon.name[0])==True:
						my_icon.icon16x16=image.fromTheme(my_icon.name[0])
						my_icon.icon32x32=image.fromTheme(my_icon.name[0])
						my_icon.icon64x64=image.fromTheme(my_icon.name[0])
						found=True
				if found==False:
					my_icon.icon16x16=QIcon(os.path.join(path_16,my_icon.file_name+".png"))
					my_icon.icon32x32=QIcon(os.path.join(path_32,my_icon.file_name+".png"))
					my_icon.icon64x64=QIcon(os.path.join(path_64,my_icon.file_name+".png"))

				self.db.append(my_icon)

		for line in data.icon_lib.var_list:
			for i in range(0,len(self.db)):
				if line[1] == self.db[i].file_name:
					self.db[i].name.append(line[0])


	def dump(self):
		for i in range(0,len(self.db)):
			print(self.db[i].name,self.db[i].file_name)

	def icon_get(self,token,size=-1):
		for i in range(0,len(self.db)):
			if token in self.db[i].name:
				if size==16:
					return self.db[i].icon16x16
				elif size==32:
					return self.db[i].icon32x32
				elif size==64 or size==-1:
					return self.db[i].icon64x64

		return False


def icon_init_db():
	global icon_db
	icon_db=icon_data_base()

def icon_get_db():
	global icon_db
	return icon_db

def icon_get(token,size=-1,sub_icon=None):
	global icon_db
	if token!=".png" and token.endswith(".png")==True:
		token=token[:-4]

	if sub_icon==None:
		return icon_db.icon_get(token,size=size)

	icon_ret=icon_db.icon_get(token)


	if icon_ret!=False:
		if sub_icon!=None:
			icon1=icon_ret
			icon2=icon_db.icon_get(sub_icon)

			icon1_pixmap=icon1.pixmap(QSize(48,48))
			icon2_small = icon2.pixmap(QSize(48,48)).scaled(QSize(24,24), Qt.KeepAspectRatio, Qt.SmoothTransformation);

			p=QPainter(icon1_pixmap)
			p.drawPixmap(24,24,icon2_small); 
			p.end()


			icon_ret=QIcon(icon1_pixmap)

		return icon_ret
	else:
		return False
