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

## @package server
#  The server module, used to run simulations across all CPUs, also has interface to the HPC class.
#

import sys
import os

from str2bool import str2bool

import threading
import multiprocessing
import time
from cal_path import get_exe_name
from cal_path import get_exe_args

from time import sleep
from win_lin import running_on_linux
import subprocess
from util import gui_print_path
from progress_class import progress_class

from cal_path import get_exe_command
from sim_warnings import sim_warnings
from stat import *
import i18n
_ = i18n.language.gettext
from cluster import cluster


from status_icon import status_icon_init
from status_icon import status_icon_run
from status_icon import status_icon_stop

import codecs
from gui_enable import gui_get

if gui_get()==True:
	from help import help_window
	from PyQt5.QtCore import pyqtSignal
	from PyQt5.QtWidgets import QWidget
	from PyQt5.QtCore import QTimer
	from process_events import process_events

from server_io import server_find_simulations_to_run
from inp import inp

import time

from cal_path import get_sim_path
from gui_enable import gui_get
from job import job
import socket

my_server=False

from datetime import datetime
from gui_hooks import tx_to_core
from lock import get_lock
from gpvdm_json import gpvdm_data

from server_base import server_base

if gui_get()==True:
	class server(QWidget,server_base,cluster):
		
		sim_finished = pyqtSignal()
		sim_started = pyqtSignal()

		def __init__(self):
			QWidget.__init__(self)
			server_base.__init__(self)
			self.display=False
			self.fit_update=None
			status_icon_init()
			self.gui_update_time= time.time()
			self.timer=QTimer()
			self.terminal=None

		def init(self,sim_dir):
			self.server_base_init(sim_dir)

			self.cluster_init()


		def set_terminal(self,terminal):
			self.terminal=terminal
		
		def set_display_function(self,display):
			self.display=display

		def set_fit_update_function(self,fit_update):
			self.fit_update=fit_update

		def gui_sim_start(self):
			help_window().hide()
			self.progress_window.start()
			status_icon_run(self.cluster)
			self.sim_started.emit()

		def gui_sim_stop(self):
			text=self.check_warnings()
			self.progress_window.stop()
			help_window().show()
			status_icon_stop(self.cluster)

			help_window().help_set_help(["plot.png",_("<big><b>Simulation finished!</b></big><br>Click on the plot icon to plot the results")])

			if len(text)!=0:
				self.dialog=sim_warnings(text)

			self.jobs_update.emit()
			self.sim_finished.emit()


		def add_job(self,path,arg):
			if self.cluster==False:
				super().add_job(path,arg)
				self.jobs_update.emit()
			else:
				self.add_remote_job(path)
				self.send_dir(path,"")



		def start(self):
			self.finished_jobs=[]
			if self.interactive_cluster==True or self.cluster==False:
				self.progress_window.show()


				self.gui_sim_start()

			self.running=True
			self.run_jobs()


		def run_jobs(self):
			self.stop_work=False
			if self.cluster==True:
				self.cluster_run_jobs()
			else:
				self.update_cpu_number()
				self.timer_start()

		def process_jobs(self):
			
			path=True
			while(path!=False):
				if self.terminal==None:
					return False

				path,command=self.server_base_get_next_job_to_run()
				self.jobs_update.emit()
				if path!=False:

					if self.terminal.run(path,command)==True:
						time.sleep(0.1)
						return True
					else:
						return False
				else:
					return

		def stop(self):
			self.timer.stop()
			
			self.progress_window.set_fraction(0.0)
			if self.interactive_cluster==True or self.cluster==False:
				self.gui_sim_stop()

			self.jobs=[]
			self.jobs_running=0
			self.jobs_run=0
			self.running=False

			if self.display!=False:

				self.display()
				
			#print(_("I have shut down the server."))

		def my_timer(self):
			#This is to give the QProcess timer enough time to update
			if self.terminal!=None:
				if self.terminal.test_free_cpus()!=0:
					self.process_jobs()


		def timer_start(self):
			self.timer.timeout.connect(self.my_timer)
			self.timer.start(100)

		def callback_dbus(self,data_in):
			#print("server_in",data_in)
			if data_in.startswith("hex"):
				data_in=data_in[3:]
				data=codecs.decode(data_in, 'hex')
				data=data.decode('ascii')
				#print("Decoded ",data)
				if data.startswith("lock"):
					if len(self.jobs)==0:
						#print(_("I did not think I was running any jobs"),data)
						self.stop()
					else:
						if self.finished_jobs.count(data)==0:
							job=int(data[4:])
							self.base_job_finished(job)

							self.finished_jobs.append(data)

							self.progress_window.set_fraction(float(self.jobs_run)/float(len(self.jobs)))

							if (self.jobs_run==len(self.jobs)):
								self.stop()

				elif (data=="pulse"):
					if len(self.jobs)==1:
						splitup=data.split(":", 1)
						if len(splitup)>1:
							text=data.split(":")[1]
							self.progress_window.set_text(text)
						#self.progress_window.progress.set_pulse_step(0.01)
						self.progress_window.pulse()
				elif (data.startswith("enable_pulse")):
					splitup=data.split(":", 1)
					if len(splitup)>1:
						value=str2bool(data.split(":")[1])
						self.progress_window.enable_pulse(value)
				elif (data.startswith("percent")):
					if len(self.jobs)==1:
						splitup=data.split(":", 1)
						if len(splitup)>1:
							frac=float(data.split(":")[1])
							self.progress_window.set_fraction(frac)
				elif (data.startswith("text")):
					if len(self.jobs)==1:
						splitup=data.split(":", 1)
						if len(splitup)>1:
							self.progress_window.set_text(data.split(":", 1)[1])
				elif (data.startswith("liblock")):
					splitup=data.split(":", 1)
					get_lock().uid=splitup[1]
				elif (data.startswith("fit_run")):
					elapsed_time = time.time() - self.gui_update_time
					if elapsed_time>5:

						self.gui_update_time=time.time()
					
						if self.fit_update!=None:
							self.fit_update()
							if self.terminal!=None:
								self.terminal.clear()
			else:
				print("rx",data_in)

def server_break():
	global my_server
	if my_server.stop_work==True:
		my_server.stop_work=False
		return True

	return False

def server_init():
	global my_server
	my_server=server()

def server_get():
	global my_server
	return my_server

