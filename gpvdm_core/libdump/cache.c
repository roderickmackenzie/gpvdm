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

/** @file cache.c
@brief cace so that files are not written to disk bit by bit instead at the end of the simulation
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "sim.h"
#include "dump.h"
#include <cal_path.h>
#include <string.h>
#include <log.h>
#include <stdlib.h>
#include <cache_struct.h>
#include <gui_hooks.h>
#include <lang.h>
#include "cache.h"

static int unused __attribute__((unused));

void cache_init(struct simulation *sim)
{
if ((sim->cache_len!=-1)||(sim->cache_max!=-1))
{
	ewe(sim,"You are trying to init the cache when it is not empty\n");
}
	sim->cache_len=0;
	sim->cache_max=2000;
	sim->cache=(struct cache_item *)malloc(sizeof(struct cache_item)*sim->cache_max);
}

void cache_dump(struct simulation *sim)
{
printf_log(sim,"%s\n",_("Writing files to disk"));
int i;
char send_data[200];
FILE *out;
struct stat st = {0};

gui_send_data(sim,gui_sub,"enable_pulse:false");
sprintf(send_data,"text:%s",_("Writing files to disk"));
gui_send_data(sim,gui_sub,send_data);

for (i=0;i<sim->cache_len;i++)
{
	if (sim->cache[i].len>=0)
	{
		sim->files_written++;
		sim->bytes_written+=sim->cache[i].len;
		out = fopen(sim->cache[i].file_name, "wb");
		fwrite(sim->cache[i].data, sim->cache[i].len, 1, out);
		fclose(out);
		log_write_file_access(sim,sim->cache[i].file_name,'w');
	}else
	{
		gpvdm_mkdir(sim->cache[i].file_name);

	}
	sprintf(send_data,"percent:%Lf",(gdouble)(i)/(gdouble)(sim->cache_len));
	gui_send_data(sim,gui_sub,send_data);
	//printf("write: %s\n",sim->cache[i].file_name);
}

}

void cache_free(struct simulation *sim)
{
int i;
	for (i=0;i<sim->cache_len;i++)
	{
		if (sim->cache[i].len>=0)
		{
			free(sim->cache[i].data);
		}
	}
	free(sim->cache);
	sim->cache_len=-1;
	sim->cache_max=-1;
}

int cache_search(struct simulation *sim,char * file_name)
{
return -1;
int i;
for (i=0;i<sim->cache_len;i++)
{
	if (strcmp(sim->cache[i].file_name,file_name)==0)
	{
		return i;
	}
}
return -1;
}

void cache_add_dir(struct simulation *sim,char * file_name)
{
	cache_add_item(sim,file_name,NULL,CACHE_DIR);
}

void cache_add_item(struct simulation *sim,char * file_name,char *data,int len)
{
int pos=0;
	pos=cache_search(sim,file_name);
	if (pos==-1)
	{
		strcpy(sim->cache[sim->cache_len].file_name,file_name);
		sim->cache[sim->cache_len].len=len;
		if (len>=0)
		{
			sim->cache[sim->cache_len].data=(char*)malloc(sizeof(char)*len);
			memcpy(sim->cache[sim->cache_len].data, data, len);
		}
		sim->cache_len++;
	}else
	{
		strcpy(sim->cache[pos].file_name,file_name);
		sim->cache[pos].len=len;
		if (len>=0)
		{
			sim->cache[pos].data=(char*)realloc(sim->cache[pos].data,sizeof(char)*len);
			memcpy(sim->cache[pos].data, data, len);
		}
	}

	if (sim->cache_len>=sim->cache_max)
	{
		sim->cache_max+=2000;
		sim->cache=(struct cache_item *)realloc(sim->cache,sizeof(struct cache_item)*sim->cache_max);
	}

}
