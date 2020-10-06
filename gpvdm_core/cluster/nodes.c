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

/** @file nodes.c
@brief functions to opperate on nodes
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
#include<pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct node_struct nodes[100];
static int nnodes=0;

struct node_struct *nodes_list()
{
return nodes;
}

int send_packet_to_node(char *ip,struct tx_struct *packet)
{
printf("broadcast\n");
int i;

	for (i=0;i<nnodes;i++)
	{

		if (strcmp(nodes[i].type,"slave")==0)
		{
			if (strcmp(nodes[i].ip,ip)==0)
			{
				tx_packet(nodes[i].sock,packet,NULL);
			}
		}
	}

return 0;
}

int broadcast_to_nodes(struct tx_struct *packet)
{
printf("broadcast\n");
int i;

	for (i=0;i<nnodes;i++)
	{

		if (strcmp(nodes[i].type,"slave")==0)
		{
			printf("broadcast to %s\n",nodes[i].host_name);

			tx_packet(nodes[i].sock,packet,NULL);

		}
	}

return 0;
}

struct node_struct* node_find(char *ip)
{
int i;
	for (i=0;i<nnodes;i++)
	{
		if (strcmp(nodes[i].ip,ip)==0)
		{
			return &(nodes[i]);
		}
	}

return NULL;
}

struct node_struct* node_find_master()
{
int i;
	for (i=0;i<nnodes;i++)
	{
		if (strcmp(nodes[i].type,"master")==0)
		{
			return &(nodes[i]);
		}
	}

return NULL;
}


int register_node(int sock)
{

int cpus;
	char host_name[200];

	gethostname(host_name, 200);

	cpus = sysconf( _SC_NPROCESSORS_ONLN );

	struct tx_struct packet;
	tx_struct_init(&packet);
	tx_set_id(&packet,"gpvdmaddnode");
	strcpy(packet.ip, get_my_ip());
	packet.cpus=cpus;
	strcpy(packet.host_name, host_name);

	tx_packet(sock,&packet,NULL);

	return 0;
}

int close_all_open()
{
	int i;

	for (i=0;i<nnodes;i++)
	{
		if (nodes[i].sock!=-1)
		{
    		close (nodes[i].sock);
		}
	}

	return 0;
}

int send_delete_node(int sock)
{
struct tx_struct packet;
	printf("head sock=%d",sock);
	if (sock>=0)
	{
		tx_struct_init(&packet);
		tx_set_id(&packet,"gpvdmdeletenode");

		tx_packet(sock,&packet,NULL);
	}
	return 0;
}

int cmp_addnode(int sock,struct tx_struct *data)
{
	if (cmpstr_min(data->id,"gpvdmaddnode")==0)
	{

		node_add("slave",data->ip,data->cpus,sock,data->host_name);
		nodes_print();
		return 0;
	}

return -1;
}

int cmp_deletenode(int sock_han,struct tx_struct *data)
{
char my_ip[20];
	if (cmpstr_min(data->id,"gpvdmdeletenode")==0)
	{


		//printf( "address is %s:%u\n",, (unsigned)ntohs(adr_inet.sin_port));
		get_ip_from_sock(my_ip,sock_han);
		node_delete(my_ip);
		return 0;
	}

return -1;
}

void nodes_print()
{
int i;
printf("number\ttype\tname\t\tip\t\tcpus\tmax_cpus\tsock\tload\tload0\t\tlast_seen\n");
	for (i=0;i<nnodes;i++)
	{
		printf("%d\t%s\t%s\t\t%s\t%d\t%d\t\t%d\t%d\t%lf\t%d\n",i,nodes[i].type,nodes[i].host_name,nodes[i].ip,nodes[i].cpus,nodes[i].max_cpus,nodes[i].sock,nodes[i].load,nodes[i].load0,node_alive_time(&(nodes[i])));
	}
}

int cmp_sendnodelist(int sock,struct tx_struct *data)
{

	if (cmpstr_min(data->id,"gpvdmsendnodelist")==0)
	{
		nodes_txnodelist();
		return 0;
	}

return -1;
}


int nodes_html_load(char *buf)
{

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	int i;
	char temp[1024];

	strcpy(buf,"HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><html><head>");
	strcat(buf,"<title>Cluster status</title>");
	strcat(buf,"<meta http-equiv=\"refresh\" content=\"1\">");
	strcat(buf,"<body>");
	sprintf(temp,"<h1>Cluster status %d-%d-%d %d:%d:%d:</h1>", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	strcat(buf,temp);

	for (i=0;i<nnodes;i++)
	{
		sprintf(temp,"Node <b>%s</b> (%s CPUs=%d): <b>%lf</b><br>",nodes[i].host_name,nodes[i].ip,nodes[i].cpus,nodes[i].load0);
		strcat(buf,temp);
	}

	strcat(buf,"</body></html>\r\n");

return 0;
}

int nodes_txnodelist()
{
	char *buf=NULL;
	char temp[1014];
	int i;
	struct tx_struct packet;
	struct node_struct* master=NULL;
	master=node_find_master();

	if (master!=NULL)
	{

		buf=malloc(LENGTH*sizeof(char));
		bzero(buf, LENGTH);

		for (i=0;i<nnodes;i++)
		{
			sprintf(temp,"%s:%s:%d:%d:%lf:%d:%d\n",nodes[i].host_name,nodes[i].ip,nodes[i].cpus,nodes[i].load,nodes[i].load0,nodes[i].max_cpus,node_alive_time(&(nodes[i])));
			strcat(buf,temp);
		}

		tx_struct_init(&packet);
		tx_set_id(&packet,"gpvdmnodelist");
		packet.size=strlen(buf);

		tx_packet(master->sock,&packet,buf);

		free(buf);

	}


return 0;
}

int nodes_get_nnodes()
{
return nnodes;
}

void nodes_reset()
{
nnodes=0;
}

int node_alive_time(struct node_struct* node)
{
return (int)(time(NULL)-node->alive);
}

pthread_mutex_t mutex_node_add=PTHREAD_MUTEX_INITIALIZER;	//PTHREAD_MUTEX_ERRORCHECK;

int node_add(char *type,char *ip,int cpus, int sock,char *host_name)
{
printf("adding IP a %s\n",ip);
pthread_mutex_lock( &mutex_node_add );
printf("adding IP b %s %d\n",ip,nnodes);
int found=FALSE;
int i;
	for (i=0;i<nnodes;i++)
	{
		if ((strcmp(nodes[i].ip,"none")==0)||((strcmp(nodes[i].ip,ip)==0)&&(strcmp(nodes[i].type,type)==0)))
		{
				strcpy(nodes[i].ip,ip);
				strcpy(nodes[i].type,type);
				strcpy(nodes[i].host_name,host_name);
				nodes[i].cpus=cpus;
				nodes[i].max_cpus=cpus;
				nodes[i].sock=sock;
				nodes[i].load=0;
				nodes[i].load0=0;
				found=TRUE;
				break;
		}
	}

	if (found==FALSE)
	{
		strcpy(nodes[nnodes].ip,ip);
		strcpy(nodes[nnodes].type,type);
		strcpy(nodes[nnodes].host_name,host_name);
		nodes[nnodes].cpus=cpus;
		nodes[nnodes].max_cpus=cpus;
		nodes[nnodes].sock=sock;
		nodes[nnodes].load=0;
		nodes[nnodes].load0=0;
		nnodes++;
	}

pthread_mutex_unlock( &mutex_node_add );
return 0;
}


pthread_mutex_t lock;

void copy_dir_to_all_nodes(struct state *sim,char *dir)
{
printf("copy_dir_to_all_nodes\n");
int i=0;
		for (i=0;i<nnodes;i++)
		{
			if (strcmp(nodes[i].ip,"none")!=0)
			{
				if ((strcmp(nodes[i].type,"slave")==0)&&(nodes[i].max_cpus!=0))
				{
					char full_path[200];

					join_path(2,full_path,calpath_get_store_path(), dir);
					printf("sending dir %s to %s\n",dir,nodes[i].ip);
					printf("Send to %s\n",full_path);
					send_dir(sim,nodes[i].sock,full_path, 0,full_path,dir);
					//tx_sync_packet_one(nodes[i].sock,full_path, dir);

				}
			}
		}
}

void run_jobs(struct state *sim)
{
//printf("run jobs1\n");
pthread_mutex_lock(&lock);
//printf("run jobs2\n");

if (nnodes==0)
{
	printf("Warning no nodes!!!!!!!!!!!\n");
}
int i;
//printf("run jobs3\n");

nodes_print();
printf("here xxx\n");
	struct job* next=NULL;
	do
	{
		next=jobs_get_next();

		if (next==NULL)
		{
			break;
		}

		int sent_job=FALSE;

		for (i=0;i<nnodes;i++)
		{
			if (strcmp(nodes[i].ip,"none")!=0)
			{
				if (strcmp(nodes[i].type,"slave")==0)
				{
					if (nodes[i].load<nodes[i].max_cpus)
					{
						char full_path[200];
						printf("sending job to %s\n",nodes[i].ip);
						join_path(2,full_path,calpath_get_store_path(), next->name);

						//tx_sync_packet_one(nodes[i].sock,full_path, next->name);
						send_dir(sim,nodes[i].sock,full_path, 0,full_path,next->name);

						send_command(nodes[i].sock,calpath_get_exe_name(),next->name,next->cpus_needed);
						strcpy(next->ip,nodes[i].ip);
						next->t_start=time(NULL);
						next->t_force_stop=time(NULL)+24*60*60;
						nodes[i].load+=next->cpus_needed;
						next->status=1;
						nodes_print();
						jobs_print();
						sent_job=TRUE;
						break;
					}
				}
			}
		}

		if (sent_job==FALSE)
		{
			break;
		}

	}
	while(1);
pthread_mutex_unlock(&lock);
printf("here xxxx\n");

}

int cmp_job_pid(struct state *sim,int sock,struct tx_struct *data)
{
	printf("%s\n",data->id);
	if (cmpstr_min(data->id,"thread_id")==0)
	{
		printf(">>>>>>>>>>>>>>>>>>>>THREAD IDDDDDD.... %s\n",data->dir_name);
		struct job* job=NULL;
		job=jobs_find_job(data->dir_name);
		if (job!=NULL)
		{
			printf(">>>>>>>>>thread ID %d\n",data->pid);
			job->pid=data->pid;
		}
		return 0;
	}
return -1;
}


int cmp_simfinished(struct state *sim,int sock,struct tx_struct *data)
{
	struct tx_struct packet;
	if (cmpstr_min(data->id,"gpvdmsimfinished")==0)
	{
		struct job* job=NULL;
		job=jobs_find_job(data->dir_name);
		if (job!=NULL)
		{

			jobs_print();

			struct node_struct* node=NULL;

			node=node_find(data->ip);
			if (node==NULL)
			{
				printf("I can't find IP %s\n",data->ip);
			}

			printf("load was %d %s \n",node->load,data->dir_name);
			if (job->status==1)
			{
				node->load-=job->cpus_needed;
			}
			job->status=2;
			job->t_stop=time(NULL);
			printf("load now %d %s\n",node->load,data->dir_name);

			run_jobs(sim);

			jobs_print();
		}



		struct node_struct* master=NULL;
		//master=node_find_master();
		//printf("sending @master - want!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		if (master!=NULL)
		{
			/*struct job* my_job=jobs_find_job(data->dir_name);
			if (my_job!=NULL)
			{
				//printf("sending @master!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%s\n",data->dir_name);

				join_path(2,full_dir,calpath_get_store_path(), data->dir_name);


				ret=send_dir(sim,master->sock,full_dir, 0,full_dir,my_job->target);

				if (ret!=0)
				{
					printf("dir not found %s %s\n",data->dir_name, calpath_get_store_path());
				}

			}else
			{
				printf("Can't find job %s\n",data->dir_name);
				jobs_print();
			}*/
			printf("here5\n");


			tx_struct_init(&packet);
			tx_set_id(&packet,"gpvdmpercent");
			packet.percent=(int)jobs_cal_percent_finished();

			tx_packet(sock,&packet,NULL);


			printf("jobs remaining %d\n",jobs_remaining());
			if (jobs_remaining()==0)
			{

				tx_struct_init(&packet);
				tx_set_id(&packet,"gpvdmfinished");
				tx_packet(sock,&packet,NULL);

				//jobs_clear_all();
			}
			printf("here6\n");


		}

		return 0;
	}

return -1;
}

void node_delete(char *ip)
{
int i;

	for (i=0;i<nnodes;i++)
	{
		//printf("cmp='%s' '%s'\n",ip,nodes[i].ip);
		if (strcmp(nodes[i].ip,ip)==0)
		{
			strcpy(nodes[i].ip,"none");
			strcpy(nodes[i].type,"none");
			nodes[i].cpus=-1;
			nodes[i].sock=-1;
			nodes[i].load=-1;
			nodes[i].load0=-1;
			break;
		}
	}

	nodes_print();
}
