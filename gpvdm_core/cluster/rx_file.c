// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright 2008-2022 Roderick C. I. MacKenzie https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
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


