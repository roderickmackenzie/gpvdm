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

/** @file command.c
@brief execute commands on the node or head node
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
#include <pthread.h>
#include <sys/prctl.h>

struct state *local_sim;
struct node_struct nodes[100];


int send_command(int sock,char *command,char *dir_name,int cpus)
{
	int ret=0;
	struct tx_struct packet;
	tx_struct_init(&packet);
	tx_set_id(&packet,"gpvdmcommand");
	strcpy(packet.exe_name,command);
	strcpy(packet.dir_name,dir_name);
	packet.cpus=cpus;

	ret=tx_packet(sock,&packet,NULL);


return ret;
}

//look up popen2

void* exec_command(void *in)
{

	char sim_dir[200];
	struct tx_struct data;
	copy_packet(&data,(struct tx_struct *)in);

	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	///pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	join_path(2,sim_dir,calpath_get_store_path(), data.dir_name);
	printf("change dir to %s\n",sim_dir);


	chdir(sim_dir);

	char full_exe_path[400];
	char lib_path[400];
	char command[400];
	printf("command>>>>>>=%s\n",data.exe_name);
	join_path(3,full_exe_path,calpath_get_store_path(), "src",data.exe_name);
	join_path(2,lib_path,calpath_get_store_path(), "src");
	sprintf(command,"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:%s;stdbuf -i0 -o0 -e0 %s",lib_path,full_exe_path);
	printf("full command =%s\n",command);
	int pid=fork();
	if (pid  == 0)
	{
		//prctl(PR_SET_PDEATHSIG, SIGKILL);

		//system("ls");
		//FILE *fp;
		system(command);
		exit(0);
	}

	printf("top level pid=%d\n",pid);
	tx_pid(pid,data.dir_name);

	int returnStatus;
    waitpid(pid, &returnStatus, 0);


	//int status = 0;
	//while ((wpid = wait(&status)) > 0);
	//system(command);


	//send_dir(sock, sim_dir, 0, sim_dir, data.dir_name);

	printf("FINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %s\n",data.dir_name);

	struct tx_struct packet;
	tx_struct_init(&packet);
	tx_set_id(&packet,"gpvdmsimfinished");
	strcpy(packet.dir_name,data.dir_name);
	strcpy(packet.ip,get_my_ip());
	packet.cpus=data.cpus;

	tx_packet(local_sim->head_sock,&packet,NULL);

	return NULL;
}

int cmp_node_runjob(struct state *sim,struct tx_struct *data)
{
	local_sim=sim;
	if (cmpstr_min(data->id,"gpvdmcommand")==0)
	{

		printf("I will run %s in a new process\n",data->exe_name);

		pthread_t thread;
		printf("command>>>>>>>=%s\n",data->exe_name);
		pthread_create( &thread, NULL,exec_command,(void*)data);
		sleep(1);		//This is super bad, but I want to give it enough time to copy the data in.
		//error=pthread_cancel(thread);
		//printf("error=%d\n",error);
		//printf("adaddadsad\n");
		//pthread_join(thread, &res);
		//printf("adaddadsad\n");

		return 0;
	}

return -1;
}

void tx_pid(int thread,char *dir_name)
{
	struct tx_struct packet;
	tx_struct_init(&packet);
	tx_set_id(&packet,"thread_id");
	strcpy(packet.dir_name,dir_name);
	strcpy(packet.ip,get_my_ip());
	packet.pid=(long int)(thread);

	tx_packet(local_sim->head_sock,&packet,NULL);
	printf("TX>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>. PID");
}
void* command_thread(void *in)
{
	struct tx_struct data;
	copy_packet(&data,(struct tx_struct *)in);
	FILE *fp;
	//int status;
	char path[1024];
	char sim_dir[200];
	char temp[2000];
	join_path(2,sim_dir,calpath_get_store_path(), data.dir_name);
	printf("change dir to %s\n",sim_dir);
	chdir(sim_dir);


	sprintf(temp,"The command is %s",data.command);
	printf("%s\n",temp);
	send_message(temp);

	getcwd(temp, 500);
	send_message(temp);

	fp = popen(data.command, "r");

	while (fgets(path, 1024, fp) != NULL)
	{
		sprintf(temp,"output>>%s", path);
		printf("rod>>>%s\n",temp);
		send_message(temp);
	}

	//status = pclose(fp);

	//system(data.command);

	if (local_sim->state==HEAD)
	{
		sprintf(temp,"I have run: %s", data.command);
		send_message(temp);

		struct node_struct* master=NULL;
		master=node_find_master();

		tx_thing_done(master->sock,&data);
	}

	return NULL;
}




int cmp_head_exe(struct state *sim,int sock,struct tx_struct *data)
{
local_sim=sim;

	if (cmpstr_min(data->id,"gpvdmheadexe")==0)
	{


		printf("I will run %s\n",data->command);
		if (sim->state==HEAD)
		{
			char temp[500];
			sprintf(temp,"running: %s", data->command);
			send_message(temp);
		}

		pthread_create( &sim->thread_command, NULL,command_thread,(void*)data);
		sleep(1);		//This is super bad, but I want to give it enough time to copy the data in.

		return 0;
	}

return -1;
}

