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

/** @file rx_loop.c
@brief main rx loop to listen for packets
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


void *rx_loop(void *s)
{
struct state *sim=get_sim();

int ret=0;
int processed=FALSE;
struct tx_struct data;
struct tx_struct last_data;
int sock=*((int*)s);
char ip[100];
get_ip_from_sock(ip,sock);
printf("rx loop for ip=%s\n",ip);
int done=0;
	while(1)
	{
		processed=FALSE;
		ret=rx_packet(sock,&data);

		if (ret<0)
		{
			get_ip_from_sock(ip,sock);
			node_delete(ip);
			printf("ip=%s\n",ip);
			printf("Breaking due to -1\n");
			break;
		}


		if (cmp_rxfile(sock,&data,sim)==0)
		{
			debug_printf("a\n");
			processed=TRUE;
		}else
		if (cmp_addjob(sock,&data)==0)
		{
			debug_printf("b\n");
			processed=TRUE;
		}else
		if (cmp_addnode(sock,&data)==0)
		{
			debug_printf("c\n");
			processed=TRUE;
		}else
		if (cmp_deletenode(sock,&data)==0)
		{
			debug_printf("d\n");
			processed=TRUE;
		}else
		if (cmp_runjobs(sim,sock,&data)==0)
		{
			debug_printf("e\n");
			processed=TRUE;
		}else
		if (cmp_simfinished(sim,sock,&data)==0)
		{
			debug_printf("f\n");
			processed=TRUE;
		}else
		if (cmp_register_master(sock,&data)==0)
		{
			debug_printf("g\n");
			processed=TRUE;
		}else
		if (cmp_head_killall(sock,&data)==0)
		{
			debug_printf("h\n");
			processed=TRUE;
		}else
		if (cmp_head_sleep(sock,&data)==0)
		{
			debug_printf("i\n");
			processed=TRUE;
		}else
		if (cmp_head_poweroff(sock,&data)==0)
		{
			debug_printf("j\n");
			processed=TRUE;
		}else
		if (cmp_sendnodelist(sock,&data)==0)
		{
			debug_printf("k\n");
			processed=TRUE;
		}else
		if (cmp_get_data(sim,sock,&data)==0)
		{
			debug_printf("l\n");
			processed=TRUE;
		}else
		if (cmp_head_exe(sim,sock,&data)==0)
		{
			debug_printf("m\n");
			processed=TRUE;
		}else
		if (cmp_master_clean(sock,&data)==0)
		{
			debug_printf("n\n");
			processed=TRUE;
		}else
		if (cmp_rxloadstats(sock,&data)==0)
		{
		/*	debug_printf("o\n");
			if (done==0)
			{
			send_command(sock,"gpvdm_core","",4);
			done=1;
			}*/
			processed=TRUE;
		}else
		if (cmp_head_quit(sock,&data)==0)
		{
			debug_printf("p\n");
			processed=TRUE;
		}else
		if (cmp_rxsetmaxloads(sock,&data)==0)
		{
			debug_printf("1\n");
			processed=TRUE;
		}else
		if (cmp_sync_packet_one(sock,&data)==0)
		{
			debug_printf("2\n");
			processed=TRUE;
		}else
		if (cmp_sync_packet_two(sim,sock,&data)==0)
		{
			debug_printf("3\n");
			processed=TRUE;
		}else
		if (cmp_send_job_list(sock,&data)==0)
		{
			debug_printf("4\n");
			processed=TRUE;
		}else
		if (cmp_head_stop_all_jobs(sock,&data)==0)
		{
			debug_printf("5\n");
			processed=TRUE;
		}else
		if (cmp_delete_all_jobs(sock,&data)==0)
		{
			debug_printf("6\n");
			processed=TRUE;
		}else
		if (cmp_job_pid(sim,sock,&data)==0)
		{
			debug_printf("r\n");
			processed=TRUE;
		}

		if (processed==FALSE)
		{
			if (sim->state==HEAD)
			{
				char temp[500];
				sprintf(temp,"command not understood last:%s this:%s", last_data.id, data.id);
				send_message(temp);
			}
		}

		jobs_save();

		copy_packet(&last_data,&data);


	}



close(sock);
printf("leavng main rx loop for %s\n",ip);
return NULL;
}
