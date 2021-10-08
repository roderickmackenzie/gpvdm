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

## @package status_icon
#  The status icon.
#

import sys
import os

from win_lin import running_on_linux

import i18n
_ = i18n.language.gettext

from gui_enable import gui_get
if gui_get()==True:
	from PyQt5.QtWidgets import QSystemTrayIcon,QMenu,QApplication
	from about import about_dlg
	import webbrowser
	from icon_lib import icon_get

from cluster import cluster



statusicon = None

if gui_get()==True:
	class tray_icon(QSystemTrayIcon):

		def __init__(self,  parent=None):
			QSystemTrayIcon.__init__(self, icon_get("ball_green"), parent)
			menu = QMenu(parent)
			self.menu_about = menu.addAction(_("About"))
			self.menu_about.triggered.connect(self.callback_about)
			self.menu_man = menu.addAction(_("Manual"))
			self.menu_man.triggered.connect(self.callback_man)

			self.menu_youtube = menu.addAction("&"+_("Youtube channel"))
			self.menu_youtube.triggered.connect(self.callback_youtube)

			self.exitAction = menu.addSeparator()		
			self.exitAction = menu.addAction(_("Quit"))
			
			self.exitAction.triggered.connect(self.callback_exit)
			self.setContextMenu(menu)

		def callback_exit(self):
			QApplication.quit()

		def callback_about(self):
			dlg=about_dlg()
			dlg.exec_()

		def callback_man(self):
			webbrowser.open("https://www.gpvdm.com/man.html")
		def	callback_youtube(self):
			webbrowser.open("https://www.youtube.com/channel/UCbm_0AKX1SpbMMT7jilxFfA")


def status_icon_init():
	if gui_get()==True:
		global statusicon
		statusicon=tray_icon()
		statusicon.show()

def status_icon_run(cluster):
	if gui_get()==True:
		global statusicon
		if cluster==False:
			statusicon.setIcon(icon_get("ball_red"))
		else:
			statusicon.setIcon(icon_get("ball_red4"))

def status_icon_stop(cluster):
	if gui_get()==True:
		global statusicon
		if cluster==False:
			statusicon.setIcon(icon_get("ball_green"))
		else:
			statusicon.setIcon(icon_get("ball_green4"))

def status_icon_get():
	if gui_get()==True:
		global statusicon
		return statusicon
