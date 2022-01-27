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

/** @file server.c
	@brief Job management for fitting, run multiple fitting instances over multiple CPUs.
*/
#include <enabled_libs.h>
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
	server->batch_id=0;
	server->send_progress_to_gui=FALSE;
}

void job_init(struct simulation *sim,struct job *j)
{
	j->fun=NULL;
	j->status=JOB_NONE;
	j->sim=NULL;
	j->data0=NULL;
	j->data1=NULL;
	j->data2=NULL;
	j->batch_id=-1;
	j->cpus=1;
	strcpy(j->name,"none");
	j->next=NULL;

}

	pthread_mutex_t lock;

void thread_lock()
{
		pthread_mutex_lock(&lock);
}

void thread_unlock()
{
		pthread_mutex_unlock(&lock);
}

int server2_get_next_batch_id(struct simulation *sim,struct server_struct *server)
{
	int new_id;
	thread_lock();

	new_id=server->batch_id++;
	if (new_id>1e6)
	{
		new_id=0;
	}

	new_id=server->batch_id;

	thread_unlock();
	return new_id;
}

void job_free_data(struct simulation *sim,struct job *j)
{
	//printf("%p",j->data0);
	free_1d((void **)&(j->data0),sizeof(struct job));
	//printf("%p",j->data0);
	free_1d((void **)&(j->data1),sizeof(struct job));
	free_1d((void **)&(j->data2),sizeof(struct job));

	j->fun=NULL;
	j->status=JOB_NONE;
	j->sim=NULL;
	j->batch_id=-1;
	strcpy(j->name,"none");
}


void server2_free_finished_jobs(struct simulation *sim,struct server_struct *server, int batch_id)
{
	thread_lock();

	struct job *j=NULL;
	struct job *j_next=NULL;

	j=server->j;
	while(j!=NULL)
	{
		j_next=j->next;
		if (j->status==JOB_FINISHED)
		{	
			if (j->batch_id==batch_id)
			{
				job_free_data(sim,j);
			}
		}
		j=j_next;
	}

	thread_unlock();

}
void server2_malloc(struct simulation *sim,struct server_struct *server)
{

	int nw;
	int nj;
	struct worker *w;

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

	server->jobs_max=1;
	if (server->j!=NULL)
	{
		ewe(sim,"Job not null\n");
	}

	server->j=(struct job *)malloc(sizeof(struct job));
	job_init(sim,server->j);
	/*for (nj=0;nj<server->jobs_max;nj++)
	{
		j=&(server->j[nj]);
		
	}*/
}

void server2_job_list_free(struct simulation *sim,struct server_struct *server)
{
	thread_lock();

	struct job *j=NULL;
	struct job *j_next=NULL;

	j=server->j;
	while(j!=NULL)
	{
		j_next=j->next;
		job_free_data(sim,j);
		free(j);
		j=j_next;
	}

	server->j=NULL;

	thread_unlock();
}

void server2_free(struct simulation *sim,struct server_struct *server)
{
	int w;

	free_1d((void **)&(server->workers),sizeof(struct worker));
	server->worker_max = -1;

	server2_job_list_free(sim,server);
	server->jobs_max=-1;

}

void server2_add_job(struct simulation *sim,struct server_struct *server,struct job *j_in)
{
	int nj;
	struct job *j;
	struct job *j_next;

	thread_lock();

	if (j_in->batch_id==-1)
	{
		ewe(sim,"Batch ID can not be -1\n");
	}

	if (server->j==NULL)
	{
		ewe(sim,"server->j==NULL\n");
	}

	j=server->j;
	while(j!=NULL)
	{
		j_next=j->next;
		if (j->status==JOB_NONE)
		{
			j_in->next=j_next;		//repair list
			memcpy(j, j_in, sizeof(struct job));
			j->status=JOB_WAIT;
			thread_unlock();
			return;
		}
		
		if (j->next==NULL)
		{
			break;
		}
		j=j_next;
	}


	j->next=malloc(sizeof(struct job));
	j=j->next;
	//job_init(sim,j);

	memcpy(j, j_in, sizeof(struct job));
	j->status=JOB_WAIT;
	j->next=NULL;

	server->jobs_max++;
	thread_unlock();
}

