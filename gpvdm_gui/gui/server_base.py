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

## @package server_base
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
from encrypt import encrypt
from encrypt import decrypt
from encrypt import encrypt_load
import i18n
_ = i18n.language.gettext
from cluster import cluster

from gui_enable import gui_get

from server_io import server_find_simulations_to_run
from inp import inp

import time

from cal_path import get_sim_path
from job import job

my_server=False

from datetime import datetime
from gui_hooks import tx_to_core
from lock import get_lock
from gpvdm_json import gpvdm_data
import socket
if gui_get()==True:
	from process_events import process_events

class node:
	ip=""
	load=""
	cpus=""

class job():
	def __init__(self):
		self.name=""
		self.path=""
		self.ip=""
		self.args=""
		self.start_time=0
		self.stop=""		#this needs to go but later
		self.cpus=1
		self.status=0
		self.full_command=""	#debug only

class server_base():
	def __init__(self):
		self.running=False
		self.progress_window=progress_class()
		self.stop_work=False
		self.update_cpu_number()
		self.clear_jobs()
		self.callback=None
		self.max_job_time=None
		self.time_out=False

	def clear_jobs(self):
		self.jobs=[]
		self.jobs_running=0
		self.jobs_run=0
		self.finished_jobs=[]
		self.start_time=0
		self.jobs_per_second=0
		self.pipe_to_null=True

	def server_base_init(self,sim_dir):
		self.sim_dir=sim_dir

	def update_cpu_number(self):
		self.cpus=multiprocessing.cpu_count()
		if self.cpus>4:
			self.cpus=self.cpus-2

		data=gpvdm_data()
		max=data.server.max_gpvdm_instances
		if max==False or str(max)=="none" or max==0:
			return

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

	def remove_debug_info(self):
		for i in range(0, len(self.jobs)):
			file_name=os.path.join(self.jobs[i].path,"gmon.out")
			if os.path.isfile(file_name)==True:
				os.unlink(file_name)

	def print_jobs(self):
		print("server job list:")
		for job in self.jobs:
			if job.status!=2:
				start_time=time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(job.start_time))
				print("job",job.path,job.args,job.status,start_time,job.stop)	#,job.full_command
		print("jobs running=",self.jobs_running,"jobs run=",self.jobs_run,"cpus=",self.cpus)


	def exe_command(self,path,command,background=True):
		if running_on_linux()==True:
			command_sep=";"
		else:
			command_sep=" & "

		cmd="cd "+path+command_sep
		cmd=cmd+command
		if running_on_linux()==True:
			if self.pipe_to_null==True:
				cmd=cmd+" >/dev/null "
				if background==True:
					cmd=cmd+" &"

		cmd=cmd+"\n"
		print(cmd)
		os.system(cmd)

	def server_base_process_jobs(self):
		path=True
		while(path!=False):
			#self.print_jobs()
			path,command=self.server_base_get_next_job_to_run(lock_file=True)
			if path!=False:
				#print(command)
				print(">>",path,">",command)
				self.exe_command(path,command)
				if len(self.jobs)>1:
					jobs_per_second="%.2f" % self.jobs_per_second
					self.progress_window.set_text(_("Running job ")+path+" jobs/s="+jobs_per_second)
					self.progress_window.set_fraction(float(self.jobs_run)/float(len(self.jobs)))
					if gui_get()==True:
						process_events()
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
				
		self.server_base_process_jobs()
		#if gui_get()==True:
		#	self.progress_window.show()
		#	self.progress_window.start()
		n=0
		while(1):
			ls=os.listdir(self.sim_dir)
			#print(self.sim_dir,ls)
			for i in range(0, len(ls)):
				if ls[i][:4]=="lock" and ls[i][-4:]==".dat":
					n=0
					lock_file=ls[i]
					os.remove(os.path.join(self.sim_dir,lock_file))
					job=int(lock_file[4:-4])
					self.base_job_finished(job)

			self.server_base_process_jobs()
			time.sleep(1)
			#self.print_jobs()
			self.server_base_check_for_crashed()
			#self.print_jobs()
			if self.jobs_run==len(self.jobs):
				self.remove_lock_files()
				break

			if self.time_out!=False:
				if n>30:
					print(str(n)+"/"+str(self.time_out))

				if n>self.time_out:
					self.remove_lock_files()
					break
			n=n+1

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

	def server_base_set_callback(self,callback):
		self.callback=callback

	def server_base_check_for_crashed(self):
		if self.max_job_time==None:
			return

		for j in self.jobs:
			if j.status==1:
				if (time.time()-j.start_time)>self.max_job_time:
					j.status=2
					print("crashed:"+j.path)

	def server_base_get_next_job_to_run(self,lock_file=False):
		#print(get_exe_command())
		if (len(self.jobs)==0):
			return False,False

		for i in range(0, len(self.jobs)):
			if (self.jobs_running<self.cpus):
				if self.jobs[i].status==0:
					self.jobs[i].status=1
					self.jobs[i].start_time=time.time()
					self.jobs[i].start=str(datetime.now())
					self.jobs[i].ip=socket.gethostbyname(socket.gethostname())
					#for r in range(0,len(self.jobs)):
					#	print(self.jobs[i],self.args[i])
					
					self.jobs_running=self.jobs_running+1

					if lock_file==True:
						command_lock=" --lockfile "+os.path.join(self.sim_dir,"lock"+str(i)+".dat")
					else:
						command_lock=" --lock "+"lock"+str(i)
					my_command=get_exe_command()
					
					#if running_on_linux()==False:
					#	my_command="\""+get_exe_command()+"\""

					full_command=my_command+command_lock+" "+self.jobs[i].args+" "+get_exe_args()
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


