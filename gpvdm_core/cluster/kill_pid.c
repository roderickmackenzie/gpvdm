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


