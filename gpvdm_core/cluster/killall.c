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
