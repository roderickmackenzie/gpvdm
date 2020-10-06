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

/** @file server.c
	@brief Job management for fitting, run multiple fitting instances over multiple CPUs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dump.h>
#include <unistd.h>
#include <dos.h>
#include "util.h"

	#include <sys/inotify.h>
	#include <sys/time.h>
	#include <signal.h>
	#include <linux/limits.h>
	#define EVENT_SIZE  ( sizeof (struct inotify_event) )
	#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#include "sim.h"
#include "server.h"
#include "inp.h"
#include "timer.h"
#include "gui_hooks.h"
#include "lang.h"
#include "log.h"
#include <cal_path.h>
#include <gpvdm_const.h>
#include <server.h>
#include <memory.h>

static int unused __attribute__((unused));


void server2_init(struct simulation *sim,struct server_struct *server)
{
	server->worker_max=-1;
	server->workers=NULL;

	server->j=NULL;
	server->jobs_max=-1;
	server->max_forks=0;
}

void job_init(struct simulation *sim,struct job *j)
{
	j->fun=NULL;
	j->status=JOB_NONE;
	j->sim=NULL;
	j->data0=NULL;
	j->data1=NULL;
	j->data2=NULL;
	strcpy(j->name,"none");

}

void job_free(struct simulation *sim,struct job *j)
{
	//printf("%p",j->data0);
	free_1d((void **)&(j->data0),sizeof(struct job));
	//printf("%p",j->data0);
	free_1d((void **)&(j->data1),sizeof(struct job));
	free_1d((void **)&(j->data2),sizeof(struct job));

	j->fun=NULL;
	j->status=JOB_NONE;
	j->sim=NULL;
	strcpy(j->name,"none");
}


void server2_free_finished_jobs(struct simulation *sim,struct server_struct *server)
{
	int nj;
	struct job *j;
	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		if (j->status==JOB_FINISHED)
		{	
			job_free(sim,j);
		}
	}
}
void server2_malloc(struct simulation *sim,struct server_struct *server)
{
	int nw;
	int nj;
	struct worker *w;
	struct job *j;

		server->worker_max=sysconf(_SC_NPROCESSORS_CONF);

//	textcolor(sim,fg_green);
//	printf_log(sim,"%s %d\n",_("Computer cores:"),server->max_threads);
//	textcolor(sim,fg_reset);

	if (server->max_threads>0)
	{
		if (server->max_threads<server->worker_max)
		{
			server->worker_max=server->max_threads;
		}
	}

	textcolor(sim,fg_green);
	printf_log(sim,"%s %d\n",_("Max threads:"),server->worker_max);
	textcolor(sim,fg_reset);

	malloc_1d((void **)&(server->workers),server->worker_max,sizeof(struct worker));

	for (nw=0;nw<server->worker_max;nw++)
	{
		w=&(server->workers[nw]);
		w->working=FALSE;
		w->worker_n=nw;
		w->thread_han=THREAD_NULL;
	}

	//server2_dump_workers(sim,server);

	server->jobs_max=10;
	malloc_1d((void **)&(server->j),server->jobs_max,sizeof(struct job));

	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		job_init(sim,j);
	}

}

void server2_free(struct simulation *sim,struct server_struct *server)
{
	int w;


	//for (w=0;w<server->worker_max;w++)
	//{
	//}

	free_1d((void **)&(server->workers),sizeof(struct worker));
	server->worker_max = -1;

	free_1d((void **)&(server->j),sizeof(struct job));
	server->jobs_max=-1;

}

void server2_add_job(struct simulation *sim,struct server_struct *server,struct job *j_in)
{
	int nj;
	struct job *j;
	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		if (j->status==JOB_NONE)
		{
			memcpy(j, j_in, sizeof(struct job));
			j->status=JOB_WAIT;
			return;
		}

	}

	int new_jobs_max=server->jobs_max+10;
	server->j=realloc(server->j,new_jobs_max*sizeof(struct job));

	for (nj=server->jobs_max;nj<new_jobs_max;nj++)
	{
		j=&(server->j[nj]);
		job_init(sim,j);
	}

	j=&(server->j[server->jobs_max]);
	memcpy(j, j_in, sizeof(struct job));
	j->status=JOB_WAIT;

	server->jobs_max=new_jobs_max;
}

void server2_dump_jobs(struct simulation *sim,struct server_struct *server)
{
	int nj=0;
	struct job *j;
	printf("jobs:\n");
	printf("-------------------------\n");
	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		printf("%d %d %s %p\n",nj,j->status,j->name,j->fun);
	}
}

void server2_dump_workers(struct simulation *sim,struct server_struct *server)
{
	int nw=0;
	struct worker *w;
	printf("workers:\n");
	printf("-------------------------\n");
	for (nw=0;nw<server->worker_max;nw++)
	{
		w=&(server->workers[nw]);
		printf("%d %d %ld\n",nw,w->working,w->thread_han);
	}

}

void server2_job_finished(struct simulation *sim,struct job *j)
{
	j->status=JOB_FINISHED;
	j->w->working=FALSE;
}

int server2_jobs_left(struct simulation *sim,struct server_struct *server)
{
	int left=0;
	int nj=0;
	struct job *j;
	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		if ((j->status==JOB_WAIT)||(j->status==JOB_RUNNING))
		{
			left++;
		}
	}

	return left;
}

int server2_count_all_jobs(struct simulation *sim,struct server_struct *server)
{
	int left=0;
	int nj=0;
	struct job *j;
	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		if ((j->status==JOB_WAIT)||(j->status==JOB_RUNNING)||(j->status==JOB_FINISHED))
		{
			left++;
		}
	}

	return left;
}

int server2_count_finished_jobs(struct simulation *sim,struct server_struct *server)
{
	int left=0;
	int nj=0;
	struct job *j;
	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		if (j->status==JOB_FINISHED)
		{
			left++;
		}
	}

	return left;
}

void server2_jobs_run(struct simulation *sim,struct server_struct *server)
{
	int nj=0;
	struct job *j;
	struct worker *w;

	for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		if (j->status==JOB_WAIT)
		{
			w=server2_get_next_worker(sim,server);
			if (w==NULL)
			{
				break;
			}
			j->status=JOB_RUNNING;
			j->w=w;
			w->working=TRUE;

				pthread_create( &(w->thread_han), NULL, j->fun, (void*) j);

			//j->fun();
		}
	}
}

void server2_clean_workers(struct simulation *sim,struct server_struct *server)
{
	int nw;
	struct worker *workers = server->workers;

	for (nw=0;nw<server->worker_max;nw++)
	{

		if ((workers[nw].thread_han!=THREAD_NULL)&&(workers[nw].working==FALSE))
		{
			//printf("wait %d\n",ww);
				pthread_join( workers[nw].thread_han, NULL);

			workers[nw].thread_han=THREAD_NULL;
			//printf("exit here1\n");

		}

	}


}

struct worker *server2_get_next_worker(struct simulation *sim,struct server_struct *server)
{
	int nw;
	struct worker *workers = server->workers;

	for (nw=0;nw<server->worker_max;nw++)
	{
		if (workers[nw].thread_han==THREAD_NULL)
		{
			//printf("exit here\n");
			return &(workers[nw]);
		}

	}

	return NULL;
}

void server2_run_until_done(struct simulation *sim,struct server_struct *server)
{
	char send_data[1000];
	double jobs_tot;
	double jobs_finished;
	double old_jobs_finished;
	while(1)
	{
		server2_clean_workers(sim,server);

		if (server2_jobs_left(sim,server)==0)
		{
			break;
		}

		server2_jobs_run(sim,server);
		jobs_tot=server2_count_all_jobs(sim,server);
		jobs_finished=server2_count_finished_jobs(sim,server);

		if (jobs_finished!=old_jobs_finished)
		{
			if (server->send_progress_to_gui==TRUE)
			{


				sprintf(send_data,"percent:%.2lf",jobs_finished/jobs_tot);
				printf_log(sim,"%s\n",send_data);
				gui_send_data(sim,gui_sub,send_data);
			}	
		}
		old_jobs_finished=jobs_finished;

		usleep(1000);
	}

	//printf("all jobs run\n");

}
