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

/** @file plot.c
	@brief A gnuplot interface for real time plotting.
*/


#include <sys/time.h>
#include <string.h>
#include "sim.h"
#include "dump.h"
#include "plot.h"
#include "util.h"

struct timeval last_time;

static char plot_script_dir[500];

void set_plot_script_dir(char * in)
{
strcpy(plot_script_dir,in);
strcat(plot_script_dir,"/plot/");
}

void plot_open(struct simulation *sim)
{
if (get_dump_status(sim,dump_plot)==TRUE)
{
gettimeofday (&last_time, NULL);
	sim->gnuplot = popen("gnuplot -persist","w");
	fprintf(sim->gnuplot, "set terminal x11 title 'General-purpose Photovoltaic Device Model - www.gpvdm.com' \n");
	fflush(sim->gnuplot);
}

}

void plot_now(struct simulation *sim,struct device *in,char *name)
{
struct timeval mytime;
struct timeval result;
gettimeofday (&mytime, NULL);
timersub(&mytime,&last_time,&result);
double diff=result.tv_sec + result.tv_usec/1000000.0;

if (diff<1e-1)
{
return;
}

last_time.tv_sec=mytime.tv_sec;
last_time.tv_usec=mytime.tv_usec;

if (get_dump_status(sim,dump_plot)==TRUE)
{
	dump_write_to_disk(sim,in);
	fprintf(sim->gnuplot, " load'%s%s'\n",plot_script_dir,name);
	fflush(sim->gnuplot);
}
}

void plot_now_excite(struct simulation *sim)
{
if (get_dump_status(sim,dump_plot)==TRUE)
{

	fprintf(sim->gnuplot, "load 'plot_excite'\n");
	fflush(sim->gnuplot);

}
}

void plot_replot(struct simulation *sim)
{
if (get_dump_status(sim,dump_plot)==TRUE)
{

	fprintf(sim->gnuplot, "replot\n");
	fflush(sim->gnuplot);

}
}



void plot_close(struct simulation *sim)
{
if (get_dump_status(sim,dump_plot)==TRUE)
{

	fprintf(sim->gnuplot, "exit\n");
	fflush(sim->gnuplot_time);
	pclose(sim->gnuplot);

}
}
