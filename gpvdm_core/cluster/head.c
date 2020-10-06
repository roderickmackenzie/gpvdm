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

/** @file head.c
@brief main loop for the head node
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
#include <pthread.h>
#include <state.h>
#include <sys/wait.h>
#include <sys/time.h>

void head_ctrl_c(int s)
{
	close_all_open();
	printf("Caught signal %d\n",s);
	exit(1);
}

void* head_alarm_wakeup(void *in)
{
	while(1)
	{
		set_master_load();
		nodes_txnodelist();
		tx_job_list();
		nodes_print();
		jobs_print();
		sleep(1);
	}
	return NULL;
}

int head (struct state *sim)
{

int port=0;
char file_store_path[200];
char interface[400]; //full path
struct inp_file inp;
inp_init(&inp);
if (inp_load(&inp,"head.inp")!=0)
{
	printf("can't find file config.inp");
	exit(0);
}
inp_check(&inp,1.0);
inp_search_int(&inp,&port,"#port");
inp_search_string(&inp,file_store_path,"#file_store");
inp_search_string(&inp,interface,"#interface");
inp_free(&inp);

calpath_set_store_path(file_store_path);
    int sockfd; // Socket file descriptor
    int nsockfd; // New Socket file descriptor
    unsigned int sin_size; // to store struct size
    struct sockaddr_in addr_local;
	struct sockaddr_in addr_remote;
    /* Get the Socket file descriptor */
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf ("ERROR: Failed to obtain Socket Descriptor.\n");
        return (0);
    }
    else
	{
		printf ("server: obtain socket descriptor successfully.\n");
	}
    /* Fill the local socket address struct */


    int one = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one));

    addr_local.sin_family = AF_INET; // Protocol Family
    addr_local.sin_port = htons(port); // Port number
    addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
    bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct
    /* Bind a special Port */

    if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
    {
        printf ("ERROR: Failed to bind Port %d.\n",port);
        return (0);
    }
    else
	{
		printf("server: bind tcp port %d in addr 0.0.0.0 sucessfully.\n",port);
	}

    /* Listen remote connect/calling */
    if(listen(sockfd,BACKLOG) == -1)
    {
        printf ("ERROR: Failed to listen Port %d.\n", port);
        return (0);
    }
    else
	{
		printf ("server: listening the port %d sucessfully.\n", port);
	}


    int success=0;



	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = head_ctrl_c;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	jobs_load();
	log_alarm_wakeup (0);
	pthread_create( &sim->alarm, NULL,head_alarm_wakeup,NULL);
	while(success == 0)
    {
        sin_size = sizeof(struct sockaddr_in);
        /* Wait a connection, and obtain a new socket file despriptor for single connection */
        if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1)
		{
            printf ("ERROR: Obtain new Socket Despcritor error.\n");
		}
        else
		{
			printf ("[server] server has got connect from %s.\n", inet_ntoa(addr_remote.sin_addr));
		}


		pthread_t thread1;
		//int iret1;

		//iret1 =
		char ip[100];
		get_ip_from_sock(ip,nsockfd);
		printf("making thread for  ip=%s %d\n",ip,nsockfd);
		pthread_create( &thread1, NULL,rx_loop,(void*)&nsockfd);

    }

printf("Exiting cluster code\n");

return 0;
}
