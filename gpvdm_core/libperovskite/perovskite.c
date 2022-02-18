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

/** @file perovskite.c
	@brief A perovskite ion solver.
*/
#include <enabled_libs.h>
#include <string.h>
#include <stdlib.h>
#include <dump.h>
#include <dos.h>
#include "sim.h"
#include "solver_interface.h"
#include "dat_file.h"
#include "log.h"
#include <cal_path.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <lang.h>
#include <perovskite.h>
#include <inp.h>
#include <plot.h>
#include <memory.h>
#include <json.h>
#include <device_fun.h>

gdouble min_perovskite_error=1e-10;

void perovskite_init(struct simulation *sim,struct device *in)
{
	in->mobileion.perovskite_enabled=FALSE;
	in->mobileion.perovskite_dt=-1.0;
	in->mobileion.perovskite_steps=1;

	in->mobileion.ystart=-1.0;
	in->mobileion.ystop=-1.0;
	in->mobileion.ylen=-1.0;

	matrix_init(&(in->mobileion.mx));
	in->mobileion.error=-1.0;
	in->mobileion.Nion_mul=-1.0;
	in->mobileion.perovskite_conv=FALSE;
	in->mobileion.Nion_mul=FALSE;
	in->mobileion.full_newton=FALSE;

}

int perovskite_enabled(struct device *in)
{
return in->mobileion.perovskite_enabled;
}

void perovskite_dump(struct simulation *sim,struct device *in)
{
	struct dimensions *dim=&in->ns.dim;
	struct newton_state *ns=&in->ns;

	struct stat st = {0};

	char out_dir[PATH_MAX];
	//join_path(2,out_dir,get_output_path(in),"equilibrium");
	join_path(1,out_dir,get_output_path(in));

	if (stat(out_dir, &st) == -1)
	{
		mkdir(out_dir, 0700);
	}

	struct dat_file buf;
	dat_file_init(&buf);
	char name[200];
	int band=0;
	int i=0;

	if (buffer_set_file_name(sim,in,&buf,"Nion.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("Ion density - position"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.y_label,"Ion Density");
		strcpy(buf.x_units,"nm");
		strcpy(buf.y_units,"$m^{-3}$");
		dat_file_add_zxy_data(sim,&buf,dim, in->Nion);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

}

long double get_perovskite_error(struct device *in,long double *b)
{
long double tot=0.0;
int i;
	struct matrix *mx=&(in->mobileion.mx);
for (i=0;i<in->mobileion.ylen;i++)
{
	tot+=gfabs(mx->b[i]);

}
//tot/=1e25;
return tot;
}

void perovskite_free(struct simulation *sim,struct device *in)
{
	struct matrix *mx=&(in->mobileion.mx);

	if (in->mobileion.perovskite_enabled==TRUE)
	{
		matrix_free(sim,mx);
		perovskite_init(sim,in);
		printf_log(sim,"%s\n",_("Ion free"));

	}
}

void perovskite_cpy(struct simulation *sim,struct perovskite *out,struct perovskite *in)
{

	out->perovskite_enabled=in->perovskite_enabled;
	out->perovskite_dt=in->perovskite_dt;
	out->perovskite_steps=in->perovskite_steps;

	out->ystart=in->ystart;
	out->ystop=in->ystop;
	out->ylen=in->ylen;

	matrix_init(&(out->mx));
	out->error=in->error;

	out->Nion_mul=in->Nion_mul;
	out->perovskite_conv=in->perovskite_conv;
	out->Nion_mul=in->Nion_mul;
	out->full_newton=in->full_newton;
}

void perovskite_solve_time_step(struct simulation *sim,struct device *in, int z, int x)
{
int i;
int ittr=0;
int quit=FALSE;
struct dimensions *dim=&in->ns.dim;
struct newton_state *ns=&in->ns;
struct matrix *mx=&(in->mobileion.mx);
long double clamp_temp=300.0;
FILE *file;


	in->mobileion.perovskite_conv=FALSE;
	do
	{
		perovskite_build_matrix(sim,in,&in->mobileion,z,x);

		matrix_solve(sim,&(in->msm),mx);


		for (i=0;i<in->mobileion.ylen;i++)
		{

			long double update;

			//update=b[i]/(1.0+fabs(b[i]/0.01/(kb/Q)));
			//printf("r%Lf\n",mx->b[i]);
			update=mx->b[i];
			in->Nion[z][x][in->mobileion.ystart+i]+=update;//(1.0+gfabs(update/(in->electrical_clamp)/(clamp_temp*kb/Qe)));
		}

		in->mobileion.error=get_perovskite_error(in,mx->b);
		printf_log(sim,"%s Nion=%Le %Le\n",_("Ion convergence="),three_d_avg(in, in->Nion),in->mobileion.error);
		perovskite_dump(sim,in);

		file=fopena(in->output_path,"ion_converge.dat","a");
		if (file!=NULL)
		{
			fprintf(file,"%Le\n",in->mobileion.error);
			fclose(file);
		}
		ittr++;


		if (ittr>200)
		{
			quit=TRUE;
		}

		if (ittr>4)
		{
			if ((in->mobileion.error<min_perovskite_error))
			{
				quit=TRUE;
			}
		}

		if ((ittr==2)&&(in->mobileion.error<min_perovskite_error))
		{
			in->mobileion.perovskite_conv=TRUE;
		}

	}while(quit==FALSE);
	//getchar();
	//time_store(sim,in);

	//in->time+=in->mobileion.perovskite_dt;

	in->odes+=dim->ylen;
}

int solve_perovskite(struct simulation *sim,struct device *in, int z, int x)
{

if (in->mobileion.perovskite_enabled==TRUE)
{

	//printf_log(sim,"%s\n",_("Ion diffusion"));

	int i;
	int y;

	//int pos_max_ittr=250;
	//char tunits[100];

	//for (i=0;i<in->mobileion.perovskite_steps;i++)
	//{
		perovskite_solve_time_step(sim,in, z, x);
		//time_with_units(tunits,in->time);
		//printf_log(sim,"%s=%Le (%s) %Le\n",_("Ion time"),tunits,in->mobileion.error);
		//plot_now(sim,in,"perovskite.plot");
		//getchar();
	//}

}

return 0;
}


