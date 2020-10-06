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

/** @file rx_file.c
@brief fucntions to rx a file
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

#include "tx_packet.h"

static int count=0;
int cmp_rxfile(int sock,struct tx_struct *data,struct state *sim)
{
int ret=0;
struct inp_file decode;
char dir_name[400];
char full_path[400]; //full path

	if (cmpstr_min(data->id,"gpvdmfile")==0)
	{
		char *buf=NULL;

		cal_abs_path_from_target(full_path,data->target,data->file_name);
		get_dir_name_from_path(dir_name, full_path);

		if (sim->state==HEAD)
		{
			char ip[500];
			char temp[500];
			get_ip_from_sock(ip,sock);
			sprintf(temp,"%s->%s", ip,full_path);
			printf("'%s'\n",temp);
			send_message(temp);
		}

		mkdirs(full_path);

		printf("file: %s %s\n",dir_name, full_path);


		FILE *fp = fopen(full_path, "w");

		if(fp == NULL)
		{
			printf("the error is %s\n", strerror(errno));
			return (0);
		}

		if (data->size>0)
		{
			fwrite(data->data, sizeof(char), data->size, fp);
		}

		fclose(fp);


		if (data->stat!=0)
		{
			chmod(full_path,data->stat);
		}

		if (data->size>0)
		{
			free(data->data);
			data->data=NULL;
		}

		count++;
		return 0;


	}

return -1;
}


