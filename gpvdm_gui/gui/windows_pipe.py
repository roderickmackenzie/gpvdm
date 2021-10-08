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

## @package windows_pipe
#  A windows pipe class for communication with gpvdm_core
#


import win32pipe
import win32file
import winerror
from threading import Thread

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget

class win_pipe(QWidget):
	new_data = pyqtSignal(str)
	
	def __init__(self):
		QWidget.__init__(self)

	def rod(self,p):
		while(1):
			#print "going to read"
			try:
				res,data = win32file.ReadFile(p, 4096)
				#print(res,data)
				if res != winerror.ERROR_MORE_DATA:
					data=data.decode("utf-8") 
					#print("decode",res,data)
					self.new_data.emit(data)
				else:
					print("no more data")
					break
			except:
				print("pipe closed")
				break
#		win32pipe.DisconnectNamedPipe(p)


	def foo(self,n):
		while(1):
			p = win32pipe.CreateNamedPipe(r'\\.\pipe\gpvdm_pipe',
				win32pipe.PIPE_ACCESS_INBOUND,
				win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_WAIT,
				255, 65536, 65536,300,None)

			win32pipe.ConnectNamedPipe(p, None)
		
			th = Thread(target=self.rod, args=(p,))
			th.daemon = True
			th.start()

	def start(self):
		p = Thread(target=self.foo, args=(10,))
		p.daemon = True
		p.start()


