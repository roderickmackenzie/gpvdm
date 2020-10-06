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

/** @file util.c
@brief Utils for the cluster code
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

void remove_dir(char* dir_name)
{

struct dirent *next_file;
DIR *theFolder;
char filepath[256];


//if (get_dump_status(dump_newton)==TRUE)
//{

	theFolder = opendir(dir_name);
	if (theFolder!=NULL)
	{
		while((next_file=readdir(theFolder))!=NULL)
		{
			if ((strcmp(next_file->d_name,".")!=0)&&(strcmp(next_file->d_name,"..")!=0))
			{
				join_path(2, filepath,dir_name,next_file->d_name);
				if (isdir(filepath)==0)
				{
					remove_dir(filepath);
						remove(filepath);
				}else
				{
					//printf("Deleteing file =%s\n",filepath);
					remove(filepath);
				}
			}
		}

		closedir (theFolder);
	}
//}


}

int check_int(char *in)
{
int i=0;
int numeric=TRUE;
for (i=0;i<strlen(in);i++)
{
	if ((in[i]<48)||(in[i]>57))
	{
		numeric=FALSE;
		break;
	}
}
return numeric;
}

int ewe( const char *format, ...)
{
	char temp[1000];
	va_list args;
	va_start(args, format);
	vsprintf(temp,format, args);

	printf(temp,"error:%s",temp);

exit(1);

}

int cmpstr_min(char * in1,char *in2)
{
int i;
if (strlen(in1)<strlen(in2))
{
	return -1;
}
int max=strlen(in1);
if (strlen(in2)<max) max=strlen(in2);
for (i=0;i<max;i++)
{
	if (in1[i]!=in2[i]) return 1;
}
return 0;
}

int isdir(char *dir)
{
struct stat st = {0};

if (stat(dir, &st) == -1)
{
	return -1;
}
return 0;
}

void mkdirs(char *dir)
{
int i;
char temp[400];
strcpy(temp,dir);
for (i=0;i<strlen(dir);i++)
{
	if (temp[i]=='/')
	{
		temp[i]=0;
		if (isdir(temp)==-1)
		{
			mkdir(temp, S_IRWXU);
		}
		strcpy(temp,dir);
	}
}

}

void join_path(int max, ...)
{
	max=max+1;
	char temp[1000];
	strcpy(temp,"");
	va_list arguments;
	int i;
	int ii;
	va_start ( arguments, max );
	char *ret=va_arg ( arguments, char * );
	strcpy(ret,"");
	for (i = 1; i < max; i++ )
	{
		if ((i!=1)&&(strcmp(temp,"")!=0))
		{
			strcat(ret,"/");
		}
		strcpy(temp,va_arg ( arguments, char * ));

		if (strlen(temp)>0)
		{
			for (ii=strlen(temp)-1;ii>=0;ii--)
			{
				if (temp[ii]!='/')
				{
					break;
				}else
				{
					temp[ii]=0;
				}
			}
		}
		strcat(ret,temp);
	}
	va_end ( arguments );                  // Cleans up the list

	return;
}

int english_to_bin( char * in)
{
int ret=0;
if (check_int(in)==TRUE)
{
sscanf(in,"%d",&ret);
return ret;
}

if (strcmp(in,"true")==0)
{
	return TRUE;
}else
if (strcmp(in,"false")==0)
{
	return FALSE;
}else
if (strcmp(in,"1")==0)
{
	return TRUE;
}else
if (strcmp(in,"0")==0)
{
	return FALSE;
}else
if (strcmp(in,"yes")==0)
{
	return TRUE;
}else
if (strcmp(in,"no")==0)
{
	return FALSE;
}else
if (strcmp(in,"ja")==0)
{
	return TRUE;
}else
if (strcmp(in,"nein")==0)
{
	return FALSE;
}


ewe("I don't understand the command %s\n",in);
return 0;
}



char *get_file_name_from_path(char *in)
{
int i=0;
for (i=strlen(in)-1;i>0;i--)
{
		if (in[i]=='/')
		{
			return (in+i+1);
		}
}
return in;
}

int get_dir_name_from_path(char *out, char *in)
{
strcpy(out,in);

int i=0;
for (i=strlen(in);i>0;i--)
{
		if (in[i]=='/')
		{
			out[i]=0;
			return 0;
		}
}

return -1;
}
