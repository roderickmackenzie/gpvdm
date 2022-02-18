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

/** @file fit_dump.c
@brief dump the fit progress.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <config.h>
#include "fit.h"
#include <inp.h>
#include <util.h>
#include <cal_path.h>
#include <sys/stat.h>
#include <log.h>
#include <lang.h>
#include <dump.h>
#include <dump_ctrl.h>
#include <server.h>
#include <gpvdm_const.h>
#include <timer.h>

#include <dat_file.h>


void fit_log_init(struct simulation *sim)
{
struct dat_file buf;
dat_file_init(&buf);

	if (buffer_set_file_name(sim,NULL,&buf,"fit_constraints.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		strcpy(buf.title,_("Fit error"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Step"));
		strcpy(buf.data_label,_("Error"));
		strcpy(buf.y_units,_("au"));
		strcpy(buf.data_units,_("au"));
		strcpy(buf.cols,"yd");
		buf.x=1;
		buf.y=-1;
		buf.z=1;
		buffer_add_json(sim,&buf);
		buffer_dump_path(sim,sim->root_simulation_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,NULL,&buf,"fitlog.csv")==0)
	{
		buffer_malloc(&buf);
		strcpy(buf.title,_("Fit error"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Step"));
		strcpy(buf.data_label,_("Error"));
		strcpy(buf.y_units,"au");
		strcpy(buf.data_units,"au");
		strcpy(buf.cols,"yd");
		buf.x=1;
		buf.y=-1;
		buf.z=1;
		buffer_add_json(sim,&buf);
		buffer_dump_path(sim,sim->root_simulation_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,NULL,&buf,"fitlog_time_error.csv")==0)
	{
		buffer_malloc(&buf);
		strcpy(buf.title,_("Fit error"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.data_label,_("Error"));
		strcpy(buf.y_units,"s");
		strcpy(buf.data_units,"au");
		strcpy(buf.cols,"yd");
		buf.x=1;
		buf.y=-1;
		buf.z=1;
		buffer_add_json(sim,&buf);
		buffer_dump_path(sim,sim->root_simulation_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,NULL,&buf,"fitlog_time_speed.csv")==0)
	{
		buffer_malloc(&buf);
		strcpy(buf.title,_("Fit error"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.data_label,_("Speed"));
		strcpy(buf.y_units,"s");
		strcpy(buf.data_units,_("odes/s"));
		strcpy(buf.cols,"yd");
		buf.x=1;
		buf.y=-1;
		buf.z=1;
		buffer_add_json(sim,&buf);
		buffer_dump_path(sim,sim->root_simulation_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,NULL,&buf,"fitlog_time_odes.csv")==0)
	{
		buffer_malloc(&buf);
		strcpy(buf.title,_("Fit error"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.data_label,_("Speed"));
		strcpy(buf.y_units,"s");
		strcpy(buf.data_units,_("odes/s"));
		strcpy(buf.cols,"yd");
		buf.x=1;
		buf.y=-1;
		buf.z=1;
		buffer_add_json(sim,&buf);
		buffer_dump_path(sim,sim->root_simulation_path,NULL,&buf);
		buffer_free(&buf);
	}

}

void fit_dump_log(struct simulation *sim,struct fitvars *fitconfig,double error,double size)
{
int i;
FILE *out;

printf_log(sim,"fit_dump_log\n");


out=fopen("fitlog.csv","a");
if (out!=NULL)
{
	printf_log(sim,"%d %le %le %le\n", fitconfig->iterations, error, sim->last_total_error,size);
	//fprintf (out,"%d %le %le\n", fitconfig->iterations, error, size);
	fprintf (out,"%d %le\n", fitconfig->iterations, error);
	fclose(out);
}

out=fopen("fit_constraints.csv","a");
if (out!=NULL)
{
	fprintf (out,"%d ", fitconfig->iterations);

	for (i=0;i<fitconfig->n_constraints;i++)
	{
		fprintf (out,"%le ", fitconfig->constraints_error[i]);
	}

	fprintf (out,"\n");

	fclose(out);
}

out=fopen("fitlog_time_error.csv","a");
if (out!=NULL)
{
	fprintf (out,"%lf %le\n", timer_get_time(0), error);
	fclose(out);
}

out=fopen("fitlog_time_speed.csv","a");
if (out!=NULL)
{
	fprintf (out,"%le %le\n", timer_get_time(0), server_get_jobs_per_s());
	fclose(out);
}

out=fopen("fitlog_time_odes.csv","a");
if (out!=NULL)
{
	fprintf (out,"%le %le\n", timer_get_time(0), server_get_odes_per_s());
	fclose(out);
}

printf_log(sim,"exit fit_dump_log\n");

}
