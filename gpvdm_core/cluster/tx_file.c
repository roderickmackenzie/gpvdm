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

/** @file tx_file.c
@brief transmit a file.
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
#include "util.h"
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/wait.h>
#include "tx_packet.h"
#include <state.h>

void strip_slash(char *in)
{
int delta=0;
char temp[1024];
bzero(temp, 1024);
strcpy(temp,in);
while(in[delta]=='/')
{
	delta++;
}
strcpy(in,temp+delta);
}

int gen_dir_list(char ** out,int *len,int *pos,const char *path,char *base_path)
{

    DIR *dir;
	char next_path[1024];
	char full_path[1024];
	char seg[1024];
	int path_ofset=strlen(base_path);
    struct dirent *entry;

    if (!(dir = opendir(path)))
	{
        return -1;
	}

    if (!(entry = readdir(dir)))
	{
        return -1;
	}

    do
	{
		if (entry->d_type == DT_DIR)
		{

			join_path(2,next_path,path, entry->d_name);

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
                continue;
			}
            gen_dir_list(out,len,pos,next_path,base_path);
        }else
        {
			join_path(2,full_path,path, entry->d_name);
			//printf("%s\n", full_path);

			sprintf(seg,"%s\n",full_path+path_ofset);
			strip_slash(seg);
			//printf("adding: %s\n", seg);
			int seg_len=strlen(seg);

			if (seg_len+(*pos)>(*len))
			{
				(*len)+=LENGTH;
				*out=realloc(*out,sizeof(char)*(*len));
			}

			strcat(*out,seg);
			(*pos)+=seg_len;

		}
    }while (entry = readdir(dir));

    closedir(dir);

return 0;
}

int send_dir(struct state *sim,int sockfd,const char *name, int level,char *base_name, char* target)
{
printf("sending dir %s %s %ld\n",base_name,target,level);
if (sim->state==HEAD)
{
	char temp[500];
	char ip[500];
	get_ip_from_sock(ip,sockfd);
	sprintf(temp,"sending from head to dir %s %s->%s %s", base_name,name,target,ip);
	send_message(temp);
}
    DIR *dir;
	struct stat stat_buf;
    struct dirent *entry;

    if (!(dir = opendir(name)))
	{
        return -1;
	}

    if (!(entry = readdir(dir)))
	{
        return -1;
	}

    do
	{
		if (entry->d_type == DT_DIR)
		{
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
                continue;
			}
            send_dir(sim,sockfd,path, level + 1,base_name,target);
        }else
        {
			char full_path[500];
			join_path(2,full_path,name, entry->d_name);
			printf("%s %s\n",name, entry->d_name);

			lstat (full_path, &stat_buf);

			if (S_ISLNK(stat_buf.st_mode))
			{
				printf("Not sending link: %s\n", full_path);
			}else
			{
				printf("sending now: %s\n", full_path);
				send_file(sim,sockfd,base_name, full_path,target);
				printf("finished sending now: %s\n", full_path);

			}
		}
    }while (entry = readdir(dir));

    closedir(dir);
printf("finished sending dir %s %s %ld\n",base_name,target,level);
if (sim->state==HEAD)
{
	char temp[500];
	sprintf(temp,"finished sending dir %s %s->%s", base_name,name,target);
	send_message(temp);
}
return 0;
}


int send_file(struct state *sim,int sockfd,char *base_name,char *file_name,char *target)
{
struct stat results;
struct tx_struct packet;
char rel_name[400];
char *buf=NULL;
if (sim->state==HEAD)
{
	char temp[500];
	sprintf(temp,"sending file %s %s %s", base_name,file_name,target);
	send_message(temp);
}
tx_struct_init(&packet);
tx_set_id(&packet,"gpvdmfile");
if (stat(file_name, &results)!=0)
{
	printf("Error sending file %s my guess would be it does not exists!\n",file_name);
	exit(0);
}

strcpy(rel_name,file_name+strlen(base_name));
tx_set_file_name(&packet,rel_name);
tx_set_target(&packet,target);
tx_set_size(&packet,results.st_size);
tx_set_stat(&packet,results.st_mode);
tx_set_zip(&packet,1);

	printf("disk read\n");
	if (results.st_size>0)
	{
		buf=(char*)malloc(sizeof(char)*results.st_size);
		bzero(buf, results.st_size);

		int f_block_sz = 0;

		FILE *fp=NULL;

		fp = fopen(file_name, "r");
		if (fp == NULL)
		{
			printf(" %s\n", strerror(errno));
			return -1;
		}

		f_block_sz = fread(buf, sizeof(char), results.st_size, fp);

		if (f_block_sz!=results.st_size)
		{
			printf("Not read enough data\n");
			return -1;
		}

		fclose(fp);
	}

	printf("call tx packet\n");
	tx_packet(sockfd,&packet,buf);
	printf("tx packet ended\n");

	free(buf);

if (sim->state==HEAD)
{
	char temp[500];
	sprintf(temp,"sent file %s %s %s", base_name,file_name,target);
	send_message(temp);
}
return 0;
}
