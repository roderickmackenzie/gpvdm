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

/** @file load.c
@brief deal with cluster load
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
#include <time.h>
#include "tx_packet.h"

struct node_struct nodes[100];

int send_node_load(int sock)
{
	int ret=0;
	double loadavg[3];
	getloadavg(loadavg, 3);

	struct tx_struct packet;
	tx_struct_init(&packet);
	tx_set_id(&packet,"gpvdmload");
	tx_set_size(&packet,0);
	packet.load0=loadavg[0];
	packet.load1=loadavg[1];
	packet.load2=loadavg[2];
	strcpy(packet.ip,get_my_ip());

	ret=tx_packet(sock,&packet,NULL);

return ret;
}

int set_master_load()
{
	double loadavg[3];
	getloadavg(loadavg, 3);

	struct node_struct* master=NULL;
	master=node_find_master();

	if (master!=NULL)
	{
		master->load0=loadavg[0];
	}
return 0;
}

int cmp_nodeload(int sock,struct tx_struct *data)
{

	int ret=0;
	if (cmpstr_min(data->id,"gpvdmnodegetload")==0)
	{
		ret=send_node_load(sock);
		return ret;
	}
return -1;
}

int cmp_rxloadstats(int sock,struct tx_struct *data)
{
	struct node_struct* node;
	if (cmpstr_min(data->id,"gpvdmload")==0)
	{

		node=node_find(data->ip);
		if (node!=NULL)
		{
			node->load0=data->load0;
			node->alive=time(NULL);
		}
		return 0;
	}

return -1;
}

int cmp_rxsetmaxloads(int sock,struct tx_struct *data)
{
char *node_ip=NULL;
char node_buf[100];
char *max_load=NULL;
char max_buf[100];
int cpus=0.0;

	if (cmpstr_min(data->id,"gpvdm_set_max_loads")==0)
	{

		printf("rod %s\n",data->data);
		struct inp_file in;
		inp_init(&in);
		in.data=data->data;
		in.fsize=data->size;
		inp_reset_read(&in);

		max_load  = (char*)-1;

		while(max_load!=NULL)
		{
			node_ip  = inp_get_string(&in);
			if (node_ip==NULL)
			{
				break;
			}
			strcpy(node_buf,node_ip);

			max_load  = inp_get_string(&in);
			if (max_load==NULL)
			{
				break;
			}
			strcpy(max_buf,max_load);

			printf("node ip %s\n",node_buf);
			printf("max_load %s\n",max_buf);
			struct node_struct* node=NULL;
			node=node_find(node_buf);

			if (node==NULL)
			{
				printf("I can't find IP %s\n",node_buf);
			}

			sscanf(max_buf,"%d",&cpus);
			printf("max found as%d %s\n",cpus,node->host_name);
			node->max_cpus=cpus;

			nodes_print();
		}

		nodes_print();

		return 0;
	}

return -1;
}


