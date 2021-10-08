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

/** @file jobs_save_load.c
@brief load and save jobs to disk incase of a crash
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

void get_full_time(char *out,time_t t)
{
	struct tm tm = *localtime(&t);
	if (t!=0)
	{
		sprintf(out,"%d/%d/%d-%d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	}else
	{
		strcpy(out,"-");
	}
}

int jobs_load()
{
	FILE * fp;

	struct job *my_jobs=get_jobs_array();

	char path[200];
	join_path(2,path,calpath_get_store_path(), "saved_jobs3.dat");
	fp = fopen(path, "rb");
	if (fp == NULL)
	{
		printf("no last data file %s\n",path);
		return -1;
	}

	int items=0;
	fread(&items, sizeof(int), 1, fp);

	int i=0;
	for (i=0;i<items;i++)
	{
		fread(&(my_jobs[i]), sizeof(struct job), 1, fp);
		my_jobs[i].status=2;
	}

	fclose(fp);

set_njobs(items);
jobs_print();
return 0;
}


void gen_job_list(char *buf)
{
int i;
struct job* jobs=get_jobs_array();
int njobs=get_njobs();
char t_start[100];
char t_stop[100];
char t_force_stop[100];
char temp[1000];

	sprintf(buf,"n\tname\tdone\tstatus\ttarget\t\t\t\t\tip\tcopystate\tstart\tforce_stop\tstop\tcpus\tpid\n");

	for (i=0;i<njobs;i++)
	{
		get_full_time(t_start,jobs[i].t_start);
		//printf("%s\n",t_start);

		get_full_time(t_stop,jobs[i].t_stop);
		//printf("%s\n",t_stop);

		get_full_time(t_force_stop,jobs[i].t_force_stop);
			sprintf(temp,"%d\t%s\t%d\t%d\t%s\t%s\t%d\t%s\t%s\t%s\t%d\t%d\n",i,jobs[i].name,jobs[i].done,jobs[i].status,jobs[i].target,jobs[i].ip,jobs[i].copy_state,t_start,t_force_stop,t_stop,jobs[i].cpus_needed,jobs[i].pid);
		strcat(buf,temp);
		//printf("%s\n",temp);
	}
}

void jobs_print()
{
char buf[10000];
gen_job_list(buf);

printf("%s\n",buf);
}

void jobs_save()
{
FILE *out=NULL;
char path[200];
join_path(2,path,calpath_get_store_path(), "saved_jobs3.dat");
out=fopen(path,"wb");

int i;
struct job* jobs=get_jobs_array();

int njobs=get_njobs();
fwrite(&njobs, sizeof(int), 1, out);
	for (i=0;i<njobs;i++)
	{
		fwrite(&(jobs[i]), sizeof(struct job), 1, out);
	}


fclose(out);
}