void server2_dump_jobs(struct simulation *sim,struct server_struct *server)
{
	int nj=0;
	struct job *j;
	printf("jobs (%d):\n",server->jobs_max);
	printf("-------------------------\n");
	printf("index\tstatus\tname\t\tfunction\tbatch id\n");

	j=server->j;
	while(j!=NULL)
	{
		printf("%d\t%d\t%s\t\t%p\t%d\t%p\n",nj,j->status,j->name,j->fun,j->batch_id,j);
		j=j->next;
		nj++;
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

int server2_jobs_left(struct simulation *sim,struct server_struct *server, int batch_id)
{
	int left=0;
	int nj=0;
	struct job *j;

	j=server->j;
	while(j!=NULL)
	{
		if ((j->status==JOB_WAIT)||(j->status==JOB_RUNNING))
		{
			if (batch_id==-1)
			{
				left++;
			}else
			{
				if (batch_id==j->batch_id)
				{
					left++;
				}
			}
		}
		j=j->next;
	}


	return left;
}

int server2_count_all_jobs(struct simulation *sim,struct server_struct *server)
{
	int left=0;
	int nj=0;
	struct job *j;

	j=server->j;
	while(j!=NULL)
	{
		if ((j->status==JOB_WAIT)||(j->status==JOB_RUNNING)||(j->status==JOB_FINISHED))
		{
			left++;
		}
		j=j->next;
	}


	return left;
}

int server2_count_finished_jobs(struct simulation *sim,struct server_struct *server)
{
	int left=0;
	int nj=0;
	struct job *j;

	j=server->j;
	while(j!=NULL)
	{
		if (j->status==JOB_FINISHED)
		{
			left++;
		}
		j=j->next;
	}

	return left;
}

void server2_jobs_run(struct simulation *sim,struct server_struct *server, int batch_id)
{
	int nj=0;
	struct job *j;
	struct worker *w;

	j=server->j;
	while(j!=NULL)
	{
		if (j->batch_id==batch_id)
		{
			if (j->status==JOB_WAIT)
			{
				//if (strcmp(j->name,"light-1")==0)
				//{
				//	printf("WTF!!!\n");
				//	getchar();
				//}
				w=server2_get_next_worker(sim,server);

				if (w==NULL)
				{
					break;
				}

				j->status=JOB_RUNNING;
				j->w=w;
				w->working=TRUE;

				if (server->max_threads==0)
				{
					j->fun((void*) j);
				}else
				{
						pthread_create( &(w->thread_han), NULL, j->fun, (void*) j);

					//j->fun();
				}
			}

		}

		j=j->next;
	}


}

void server2_clean_workers(struct simulation *sim,struct server_struct *server)
{
	int nw;
	thread_lock();
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
	thread_unlock();

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

void server2_run_until_done(struct simulation *sim,struct server_struct *server,int batch_id)
{
	char send_data[1000];
	double jobs_tot;
	double jobs_finished;
	double old_jobs_finished=server2_count_finished_jobs(sim,server);
	while(1)
	{
		//printf("-\n");
		server2_clean_workers(sim,server);
		//printf("--\n");

		if (server2_jobs_left(sim,server,batch_id)==0)
		{
			break; 
		}

		//printf("---\n");

		server2_jobs_run(sim,server,batch_id);
		jobs_tot=server2_count_all_jobs(sim,server);
		jobs_finished=server2_count_finished_jobs(sim,server);
		//printf("----\n");
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
		//printf("-----\n");
		//server2_dump_jobs(sim,server);
		usleep(100);
		//getchar();
	}

	//printf("all jobs run\n");

}
