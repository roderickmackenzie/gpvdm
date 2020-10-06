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

/** @file gui_hooks.c
	@brief Handle GUI communcation dbus for Linux and pipes for windows.
*/



#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <gui_hooks.h>
#include <util.h>
#include <gpvdm_const.h>
#include <log.h>
#include <string.h>
#include <cal_path.h>


#ifdef dbus
	#include <dbus/dbus.h>
#endif


struct timeval my_last_time;

void gui_send_finished_to_gui(struct simulation *sim)
{
printf_log(sim,"finished signal=%s\n",sim->server.dbus_finish_signal);
if (strcmp(sim->server.dbus_finish_signal,"")!=0)
{
	gui_send_data(sim,gui_main,sim->server.dbus_finish_signal);
}

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

	gui_close_pipe(sim);
}

void poll_gui(struct simulation *sim)
{
char temp[PATH_MAX];
FILE *in;
char line[200];
strcpy(temp,"");
strcpy(line,"");
join_path(2,temp,sim->root_simulation_path,"tx.dat");
//printf("checking for tx from %s\n",temp);
if (isfile(temp)==0)
{
	in=fopen(temp,"r");
	if (in==NULL)
	{
		return;
	}
	fscanf(in,"%s",line);
	fclose(in);
	//remove_file(sim,temp);

	//printf("read from gui:%s\n", line);
	if (strcmp(line,"terminate")==0)
	{
		printf("terminate!!!");
		ewe(sim,"User terminated");
	}
}
//else
//{
//	printf("no file\n");
//}


}

void gui_terminal_reset(struct simulation *sim)
{
	if (sim->gui==TRUE)
	{
		printf_log(sim,"<clear_terminal>\n");
	}
}


int gui_send_data (struct simulation *sim,int from,char *tx_data_in)
{
char tx_data[1024];
char temp[1024];
int ret;

if (sim->gui==TRUE)
{
	if (sim->fitting==TRUE)
	{
		if (from==gui_sub)
		{
			return 0;
		}
	}
	if (sim->mindbustx==TRUE)
	{
		if ((strcmp_begin(tx_data_in,"pulse")==0)||(strcmp_begin(tx_data_in,"text")==0)||(strcmp_begin(tx_data_in,"percent")==0))
		{
			return 0;
		}
	}

	if ((strcmp_begin(tx_data_in,"pulse")==0)||(strcmp_begin(tx_data_in,"percent")==0))
	{
		struct timeval mytime;
		struct timeval result;

		gettimeofday (&mytime, NULL);

		timersub(&mytime,&my_last_time,&result);
		double diff=(double)result.tv_sec + ((double)result.tv_usec)/1000000.0;

		//printf("no %ld %ld %lf\n",(long)my_last_time.tv_sec,(long)my_last_time.tv_usec,diff);
		if (diff<0.15)
		{
			//printf("return\n");
			return 0;
		}

		gettimeofday (&my_last_time, NULL);
		//printf("reset\n");

	}


		//printf("sending data!!!!!!!!!!!!!!!!!!!!!!\n");

		string_to_hex(temp,tx_data_in);
		ret=snprintf(tx_data,1024,"hex%s",temp);
		if (ret<0)
		{
			ewe(sim,"tx_data error\n");
		}
		#ifdef dbus

		DBusMessage *message;
		message = dbus_message_new_signal ("/org/my/test","org.my.gpvdm",tx_data);
		/* Send the signal */
		dbus_connection_send (sim->connection, message, NULL);
		dbus_connection_flush(sim->connection);
		dbus_message_unref (message);

		#endif



}
return 0;
}

int dbus_init()
{
my_last_time.tv_sec=0;
my_last_time.tv_usec=0;
return 0;
}

void gui_close_pipe(struct simulation *sim)
{

	#ifdef dbus
	if (sim->connection!=NULL)
	{
		dbus_connection_unref (sim->connection);
		//dbus_connection_close(sim->connection);
	}
	dbus_shutdown();
	#endif
}

void gui_start(struct simulation *sim)
{
	gettimeofday (&my_last_time, NULL);

	#ifdef dbus
	DBusError error;
	dbus_error_init (&error);
	sim->connection = dbus_bus_get (DBUS_BUS_SESSION, &error);

	if (!sim->connection)
	{
		printf_log(sim,"Failed to connect to the D-BUS daemon: %s", error.message);
		sim->connection=NULL;
		dbus_error_free (&error);
		return;
	}
	#endif


	gui_send_data(sim,gui_main,"start");
}
