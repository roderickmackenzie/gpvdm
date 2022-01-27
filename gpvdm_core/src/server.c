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

/** @file server.c
	@brief Job management for fitting, run multiple fitting instances over multiple CPUs.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dump.h>
#include <unistd.h>
#include "util.h"

	#include <sys/inotify.h>
	#include <sys/time.h>
	#include <signal.h>
	#include <linux/limits.h>
	#define EVENT_SIZE  ( sizeof (struct inotify_event) )
	#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#include "sim.h"
#include "server.h"
#include "inp.h"
#include "timer.h"
#include "gui_hooks.h"
#include "lang.h"
#include "log.h"
#include <gui_hooks.h>
#include <cal_path.h>
#include <device_fun.h>

static int unused __attribute__((unused));

struct simulation *local_sim;



static double server_jobs_per_s=0.0;
static double server_odes_per_s=0.0;

static time_t last_job_ended_at;

void server_update_last_job_time()
{
last_job_ended_at=time(NULL);
}

void change_cpus(struct simulation *sim,struct server_struct *myserver)
{
}

/*void alarm_wakeup (int i)
{
#ifdef enable_server
		struct itimerval tout_val;

		signal(SIGALRM,alarm_wakeup);

		tout_val.it_interval.tv_sec = 0;
		tout_val.it_interval.tv_usec = 0;
		tout_val.it_value.tv_sec = alarm_time; // 10 seconds timer 
		tout_val.it_value.tv_usec = 0;

		int since_last_job=(int)(time(NULL)-last_job_ended_at);
		//Note we can't use print f within a signal handelr.
		//printf_log(local_sim,"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Time since %d seconds since last job run.\n",since_last_job);
		if (since_last_job>stall_time)
		{
			ewe(local_sim,"\n%s %ld>%ld\n",_("Job stalled. Time since last run:"),since_last_job,stall_time);
		}

		setitimer(ITIMER_REAL, &tout_val,0);
		server_check_wall_clock(local_sim,pserver);
#endif
}*/


int cmp_lock(char *in)
{
return -1;
}

void server_set_dbus_finish_signal(struct server_struct *myserver, char *signal)
{
	strcpy(myserver->dbus_finish_signal,signal);
}

void server_set_lock_file(struct server_struct *myserver, char *file_name)
{
	strcpy(myserver->lock_file,file_name);
}

void server_shut_down(struct simulation *sim,struct server_struct *myserver)
{
	gui_send_finished_to_gui(sim);
}

void print_jobs(struct simulation *sim)
{
}

double server_get_odes_per_s()
{
return server_odes_per_s;
}

double server_get_jobs_per_s()
{
return server_jobs_per_s;
}

void server_init(struct simulation *sim)
{

local_sim=sim;

strcpy(sim->server.dbus_finish_signal,"");
strcpy(sim->server.lock_file,"");


}



void server_add_job(struct simulation *sim,char *command,char *output)
{
char send_data[PATH_MAX];
char temp[PATH_MAX];
poll_gui(sim);
struct device dev;

	int odes=0;

	//printf("%s %s\n",command,output);
	//getchar();

	gui_send_data(sim,gui_main,"pulse");
	sprintf(send_data,"text:%s",output);
	gui_send_data(sim,gui_main,send_data);

	device_init(sim,&dev);
	strcpy(dev.input_path,command);
	strcpy(dev.output_path,output);
	odes=device_run_simulation(sim,&dev);

	printf_log(sim,"Solved %d ODEs\n",odes);
}


void server_exe_jobs(struct simulation *sim, struct server_struct *myserver)
{
char lockname[PATH_MAX];
char my_temp[PATH_MAX+20];

if (myserver->jobs==0) return;
}


void server_job_finished(struct server_struct *myserver,char *job)
{
}

int server_run_jobs(struct simulation *sim,struct server_struct *myserver)
{

	return 0;
}

void server_check_wall_clock(struct simulation *sim,struct server_struct *myserver)
{
time_t now = time(NULL);
if (now>myserver->end_time)
{
struct tm tm = *localtime(&now);

printf_log(sim,"Server quit due to wall clock at: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
now-=myserver->start_time;
printf_log(sim,"I have run for: %lf hours\n",now/60.0/60.0);
exit(0);
}
}

void server_stop_and_exit()
{
	exit(0);
}
