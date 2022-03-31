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

/** @file exit.c
	@brief Exit while sending sane data to the gui, also handle crashes while fitting.
*/

#include <enabled_libs.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "log.h"
#include "gpvdm_const.h"
//#include "dump_ctrl.h"
//#include "dos_types.h"
#include "gui_hooks.h"
#include "lang.h"
#include <signal.h>
#include <server.h>
#include <unistd.h>

static char lock_name[100];
static char lock_data[100];

void errors_add(struct simulation *sim, const char *format, ...)
{

	char temp[1000];
	char temp2[1000];
	int len=0;
	int next_size=0;
	int ret=0;

	va_list args;
	va_start(args, format);
	vsprintf(temp,format, args);

	ret=snprintf(temp2,1000,"error:%s\n",temp);
	if (ret<0)
	{
		ewe(sim,"ret error\n");
	}

	len=strlen(temp2);

	ewe(sim,temp2);

	next_size=sim->error_log_size+len;
	if (next_size>sim->error_log_size_max)
	{
		sim->error_log_size_max+=1024;
		sim->error_log=realloc(sim->error_log,sizeof(char)*sim->error_log_size_max);
	}

	sim->error_log_size=next_size;
	strcat(sim->error_log,temp2);
	sim->errors_logged++;

}

void errors_dump(struct simulation *sim)
{
	printf("%s\n",sim->error_log);
}

int is_errors(struct simulation *sim)
{
	if (sim->errors_logged==0)
	{
		return -1;
	}

	return 0;
}

void errors_init(struct simulation *sim)
{
	sim->error_log_size=0;
	sim->error_log_size_max=1024;
	sim->error_log=malloc(sizeof(char)*sim->error_log_size_max);
	strcpy(sim->error_log,"");
	sim->errors_logged=0;

}

void errors_free(struct simulation *sim)
{
	sim->error_log_size=0;
	sim->error_log_size_max=0;
	free(sim->error_log);
	sim->errors_logged=0;
}

void set_ewe_lock_file(char *lockname,char *data)
{
strcpy(lock_name,lockname);
strcpy(lock_data,data);
}

int ewe( struct simulation *sim, const char *format, ...)
{
	FILE* out;
	char temp[1000];
	char temp2[1000];
	int ret=0;
	va_list args;
	va_start(args, format);
	vsprintf(temp,format, args);

	ret=snprintf(temp2,1000,"error:%s",temp);
	if (ret<0)
	{
		printf("error.c: sprintf error\n");
		exit(0);
	}
	rainbow_print(sim,600e-9, 640e-9, "%s\n",temp2);
	//printf_log(sim, "%s\n",temp2);

	va_end(args);

		//getchar();




	gui_send_finished_to_gui(sim);


	if (strcmp(lock_name,"")!=0)
	{
		out=fopen(lock_name,"w");
		fprintf(out,"%s",lock_data);
		fclose(out);
	}
	//printf_log(sim,"Raising segmentation fault\n");
	//raise(SIGSEGV);

	write_lock_file(sim);
exit(1);
}

void write_lock_file( struct simulation *sim)
{
	if (strcmp(sim->server.lock_file,"")!=0)
	{
		char lockname[500];
		FILE *out=fopen(sim->server.lock_file,"w");
		if (out == NULL)
		{
			printf("Problem writing file!\n");
			getchar();
		}
		fclose(out);

	}
}
