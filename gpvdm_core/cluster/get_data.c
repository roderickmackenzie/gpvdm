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

/** @file get_data.c
@brief Get data from the cluster
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
#include <state.h>

int cmp_node_send_data(struct state *sim,int sock,struct tx_struct *data)
{
	char full_path[200];
	struct tx_struct packet;

	if (cmpstr_min(data->id,"gpvdmnodesenddata")==0)
	{


		join_path(2,full_path,calpath_get_store_path(), data->job);

		printf("sending dir %s\n",full_path);
		send_dir(sim,sock,full_path, 0,full_path,data->job);

		tx_struct_init(&packet);
		tx_set_id(&packet,"gpvdmgetdata");
		tx_packet(sock,&packet,NULL);

		return 0;
	}

return -1;
}


int cmp_get_data(struct state *sim,int sock,struct tx_struct *data)
{
	int i=0;
	struct job* jobs=NULL;
	int njobs=0;
	char full_path[512];
	int state_changed=FALSE;
	njobs=get_njobs();
	jobs=get_jobs_array();
	if (cmpstr_min(data->id,"gpvdmgetdata")==0)
	{
		printf("check getdata gpvdmgetdata\n");




		for (i=0;i<njobs;i++)
		{
			if ((jobs[i].copy_state==0)&&(jobs[i].status>0))
			{

				struct node_struct* job_node=NULL;
				job_node=node_find(jobs[i].ip);
				printf(">>>>>%s %d %s %s\n",jobs[i].ip,i,job_node->ip,jobs[i].name);
				if (job_node!=NULL)
				{
					struct tx_struct packet;
					tx_struct_init(&packet);
					tx_set_id(&packet,"gpvdmnodesenddata");
					strcpy(packet.job,jobs[i].name);

					tx_packet(job_node->sock,&packet,NULL);

					state_changed=TRUE;
					jobs[i].copy_state=1;
					break;
				}
			}
		}

		if (state_changed==FALSE)
		{
			for (i=0;i<njobs;i++)
			{
				jobs[i].copy_state=0;
			}
			jobs_print();
			printf("Got all data from nodes\n");


			for (i=0;i<njobs;i++)
			{
				printf("%s %d %d\n",calpath_get_store_path(),i,njobs);
				printf("%s\n",jobs[i].name);
				join_path(2,full_path,calpath_get_store_path(), jobs[i].name);
				printf("tx to %s %s\n",full_path,jobs[i].target);
				struct node_struct* master=NULL;
				master=node_find_master();
				send_dir(sim,master->sock,full_path, 0,full_path,jobs[i].target);

			}
		}

		return 0;
	}

return -1;
}

