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

## @package server
#  The server module, used to run simulations across all CPUs, also has interface to the HPC class.
#

import sys
import os

from str2bool import str2bool

from inp import inp_get_token_value
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
from inp import inp_search_token_value
from stat import *
from encrypt import encrypt
from encrypt import decrypt
from encrypt import encrypt_load
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

class node:
	ip=""
	load=""
	cpus=""

class job:
	name=""
	path=""
	ip=""
	args=""
	start=""
	stop=""
	cpus=1
	status=0
	full_command=""	#debug only

class base_server():
	def __init__(self):
		self.running=False
		self.progress_window=progress_class()
		self.stop_work=False
		self.update_cpu_number()
		self.clear_jobs()
		self.callback=None

	def clear_jobs(self):
		self.jobs=[]
		self.jobs_running=0
		self.jobs_run=0
		self.finished_jobs=[]
		self.start_time=0
		self.jobs_per_second=0
		self.pipe_to_null=True

	def base_server_init(self,sim_dir):
		self.sim_dir=sim_dir

	def update_cpu_number(self):
		self.cpus=multiprocessing.cpu_count()
		if self.cpus>4:
			self.cpus=self.cpus-2

		data=inp()
		if data==False:
			return

		data.load(os.path.join(get_sim_path(),"server.inp"))
		max=data.get_token("#max_gpvdm_instances")

		if max=="false" or max=="False" or max== "none":
			return

		if max!=False:
			max=int(max)
			self.cpus=max

	def add_job(self,path,arg):
		j=job()
		j.path=path
		j.args=arg
		j.status=0
		j.name="job"+str(len(self.jobs))
		self.jobs.append(j)

	def run_now(self):
		self.exe_command(get_sim_path(),get_exe_command(),background=False)

	def check_warnings(self):
		message=""
		problem_found=False
		#print(len(self.jobs))

		for i in range(0,len(self.jobs)):
			log_file=os.path.join(self.jobs[i].path,"log.dat")
			if os.path.isfile(log_file):
				f = open(log_file, "r")
				lines = f.readlines()
				f.close()
				found=""
				for l in range(0, len(lines)):
					lines[l]=lines[l].rstrip()
					if lines[l].startswith("error:") or lines[l].startswith("warning:"):
						#print("whoo",lines[l])
						found=found+lines[l]+"\n"
						problem_found=True
				if len(found)!=0:
					message=message+self.jobs[i].path+":\n"+found+"\n"
				else:
					message=message+self.jobs[i].path+":OK\n\n"
		if problem_found==False:
			message=""

		return message

	def print_jobs(self):
		print("server job list:")
		for i in range(0, len(self.jobs)):
			print("job",i,self.jobs[i].path,self.jobs[i].args,self.jobs[i].status,self.jobs[i].start,self.jobs[i].stop)	#,self.jobs[i].full_command
		print("jobs running=",self.jobs_running,"jobs run=",self.jobs_run,"cpus=",self.cpus)


	def exe_command(self,path,command,background=True):
		cmd="cd "+path+";"
		cmd=cmd+command
		if self.pipe_to_null==True:
			cmd=cmd+" >/dev/null "
			if background==True:
				cmd=cmd+" &"

		cmd=cmd+"\n"
		#print(cmd)
		os.system(cmd)

	def base_server_process_jobs(self):
		path=True
		while(path!=False):
			#self.print_jobs()
			path,command=self.base_server_get_next_job_to_run(lock_file=True)
			if path!=False:
				self.exe_command(path,command)
				if len(self.jobs)>1:
					jobs_per_second="%.2f" % self.jobs_per_second
					self.progress_window.set_text(_("Running job ")+path+" jobs/s="+jobs_per_second)
					self.progress_window.set_fraction(float(self.jobs_run)/float(len(self.jobs)))

			else:
				return

	def remove_lock_files(self):
		ls=os.listdir(self.sim_dir)
		#print(">>>>>>",self.sim_dir,ls)
		for i in range(0, len(ls)):
			if ls[i][:4]=="lock" and ls[i][-4:]==".dat":
				del_file=os.path.join(self.sim_dir,ls[i])
				#print("delete file:",del_file)
				os.remove(del_file)

	def simple_run(self):
		self.start_time=time.time()
		self.stop_work=False
		self.remove_lock_files()
				
		self.base_server_process_jobs()

		while(1):
			ls=os.listdir(self.sim_dir)
			#print(self.sim_dir,ls)
			for i in range(0, len(ls)):
				if ls[i][:4]=="lock" and ls[i][-4:]==".dat":
					lock_file=ls[i]
					os.remove(os.path.join(self.sim_dir,lock_file))
					#os.rename(os.path.join(self.sim_dir,lock_file),os.path.join("./old",lock_file))
					#self.jobs_run=self.jobs_run+1
					#self.jobs_running=self.jobs_running-1
					job=int(lock_file[4:-4])
					self.base_job_finished(job)
			self.base_server_process_jobs()
			time.sleep(0.1)

			if self.jobs_run==len(self.jobs):
				self.remove_lock_files()
				break

	def base_job_finished(self,job):
		if self.jobs[job].status!=1:
			print("This job never ran!!!",self.jobs[job].status)
			asdsadasd
		self.jobs[job].status=2
		self.jobs[job].stop=str(datetime.now())
		self.jobs_run=self.jobs_run+1
		self.jobs_running=self.jobs_running-1
		self.jobs_per_second=self.jobs_run/(time.time()-self.start_time)

		if self.jobs_running==0:
			if self.callback!=None:
				self.callback()
			self.callback=None

		self.progress_window.set_fraction(float(self.jobs_run)/float(len(self.jobs)))

	def base_server_set_callback(self,callback):
		self.callback=callback

	def base_server_get_next_job_to_run(self,lock_file=False):
		if (len(self.jobs)==0):
			return False,False

		for i in range(0, len(self.jobs)):
			if (self.jobs_running<self.cpus):
				if self.jobs[i].status==0:
					self.jobs[i].status=1
					self.jobs[i].start=str(datetime.now())
					self.jobs[i].ip=socket.gethostbyname(socket.gethostname())
					#for r in range(0,len(self.jobs)):
					#	print(self.jobs[i],self.args[i])
					
					self.jobs_running=self.jobs_running+1

					if lock_file==True:
						command_lock=" --lockfile "+os.path.join(self.sim_dir,"lock"+str(i)+".dat")
					else:
						command_lock=" --lock "+"lock"+str(i)

					full_command=get_exe_command()+command_lock+" "+self.jobs[i].args+" "+get_exe_args()
					self.jobs[i].full_command=full_command
					return self.jobs[i].path,full_command

		return False,False


	def killall(self):
		self.stop_work=True
		if self.cluster==True:
			self.cluster_killall()
		#else:
		tx_to_core("terminate")
		#	if running_on_linux()==True:
		#		cmd = 'killall '+get_exe_name()
		#		os.system(cmd)
		#		print(cmd)
		#	else:
		#		cmd="taskkill /im "+get_exe_name()
		#		print(cmd)
		#		os.system(cmd)

		self.gui_sim_stop()

if gui_get()==True:
	class server(QWidget,base_server,cluster):
		
		sim_finished = pyqtSignal()
		sim_started = pyqtSignal()

		def __init__(self):
			QWidget.__init__(self)
			base_server.__init__(self)
			self.display=False
			self.fit_update=None
			self.excel_workbook_gen_error=False
			status_icon_init()
			self.gui_update_time= time.time()
			self.timer=QTimer()

		def init(self,sim_dir):
			self.base_server_init(sim_dir)

			self.cluster_init()
			#self.cluster=str2bool(inp_get_token_value("server.inp","#cluster"))



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
				path,command=self.base_server_get_next_job_to_run()
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
			if self.terminal.test_free_cpus()!=0:
				self.process_jobs()


		def timer_start(self):
			self.timer.timeout.connect(self.my_timer)
			self.timer.start(100)

		def callback_dbus(self,data_in):
			if data_in.startswith("hex"):
				data_in=data_in[3:]
				data=codecs.decode(data_in, 'hex')
				data=data.decode('ascii')
				#print("!!!!!!!!!!!!!!!!! ",data)
				if data.startswith("lock"):
					if len(self.jobs)==0:
						print(_("I did not think I was running any jobs"),data)
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

