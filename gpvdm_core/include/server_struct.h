// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright 2008-2022 Roderick C. I. MacKenzie https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
// 

/** @file server_struct.h
@brief a struct to hold server jobs in.
*/

#ifndef server_struct_h
#define server_struct_h

#define server_max 20
#define server_no_job 0
#define server_job_ready 1
#define server_job_running 2

#include <time.h>
#include <enabled_libs.h>
	#include <pthread.h>

#include <gpvdm_const.h>

#define JOB_NONE		0
#define JOB_WAIT		1
#define JOB_RUNNING		2
#define JOB_FINISHED	3



struct worker
{
	int working;
		pthread_t thread_han;
	int worker_n;
};

struct job
{
	char name[100];
	int status;
	int batch_id;
		void *(* fun)(void *);		//Function to call
	void *sim;
	void * data0;
	void * data1;
	void * data2;
	void * data3;
	int data_int0;
	int data_int1;
	int data_int2;
	int data_int3;
	int data_int4;
	int data_int5;
	int cpus;
	struct worker *w;
	void * next;
};

struct server_struct
{
	char command[server_max][PATH_MAX];		//these need removing as they take up too much memory
	char output[server_max][PATH_MAX];
	int state[server_max];
	char dbus_finish_signal[200];
	char lock_file[PATH_MAX];
	int jobs;
	int jobs_running;
	int fd;
	int wd;
	int on;
	int readconfig;
	int min_cpus;
	int steel;
	int max_run_time;
	time_t end_time;
	time_t start_time;
	int max_forks;
	int batch_id;
	//server2
	int max_threads;
	int worker_max;
	struct worker *workers;

	struct job *j;
	int jobs_max;
	int send_progress_to_gui;
};

#endif
