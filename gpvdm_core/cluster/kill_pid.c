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

/** @file kill_pid.c
@brief code to kill a job, bit complex still needs work
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdarg.h>

#include "inp.h"
#include "util.h"
#include <linux/limits.h>
#include <sys/stat.h>

int add_pid_list(int *list,int *list_len,int want_id)
{
int i;
for (i=0;i<*list_len;i++)
{
	if (list[i]==want_id)
	{
		return 0;
	}
}

list[*list_len]=want_id;

*list_len=*list_len+1;

return -1;
}

int update_pids(int *list, int *list_len,int want_id)
{
int added=0;
int i=0;
FILE *in;
struct dirent *next_file;
DIR *theFolder;
char filepath[256];
char line[1000];
int ppid;
int pid;
add_pid_list(list,list_len,want_id);

	theFolder = opendir("/proc/");
	if (theFolder!=NULL)
	{
		while((next_file=readdir(theFolder))!=NULL)
		{
			join_path(3, filepath ,"/proc/",next_file->d_name,"status");
			in=fopen(filepath,"r");
			if (in!=NULL)
			{
				i=0;
				while ((fgets(line, 1000, in) != NULL))
				{
					if  (cmpstr_min(line,"PPid:")==0)
					{
						sscanf(line,"PPid: %d",&ppid);
						sscanf(next_file->d_name,"%d",&pid);
						if (ppid==want_id)
						{
							if (add_pid_list(list,list_len,pid)==-1)
							{
								added=added+1;
							}
							//printf("%d %d\n", ppid,pid);
						}
						break;
					}
					i=i+1;
				}
				fclose(in);
			}
		}

		closedir (theFolder);
	}


return added;
}

void kill_all(int want_id)
{
int i;
int list[100];

int list_len=1;
list[0]=want_id;
int pos=0;
int added=0;
int cur_len=0;
while(1)
{
	cur_len=list_len;
	for (i=0;i<cur_len;i++)
	{
		added+=update_pids(list, &list_len,list[i]);
	}

	if (added==0)
	{
		break;
	}

	pos=pos+1;
	added=0;
}

int status;
for (i=list_len-1;i>0;i--)
{
	printf("killing %d\n",list[i]);
	sleep(0.1);
	kill(list[i], SIGQUIT);
	wait(&status);
}
}


