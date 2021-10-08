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

## @package progress
#  Progress bar window, when not in GUI mode it will do a text based one.
#


import sys
import time
from numpy import exp
from gui_enable import gui_get
	
class progress_base():
	def __init__(self):
		self.start_time=time.time()
		self.fraction=0.0
		self.finish_time=""
		self.avg=[]
		self.text=""

	def left_time(self):
		if self.fraction!=0:
			delta=(time.time()-self.start_time)*(1.0-self.fraction)/self.fraction
			t=time.time()+delta
			self.avg.append(t)
			if len(self.avg)>100:
				del self.avg[0]
			t=0.0
			
			l=len(self.avg)
			w_sum=0.0
			for i in range(0,l):
				weight=exp(-(l-i)*1e-1)
				t=t+self.avg[i]*weight
				w_sum=w_sum+weight
			t=t/w_sum
			self.finish_time=time.strftime('%A %H:%M:%S', time.localtime(t))

	def draw(self):
		length=40
		full=int(self.fraction*length)
		empty=int((1.0-self.fraction)*length)
		sys.stdout.write('[')
		for i in range(0,full):
			sys.stdout.write('#')

		for i in range(0,empty):
			sys.stdout.write(' ')

		sys.stdout.write("] "+str(self.fraction*100.0)+"% \n")
		sys.stdout.write(self.text+"\n") 
		if self.finish_time!="":
			sys.stdout.write(_("Finish time:")+" "+self.finish_time+" \n")
			sys.stdout.write("\033[F\033[F\033[F")
		else:
			sys.stdout.write("\033[F\033[F")
		sys.stdout.flush()
	
	def enable_pulse(self,value):
		print("pulse")
		
	def set_fraction(self,fraction):
		self.fraction=fraction
		self.draw()
		self.left_time()

	def pulse(self):
		print("pulse")
		
	def start(self):
		self.draw()

	def show(self):
		self.draw()

	def stop(self):
		print("stop")

	def set_text(self,text):
		self.text=text
		self.draw()
	
if gui_get()==True:

	from PyQt5.QtCore import QSize, Qt
	from PyQt5.QtWidgets import QWidget, QVBoxLayout,QHBoxLayout,QLabel,QDesktopWidget
	from gpvdm_progress import gpvdm_progress
	from spinner import spinner

	#from help import my_help_class

	class progress_class(QWidget,progress_base):

		def __init__(self):
			QWidget.__init__(self)
			progress_base.__init__(self)
			self.setWindowFlags(Qt.FramelessWindowHint|Qt.WindowStaysOnTopHint)
			self.setFixedSize(400, 90)

			main_vbox = QVBoxLayout()
			hbox= QHBoxLayout()
			hbox.setContentsMargins(0, 0, 0, 0)
			self.progress = gpvdm_progress()
			self.spinner=spinner()
			hbox.addWidget(self.progress, 0)
			hbox.addWidget(self.spinner, 0)
			w=QWidget()
			w.setLayout(hbox)
			main_vbox.addWidget(w,0)

			self.label=QLabel()
			self.label.setText(_("Running")+"...")
			main_vbox.addWidget(self.label)

			self.label_time=QLabel()
			self.label_time.setText("")
			main_vbox.addWidget(self.label_time)


			self.setLayout(main_vbox)

		def hide_time(self):
			self.label_time.hide()
			self.setFixedSize(400, 70)

		def enable_pulse(self,value):
			self.progress.enablePulse(value)

		def set_fraction(self,fraction):
			self.fraction=fraction
			self.progress.setValue(fraction)
			self.left_time()
			if self.finish_time!="":
				self.label_time.setText(_("Finish time:")+" "+self.finish_time)

		def pulse(self):
			self.progress.pulse()
			
		def start(self):
			shape=QDesktopWidget().screenGeometry()

			w=shape.width()
			h=shape.height()
			win_w=self.frameGeometry().width()
			win_h=self.frameGeometry().height()

			x=w-win_w
			y=0
			self.move(x,y)
			self.show()

		def stop(self):
			self.hide()
			#self.spin.stop()
			#my_help_class.help_show()

		def set_text(self,text):
			text=text
			l=len(text)
			if l>50:
				l=l-50
				text=text[l:]

			self.label.setText(text)

else:
	class progress_class(progress_base):

		def __init__(self):
			progress_base.__init__(self)


