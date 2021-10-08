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

/** @file killall.c
@brief kill all the jobs
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
#include "inp.h"
#include "tx_packet.h"
#include <pthread.h>

int cmp_node_killjob(int sock,struct tx_struct *data)
{
int     status;

	if (cmpstr_min(data->id,"kill_job")==0)
	{
		int pid=(int)data->pid;
		kill_all(pid);
		wait(&status);
		return 0;
	}

return -1;
}

//This should be removed
int cmp_node_killall(int sock,struct tx_struct *data)
{
	if (cmpstr_min(data->id,"gpvdmnodekillall")==0)
	{
		printf("killall gpvdm_core\n");
		system("killall gpvdm_core");
		return 0;
	}

return -1;
}

int cmp_head_killall(int sock,struct tx_struct *data)
{
	int i=0;
	struct job *jobs=NULL;
	struct tx_struct packet;
	int njobs=0;
	if (cmpstr_min(data->id,"gpvdmkillall")==0)
	{
		jobs=get_jobs_array();
		njobs=get_njobs();
		for (i=0;i<njobs;i++)
		{
			if (jobs[i].pid!=-1)
			{
				tx_struct_init(&packet);
				tx_set_id(&packet,"kill_job");
				packet.pid=jobs[i].pid;
				send_packet_to_node(jobs[i].ip,&packet);
			}
		}


		return 0;
	}

return -1;
}

int cmp_head_stop_all_jobs(int sock,struct tx_struct *data)
{
	if (cmpstr_min(data->id,"gpvdm_stop_all_jobs")==0)
	{
		stop_all_jobs();
		return 0;
	}

return -1;
}

int cmp_delete_all_jobs(int sock,struct tx_struct *data)
{
	if (cmpstr_min(data->id,"gpvdm_delete_all_jobs")==0)
	{
		jobs_clear_all();
		return 0;
	}

return -1;
}

int cmp_head_stop_running_jobs(int sock,struct tx_struct *data)
{
	if (cmpstr_min(data->id,"gpvdm_stop_all_jobs")==0)
	{
		stop_all_jobs();
		return 0;
	}

return -1;
}
