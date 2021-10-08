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
