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

/** @file node.c
@brief main loop for the node
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>

#include "util.h"
#include "inp.h"
#include <state.h>

struct state *local_sim;

void* alarm_thread(void *in)
{
	while(1)
	{
		if (local_sim->head_sock>=0)
		{
			send_node_load(local_sim->head_sock);
		}
		sleep(1);
	}
	return NULL;
}

/*void alarm_wakeup (int i)
{
		struct itimerval tout_val;

		signal(SIGALRM,alarm_wakeup);

		tout_val.it_interval.tv_sec = 0;
		tout_val.it_interval.tv_usec = 0;
		tout_val.it_value.tv_sec = 2;
		tout_val.it_value.tv_usec = 0;
		if (local_sim->head_sock>=0)
		{
			send_node_load(local_sim->head_sock);
		}

		setitimer(ITIMER_REAL, &tout_val,0);

}*/

void my_handler(int s)
{
	printf("Caught signal %d\n",s);
	send_delete_node(local_sim->head_sock);
	printf("quitting\n");
	exit(1);
}

int login(char * master_ip, int port)
{


    int sockfd; // Socket file descriptor
    //struct sockaddr_in addr_remote;
    struct sockaddr_in remote_addr;
    /* Get the Socket file descriptor */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
		printf(" %s\n", strerror(errno));
        return (0);
    }
    /* Fill the socket address struct */
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
    inet_pton(AF_INET, master_ip, &remote_addr.sin_addr);
    bzero(&(remote_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        return -1;
    }
    else
	{
		printf("client: Connected to server at port:%d sock:%d...ok!\n", port,sockfd);
	}
return sockfd;
}

int node(struct state *sim)
{

	local_sim=sim;
	int port=0;
	char temp_path[200];
	char interface[200];
	char master_ip[200];
	char store_path[200];
	struct sigaction sigIntHandler;

	//struct itimerval tout_val;

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	struct inp_file inp;
	inp_init(&inp);
	if (inp_load(&inp,"node.inp")!=0)
	{
		printf("can't find file config.inp");
		exit(0);
	}
	inp_check(&inp,1.0);
	inp_search_string(&inp,temp_path,"#store_path");
	inp_search_string(&inp,interface,"#interface");
	inp_search_string(&inp,master_ip,"#master_ip");

	if (strcmp(master_ip,"localhost")==0)
	{
		strcpy(master_ip,"127.0.0.1");
	}

	inp_search_int(&inp,&port,"#port");
	inp_free(&inp);

	realpath(temp_path, store_path);
	calpath_set_store_path(store_path);
	set_porgress_max(20);
	pthread_create( &sim->alarm, NULL,alarm_thread,NULL);

	do
	{
		sim->head_sock=-1;
		double p=0.0;
		do
		{
			sim->head_sock=login(master_ip,port);

			if (sim->head_sock>=0)
			{
				break;
			}
			printf("a\n");
			sleep(1);
			text_progress(p);
			printf("a\n");
			fputs("\r", stdout);
			p=p+0.1;
			if (p>1.0)
			{
				p=0;
			}
		}while(sim->head_sock<0);

		cal_my_ip(sim->head_sock);

		register_node(sim->head_sock);

		send_node_load(sim->head_sock);

		struct tx_struct data;
		int ret=0;
		int processed=FALSE;
		while(1)
		{
			ret=rx_packet(sim->head_sock,&data);

			processed=FALSE;

			if (ret==-1)
			{
				break;
			}

			if (cmp_node_runjob(sim,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_rxfile(sim->head_sock,&data,sim)==0)
			{
				processed=TRUE;
			}else
			if (cmp_node_killall(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_node_sleep(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_node_poweroff(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_node_send_data(sim,sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_nodeload(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_node_quit(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_slave_clean(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_sync_packet_one(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_sync_packet_two(sim,sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}else
			if (cmp_node_killjob(sim->head_sock,&data)==0)
			{
				processed=TRUE;
			}

			if (processed==FALSE)
			{
				printf("Command not understood!!!!!!!!!!!!!!!!! %s\n",data.id);
			}

		}

		close (sim->head_sock);
		printf("client: connection lost trying again.\n");

	}while(1);

    return (0);
}
