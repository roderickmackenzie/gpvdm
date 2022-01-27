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

/** @file light_mesh.c
	@brief Performs meshing for the light model.
*/

#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "lang.h"
#include "log.h"
#include "memory.h"
#include <light_fun.h>
#include <epitaxy.h>
#include <ray_fun.h>
#include <server.h>

static int unused __attribute__((unused));


void light_build_mesh(struct simulation *sim,struct light *li,struct dimensions *electrical_dim)
{
	int y=0;
	int x=0;
	int z=0;
	int l=0;

	long double pos=0.0;

	struct dimensions *dim=&(li->dim);

	printf_log(sim,"init: mesh\n");
	for (x=0;x<electrical_dim->xlen;x++)
	{
		dim->x[x]=electrical_dim->x[x];
	}

	for (z=0;z<electrical_dim->zlen;z++)
	{
		dim->z[z]=electrical_dim->z[z];
	}

	dim->dy=(epitaxy_get_optical_problem_stop(li->epi)-epitaxy_get_optical_problem_start(li->epi))/((long double)dim->ylen);

	//printf("%Le %Le\n",epitaxy_get_optical_problem_stop(li->epi),epitaxy_get_optical_problem_start(li->epi));
	//getchar();
	pos=epitaxy_get_optical_problem_start(li->epi);

	for (y=0;y<dim->ylen;y++)
	{
		dim->y[y]=pos;
		pos=pos+dim->dy;
		//printf("%Le %Le\n",pos,dim->dy);
		//getchar();
	}

	pos=li->lstart;
	for (l=0;l<dim->llen;l++)
	{
		dim->l[l]=pos;
		pos+=dim->dl;
	}

}

int light_build_obj_pointer_array_z(struct simulation *sim,struct device *dev,struct light *li,int z)
{
	int y;
	int x;

	struct vec v;

	struct dimensions *dim=&li->dim;

	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			v.z=dim->z[z];
			v.x=dim->x[x];
			v.y=dim->y[y];

			li->obj[z][x][y]=ray_obj_search_xyz(sim,dev,&v);

		}
	}

	return 0;
}

THREAD_FUNCTION thread_light_build_obj_pointer_array(void * in)
{
	int l;
	int y;
	int x;
	long double n=0.0;
	long double alpha=0.0;

	long double nc=0.0;
	long double kc=0.0;

	long double nr=0.0;
	long double kr=0.0;
	long double complex n0=0.0+0.0*I;
	long double complex n1=0.0+0.0*I;

	//int nw;
	struct vec v;

	struct job *j=(struct job *)in;
	//struct worker *my_worker=j->w;
	//nw=my_worker->worker_n;

	struct simulation *sim=(struct simulation *)j->sim;
	struct light *li=(struct light *)j->data0;
	struct device *dev=(struct device *)j->data1;
	struct dimensions *dim=&li->dim;
	struct object *obj_last=NULL;
	struct object *obj=NULL;
	struct shape *s;
	int z=j->data_int0;
	long double lam=0.0;

	for (l=0;l<dim->llen;l++)
	{
		lam=dim->l[l];

		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				if (l==0)
				{
					v.z=dim->z[z];
					v.x=dim->x[x];
					v.y=dim->y[y];

					obj=ray_obj_search_xyz(sim,dev,&v);
					li->obj[z][x][y]=obj;
				}else
				{
					obj=li->obj[z][x][y];
				}

				if (obj_last!=obj)
				{
					s=obj->s;
					n=inter_get_noend(&(s->n),lam);
					alpha=inter_get_noend(&(s->alpha),lam);
				}

				li->alpha[z][x][y][l]=alpha;
				li->alpha0[z][x][y][l]=alpha;
				li->n[z][x][y][l]=n;

				obj_last=obj;
				//printf("%le %le %le %s\n",v.z,v.x,v.y,li->obj[z][x][y]->epi_layer);
			}
		}
	}

	for (l=0;l<dim->llen;l++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{

				if (y==dim->ylen-1)
				{
					nr=li->n[z][x][y][l];
					kr=li->alpha[z][x][y][l]*(dim->l[l]/(PI*4.0));
				}else
				{
					nr=li->n[z][x][y+1][l];
					kr=li->alpha[z][x][y+1][l]*(dim->l[l]/(PI*4.0));
				}

				nc=li->n[z][x][y][l];
				kc=li->alpha[z][x][y][l]*(dim->l[l]/(PI*4.0));

				n0=nc-kc*I;
				n1=nr-kr*I;

				li->nbar[z][x][y][l]=n0;

				li->r[z][x][y][l]=(n0-n1)/(n0+n1);
				li->t[z][x][y][l]=(2.0*n0)/(n0+n1);
			}
		}

	}

	j->data0=NULL;
	j->data1=NULL;
	if (sim->server.max_threads>1)
	{
		server2_job_finished(sim,j);
	}

	return 0;
}

void light_build_obj_pointer_array(struct simulation *sim,struct light *li, struct device *dev)
{
	printf_log(sim,"Light: Building object pointer array\n");
	int z=0;
	struct job j;
	struct dimensions *dim=&(li->dim);
	int batch_id=server2_get_next_batch_id(sim,&(sim->server));
	for (z=0;z<dim->zlen;z++)
	{
			job_init(sim,&j);
			sprintf(j.name,"build_light_mesh-%d",z);
			j.fun=&thread_light_build_obj_pointer_array;
			j.sim=(void *)sim;
			j.batch_id=batch_id;

			j.data0=(void *)li;
			j.data1=(void *)dev;
			j.data_int0=z;
			if (sim->server.max_threads>1)
			{
				server2_add_job(sim,&(sim->server),&j);
			}else
			{
				thread_light_build_obj_pointer_array((void*)&j);
			}
	}

	if (sim->server.max_threads>1)
	{
		server2_run_until_done(sim,&(sim->server),batch_id);
		server2_free_finished_jobs(sim,&(sim->server),batch_id);
		server2_dump_jobs(sim,&(sim->server));
	}
}

void light_build_materials_arrays(struct simulation *sim,struct light *li, struct device *dev)
{

	struct dimensions *dim=&(li->dim);


	/*for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				obj=li->obj[z][x][y];
				printf("test %d %d %d %p\n",z,x,y,obj);
			}
		}
	}

	printf("here\n");*/
	light_build_obj_pointer_array(sim,li, dev);


	//light_calculate_complex_n(li);

	memset_light_zxyl_float(dim, li->En,0);
	memset_light_zxyl_float(dim, li->Ep,0);
	memset_light_zxyl_float(dim, li->Enz,0);
	memset_light_zxyl_float(dim, li->Epz,0);

	light_src_build_spectra_tot(sim,&(li->light_src_y0), li->lstart, li->lstop, dim->llen);
	light_src_build_spectra_tot(sim,&(li->light_src_y1), li->lstart, li->lstop, dim->llen);

}
