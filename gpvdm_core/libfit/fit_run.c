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

/** @file fit_run.c
@brief run the fit to expeimental data
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <i.h>
#include <util.h>
#include <unistd.h>
#include <server.h>
#include <sim.h>
#include <dump.h>
#include "fit.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <log.h>
#include <gui_hooks.h>
#include <cal_path.h>
#include <dos.h>
#include <lock.h>
#include <memory.h>
#include <device_fun.h>

static int unused __attribute__((unused));

//#define enable_server2

#ifdef enable_server2
	THREAD_FUNCTION fit_sim_thread(void * in)
	{

		struct job *j=(struct job *)in;
		struct simulation *sim=(struct simulation *)j->sim;
		struct device *dev=(struct device *)j->data0;
		//server2_dump_jobs(sim,&(sim->server));
		//printf(">>>>>>>>>JOB FINISHED %p\n",j);
		//exit(0);
		device_run_simulation(sim,dev);

		//printf(">>>>>>>>>JOB FINISHED %d\n",JOB_FINISHED);
		server2_job_finished(sim,j);
		//printf(">>>>>>>>>JOB FINISHED %p\n",j);
		//server2_dump_jobs(sim,&(sim->server));
		return 0;
	}
#endif
double fit_run_sims(struct simulation *sim,struct fitvars *fitconfig)
{
	lock_feature_enabled(sim,"fitting");
//getchar();
int i;
double error=0.0;

#ifdef enable_server2
	int batch_id;
	struct job j;
	struct device *dev;
#endif
int static error_count=0;

printf_log(sim,"fit_run_sims\n");

char input_path[PATH_MAX];

for (i=0;i<fitconfig->data_sets;i++)
{
	if (fitconfig->data_set[i].enabled==1)
	{
		strcpy(input_path,fitconfig->data_set[i].fit_path);
		//printf("%s\n",sim->input_path);
		gen_dos_fd_gaus_fd_stand_alone(sim,input_path);
		break;
	}
}

//getchar();
printf_log(sim,"Submitting job %d - %s\n",0,fitconfig->data_set[0].fit_name);
#ifdef enable_server2
	batch_id=server2_get_next_batch_id(sim,&(sim->server));
#endif

for (i=0;i<fitconfig->data_sets;i++)
{
	if (fitconfig->data_set[i].enabled==1)
	{
		//sprintf(patch_file_path,"fit_patch%d.inp",i);
		//patch(sim,fitconfig->data_set[i].fit_path,patch_file_path);
		//printf("%d %s\n",fitconfig->data_set[i].run_this_simulation,fitconfig->data_set[i].fit_name);
		//getchar();
		if (fitconfig->data_set[i].run_this_simulation==TRUE)
		{
			#ifndef enable_server2
				server_add_job(sim,fitconfig->data_set[i].fit_path,fitconfig->data_set[i].fit_path);
			#else
				job_init(sim,&j);
				strcpy(j.name,fitconfig->data_set[i].fit_name);
				j.fun=&fit_sim_thread;
				j.sim=(void *)sim;
				j.batch_id=batch_id;
				printf("%d'\n",batch_id);
				malloc_1d((void **)&(j.data0),1,sizeof(struct device));
				dev=(struct device *)(j.data0);
				device_init(sim,dev);
				strcpy(dev->input_path,fitconfig->data_set[i].fit_path);
				strcpy(dev->output_path,fitconfig->data_set[i].fit_path);
				server2_add_job(sim,&(sim->server),&j);

			#endif
		}
	}
}

#ifndef enable_server2
	print_jobs(sim);
#else
	server2_dump_jobs(sim,&(sim->server));
#endif
error=0.0;
#ifdef enable_server2
	server2_run_until_done(sim,&(sim->server),batch_id);
//printf("here\n");
//getchar();
	server2_free_finished_jobs(sim,&(sim->server),batch_id);
#else
	if (server_run_jobs(sim,&(sim->server))!=0)
	{
		error+=100.0;
		printf_log(sim,"Error in newton solver detected by fit_run.c\n");
		printf_log(sim,"error_count=%d\n",error_count);
		usleep(0.1e6);
		error_count++;
		if (error_count>100)
		{
			ewe(sim,"Too many erros in a row - quitting\n");
		}
	}else
#endif
{
error_count=0;
error+=get_all_error(sim,fitconfig);
}

change_cpus(sim,&(sim->server));


printf_log(sim,"Jobs/s=%le ODEs/s=%le\n",server_get_jobs_per_s(),server_get_odes_per_s());
//printf_log(sim,"rod exit\n");

gui_send_data(sim,gui_main,"fit_run");
return error;
}

