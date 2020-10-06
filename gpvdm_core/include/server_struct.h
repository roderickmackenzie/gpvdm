// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/** @file server_struct.h
@brief a struct to hold server jobs in.
*/

#ifndef server_struct_h
#define server_struct_h

#define server_max 100
#define server_no_job 0
#define server_job_ready 1
#define server_job_running 2

#include <time.h>

	#include <pthread.h>


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
		void *(* fun)(void *);		//Function to call
	void *sim;
	void * data0;
	void * data1;
	void * data2;
	int data_int0;
	struct worker *w;
};

struct server_struct
{
	char command[server_max][200];
	char output[server_max][200];
	int state[server_max];
	char dbus_finish_signal[200];
	char lock_file[200];
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

	//server2
	int max_threads;
	int worker_max;
	struct worker *workers;

	struct job *j;
	int jobs_max;
	int send_progress_to_gui;
};

#endif
