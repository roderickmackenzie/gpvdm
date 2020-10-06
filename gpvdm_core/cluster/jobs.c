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

/** @file jobs.c
@brief deal with jobs
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "inp.h"
#include "util.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include "tx_packet.h"
#include <time.h>

struct job jobs[1000];
static int njobs=0;



int cmp_addjob(int sock,struct tx_struct *data)
{
char job_name[400];
char full_path[400];

	if (cmpstr_min(data->id,"gpvdmaddjob")==0)
	{
		sprintf(job_name,"job%d",njobs);
		jobs_add(job_name,data->target,data->cpus);

		printf("%s\n",calpath_get_store_path());
		printf("%s\n",job_name);

		join_path(2,full_path,calpath_get_store_path(),job_name);

		if (strlen(full_path)>4)		// / is not allowed!
		{
			remove_dir(full_path);
		}

		jobs_print();
		tx_thing_done(sock,data);
		return 0;
	}

return -1;
}

int tx_job_list()
{
	struct node_struct* master=NULL;
	master=node_find_master();

	struct tx_struct packet;

	char buf[10000];
	strcpy(buf,"");

	if (master!=NULL)
	{
		tx_struct_init(&packet);
		tx_set_id(&packet,"gpvdm_job_list");

		gen_job_list(buf);

		tx_set_size(&packet,strlen(buf));
		tx_packet(master->sock,&packet,buf);
	}
return 0;
}

int cmp_send_job_list(int sock,struct tx_struct *data)
{

	if (cmpstr_min(data->id,"gpvdm_send_job_list")==0)
	{
		tx_job_list();

		return 0;
	}

return -1;
}


void jobs_reset()
{
njobs=0;
}

struct job* jobs_get_next()
{
int i;

	for (i=0;i<njobs;i++)
	{
		if (jobs[i].status==0)
		{
			return &(jobs[i]);
		}
	}
return NULL;
}

void jobs_add(char *name,char *target,int cpus)
{
	strcpy(jobs[njobs].name,name);
	strcpy(jobs[njobs].target,target);
	strcpy(jobs[njobs].ip,"-");
	jobs[njobs].done=FALSE;
	jobs[njobs].status=0;
	jobs[njobs].copy_state=0;
	jobs[njobs].cpus_needed=cpus;
	jobs[njobs].t_start=0;
	jobs[njobs].t_stop=0;

	njobs++;
}

struct job* get_jobs_array()
{
	return jobs;
}

int get_njobs()
{
	return njobs;
}

void set_njobs(int n)
{
	njobs=n;
}

void stop_all_jobs()
{
int i=0;

	for (i=0;i<njobs;i++)
	{
		if (jobs[i].status==0)
		{
			jobs[i].status=3;
		}
	}

return;
}

double jobs_cal_percent_finished()
{
int i=0;
int finished=0;

	for (i=0;i<njobs;i++)
	{
		if (jobs[i].status==2)
		{
			finished++;
		}
	}
return 100.0*((double)finished)/((double)njobs);
}

int cal_abs_path_from_target(char *full_path,char *target,char *file_name)
{
char dest[200];
struct job* job=NULL;

	job=jobs_find_job(target);
	if (job!=NULL)
	{
		strcpy(dest,job->name);
	}else
	if (strcmp(target,"")!=0)
	{
		strcpy(dest,target);
	}else
	{
		printf("job for target '%s' not found\n",file_name);
		return -1;
	}

	join_path(3,full_path,calpath_get_store_path(),dest,file_name);

return 0;
}

struct job* jobs_find_job(char *target)
{
int i=0;
if (strcmp(target,"")==0)
{
	return NULL;
}

	for (i=njobs-1;i>=0;i--)
	{
		if (strcmp(jobs[i].target,target)==0)
		{
			return &(jobs[i]);
		}
	}

	for (i=njobs-1;i>=0;i--)
	{
		if (strcmp(jobs[i].name,target)==0)
		{
			return &(jobs[i]);
		}
	}
return NULL;
}

int jobs_remaining()
{
int i=0;
int left=0;

	for (i=0;i<njobs;i++)
	{
		//printf("%d %d %d\n",i,jobs[i].status,njobs);
		if (jobs[i].status==2)
		{
			left++;
		}
	}
return njobs-left;
}

void jobs_clear_all()
{
njobs=0;
}
