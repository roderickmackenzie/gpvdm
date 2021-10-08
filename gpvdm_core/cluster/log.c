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

/** @file log.c
@brief log what has been done to disk
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

#include "util.h"
#include "inp.h"


void log_alarm_wakeup (int i)
{
		int ii;
		FILE *out;
		struct node_struct *nodes=nodes_list();
		struct itimerval tout_val;

		signal(SIGALRM,log_alarm_wakeup);

		tout_val.it_interval.tv_sec = 0;
		tout_val.it_interval.tv_usec = 0;
		tout_val.it_value.tv_sec = 60;
		tout_val.it_value.tv_usec = 0;

		char path[200];
		//printf("log\n %d",nodes_get_nnodes());
		for (ii=0;ii<nodes_get_nnodes();ii++)
		{
			//printf("ii= %d\n",ii);
			//printf("nodes[ii].ip= '%s'\n",nodes[ii].ip);
			sprintf(path,"./logs/%s",nodes[ii].ip);
			//printf("log %s \n",path);
			out=fopen(path,"a");
			if (out!=NULL)
			{
				fprintf(out,"%lf\n",nodes[ii].load0);
				fclose(out);
			}else
			{
				printf("can't open log %s\n",nodes[ii].ip);
			}

			}

		setitimer(ITIMER_REAL, &tout_val,0);
}

