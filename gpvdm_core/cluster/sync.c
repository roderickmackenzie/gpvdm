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

/** @file sync.c
@brief rsync equivlent functions
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
#include <sys/stat.h>

#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/sha.h>

#include "tx_packet.h"


void get_md5sum(char *ret,char *file_name)
{

char *buf=NULL;
unsigned long len;
unsigned char md5[16];

//printf("%s\n",file_name);
FILE *f = fopen(file_name, "rb");
if (f==NULL)
{
	printf("%s not found\n",file_name);
	strcpy(ret,"");
	return;
}

fseek(f, 0, SEEK_END);
len = ftell(f);
fseek(f, 0, SEEK_SET);

buf = malloc(len*sizeof(char));
fread(buf, len, 1, f);
fclose(f);


MD5((const unsigned char*)buf, len,md5);

sprintf(ret,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);

free(buf);

return;
}

int cmp_sync_packet_one(int sock,struct tx_struct *data)
{
struct inp_file decode;
char full_path[400];

	if (cmpstr_min(data->id,"gpvdm_sync_packet_one")==0)
	{
		printf("gpvdm_sync_packet_one\n");

		inp_init(&decode);
		decode.data=data->data;
		decode.fsize=strlen(data->data);
		//printf("a got data%s\n",data->data);
		char *fname=(char *)-1;
		char fname_buf[400];

		char *md5=(char *)-1;
		char md5_buf[400];
		char *build=NULL;
		int len=LENGTH;
		build=malloc(sizeof(char)*len);
		bzero(build, len);

		int pos=0;
		char seg[1000];
		char file_sum[64];
		int items=0;

		while(1)
		{
			fname  = inp_get_string(&decode);

			if (fname==NULL)
			{
				break;
			}

			strcpy(fname_buf,fname);

			md5  = inp_get_string(&decode);
			if (md5==NULL)
			{
				break;
			}
			strcpy(md5_buf,md5);

			cal_abs_path_from_target(full_path,data->target,fname_buf);

			if (isdir(full_path)!=0)
			{

				get_md5sum(file_sum,full_path);

				int needed=TRUE;

				if (strcmp(file_sum,md5_buf)==0)
				{
					needed=FALSE;
				}

				if (needed==TRUE)
				{
					sprintf(seg,"%s\n",fname_buf);
					int seg_len=strlen(seg);

					if (seg_len+pos>len)
					{
						len+=LENGTH;
						build=realloc(build,sizeof(char)*len);
						bzero((build+len-LENGTH), LENGTH);

					}

					strcat(build,seg);
					pos+=seg_len;
					items++;
				}
			}

		}
		if (items!=0)
		{
			pos=pos-1;
		}

		struct tx_struct packet;
		tx_struct_init(&packet);
		tx_set_id(&packet,"gpvdm_sync_packet_two");
		tx_set_size(&packet,pos);
		tx_set_target(&packet,data->target);
		tx_set_src(&packet,data->src);

		tx_packet(sock,&packet,build);

		free(build);
		free(data->data);

		return 0;

	}

return -1;
}


int tx_sync_packet_one(int sock,char *src, char* target)
{
struct inp_file decode;
printf("tx_sync_packet_one\n");
char *out=NULL;
int len=LENGTH;
int pos=0;
char *file_name;
char md5[100];
char *packet=NULL;
int packet_len=LENGTH;
int packet_pos=0;
char seg[1024];
char full_path[1024];
out=malloc(sizeof(char)*len);
bzero(out, len);
gen_dir_list(&out,&len,&pos,src,src);

packet=malloc(packet_len*sizeof(char));
bzero(packet, packet_len);

inp_init(&decode);
decode.data=out;
decode.fsize=strlen(out);
inp_reset_read(&decode);


while(1)
{
	file_name  = inp_get_string(&decode);
	if (file_name==NULL)
	{
		break;
	}

	cal_abs_path_from_target(full_path,target,file_name);
	if (isfile(full_path)==0)
	{
		get_md5sum(md5,full_path);

		sprintf(seg,"%s\n%s\n",file_name,md5);

		int seg_len=strlen(seg);

		if ((seg_len+packet_pos)>packet_len)
		{
			packet_len+=LENGTH;
			packet=realloc(packet,sizeof(char)*(packet_len));
		}

		strcat(packet,seg);
		packet_pos+=seg_len;
	}

}

struct tx_struct packet_header;
tx_struct_init(&packet_header);
tx_set_id(&packet_header,"gpvdm_sync_packet_one");
tx_set_size(&packet_header,packet_pos);
tx_set_target(&packet_header,target);
tx_set_src(&packet_header,src);

tx_packet(sock,&packet_header,packet);

free(packet);
free(out);

return -1;
}


int cmp_sync_packet_two(struct state *sim,int sock,struct tx_struct *data)
{
struct inp_file decode;

char full_path[400];

	if (cmpstr_min(data->id,"gpvdm_sync_packet_two")==0)
	{

		inp_init(&decode);
		decode.data=data->data;
		decode.fsize=strlen(data->data);


		char *fname=NULL;
		char fname_buf[400];

		while(1)
		{
			fname  = inp_get_string(&decode);

			if (fname==NULL)
			{
				break;
			}

			strcpy(fname_buf,fname);

			cal_abs_path_from_target(full_path,data->target,fname_buf);

			send_file(sim,sock,data->src,full_path,data->target);

		}

	free(data->data);
	printf("ended\n");
	return 0;
	}


return -1;

}
