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

/** @file light_utils.c
	@brief Miscellaneous functions for the light model.
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
#include <server.h>
#include <device_fun.h>

static int unused __attribute__((unused));

void light_solve_and_update(struct simulation *sim,struct device *dev,struct light *li,long double laser_eff_in)
{
	int l=0;
	long double Psun=0.0;
	struct dimensions *dim=&li->dim;

	//struct dimensions *dim=&dev->ns.dim;

	li->laser_eff=laser_eff_in;
	//printf(">>%Le %Le\n",li->last_laser_eff,li->laser_eff);
	//printf(">%Le, %Le\n",li->last_Psun,li->Psun);
	//printf(">%d\n",(li->last_wavelength_laser!=li->laser_wavelength));
	//printf(">%d\n",li->force_update);

	if ((li->last_laser_eff!=li->laser_eff)||(li->last_Psun!=li->Psun)||(li->last_wavelength_laser!=li->laser_wavelength)||(li->force_update==TRUE))
	{

		memset_light_zxy_long_double(dim,li->Gn,0);
		memset_light_zxy_long_double(dim,li->Gp,0);
		memset_light_zxy_long_double(dim,li->Htot,0);
		memset_light_zxy_long_double(dim,li->photons_tot,0);

		memset_light_zxyl_float(dim,li->H,0);
		memset_light_zxyl_double(dim,li->photons_asb,0);
		memset_light_zxyl_double(dim,li->photons,0);

		Psun=li->Psun*gpow(10.0,-li->ND);

		light_set_sun_power(li,Psun,li->laser_eff);

		light_solve_all(sim,dev,li);
		light_cal_photon_density(sim,li,dev);

		for (l=0;l<dim->llen;l++)
		{
			light_dump_1d(sim,get_output_path(dev),li, l,"");
		}

		li->last_laser_eff=li->laser_eff;
		li->last_Psun=li->Psun;
		li->last_wavelength_laser=li->laser_wavelength;

	}

	if (li->laser_pos!=-1)
	{
		light_dump_1d(sim,get_output_path(dev),li, li->laser_pos,"");
	}
	light_transfer_gen_rate_to_device(sim, dev,li);

}


void restore_slice(struct simulation *sim, struct light *li, float *En, float *Ep, float *Enz,float *Epz,int x,int z,int l)
{
	int y;
	struct dimensions *dim=&li->dim;
	for (y=0;y<dim->ylen;y++)
	{
		li->En[z][x][y][l]=En[y];
		li->Ep[z][x][y][l]=Ep[y];
		li->Enz[z][x][y][l]=Enz[y];
		li->Epz[z][x][y][l]=Epz[y];
	}

}

void save_slice(struct simulation *sim, float *En, float *Ep, float *Enz,float *Epz,struct light *li,int z,int x,int l)
{
	int y;
	struct dimensions *dim=&li->dim;
	for (y=0;y<dim->ylen;y++)
	{
		En[y]=li->En[z][x][y][l];
		Ep[y]=li->Ep[z][x][y][l];
		Enz[y]=li->Enz[z][x][y][l];
		Epz[y]=li->Epz[z][x][y][l];
	}

}

float sum_layer(struct simulation *sim, struct light *li,int z,int x,int l)
{
int y;
float ret=0;
struct dimensions *dim=&li->dim;
for (y=0;y<dim->ylen;y++)
{
	ret+=li->alpha[z][x][y][l]+li->n[z][x][y][l];
}

return ret;
}

int light_zx_lambda_solver(struct simulation *sim, struct light *li, struct device *cell, int l, int nw)
{
	int z=0;
	int x=0;
	char temp[200];
	struct dimensions *dim=&li->dim;
	int all_finished=FALSE;

	if (strcmp(li->mode,"constant")==0)
	{
		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				light_solve_lam_slice(sim,cell,li,li->sun_E_y0,z,x,l,nw,FALSE);
			}
		}
		all_finished=TRUE;

		return all_finished;
	}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			if ((li->sun_E_y0[l]!=0.0)||(li->sun_E_y1[l]!=0.0))
			{
				if ((x==0)&&(z==0))
				{

					if (li->print_wavlengths==TRUE)
					{
						if (li->dump_verbosity>=0)
						{
							sprintf(temp,"%s %Lf nm\n",_("Solve optical slice at"),dim->l[l]*1e9);
							waveprint(sim,temp,dim->l[l]);
						}
					}
				}
			}

			if (li->sun_E_y0[l]!=0.0)
			{
				light_solve_lam_slice(sim,cell,li,li->sun_E_y0,z,x,l,nw,FALSE);
				light_cal_photon_density_y(sim,li,cell, z, x, l);
			}


			//for (y=0;y<dim->ylen;y++)
			//{
			//	printf("y0 %d %d %e\n",y,l,li->photons_asb[z][x][y][l]);
			//}

			//printf("here1 %d\n",l);
			//getchar();
			if (li->sun_E_y1[l]!=0.0)
			{
				light_solve_lam_slice(sim,cell,li,li->sun_E_y1,z,x,l,nw,TRUE);
				light_cal_photon_density_y(sim,li,cell, z, x, l);
			}

			//for (y=0;y<dim->ylen;y++)
			//{
			//	printf("y1 %d %d %e\n",y,l,li->photons_asb[z][x][y][l]);
			//}

			//printf("here2\n");
			//getchar();

			if (li->finished_solveing==TRUE)
			{
				break;
			}


		}
	}

	return all_finished;

}

THREAD_FUNCTION thread_light_solve(void * in)
{

	int nw;
	//printf("yes\n");
	struct job *j=(struct job *)in;
	struct worker *my_worker=j->w;
	nw=my_worker->worker_n;

	//printf("nw=%d\n",nw);
	struct simulation *sim=(struct simulation *)j->sim;
	struct light *li=(struct light *)j->data0;
	struct device *cell=(struct device *)j->data1;
	if (nw>li->worker_max)
	{
		ewe(sim,"Worker error\n");
	}

	int l=((int*)j->data2)[0];

	light_zx_lambda_solver(sim, li, cell, l,nw);

	j->data0=NULL;
	j->data1=NULL;
	server2_job_finished(sim,j);

	return 0;
}

void light_solve_all(struct simulation *sim,struct device *cell,struct light *li)
{
	struct dimensions *dim=&li->dim;
	memset_light_zxyl_float(dim, li->En,0.0);
	memset_light_zxyl_float(dim, li->Ep,0.0);
	memset_light_zxyl_float(dim, li->Enz,0.0);
	memset_light_zxyl_float(dim, li->Epz,0.0);
	memset_light_zxyl_float(dim, li->Epz,0.0);
	memset_light_zxyl_double(dim, li->photons,0.0);
	memset_light_zxyl_double(dim, li->photons_asb,0.0);
	memset_light_zxyl_float(dim, li->H,0.0);


	int l=0;
	struct job j;
	int batch_id=server2_get_next_batch_id(sim,&(sim->server));
	int ret=0;
	int slices_solved=0;

	li->finished_solveing=FALSE;

	if (((dim->zlen>1)||(dim->xlen>1))&&(sim->server.max_threads>1))
	{
		//server2_dump_jobs(sim,&(sim->server));
		//printf("here\n");
		for (l=0;l<dim->llen;l++)
		{
			job_init(sim,&j);
			sprintf(j.name,"light-%d",l);
			j.fun=&thread_light_solve;
			j.sim=(void *)sim;
			j.batch_id=batch_id;

			j.data0=(void *)li;
			j.data1=(void *)cell;
			malloc_1d((void **)&(j.data2),1,sizeof(int));
			((int *)j.data2)[0]=l;

			server2_add_job(sim,&(sim->server),&j);
			slices_solved++;

		}

		server2_run_until_done(sim,&(sim->server),batch_id);

		server2_free_finished_jobs(sim,&(sim->server),batch_id);
	}else
	{
		for (l=0;l<dim->llen;l++)
		{
			ret=light_zx_lambda_solver(sim, li, cell, l,0);
			if (ret==TRUE)
			{
				break;
			}
		}
	}

	/*if (slices_solved==0)
	{
		printf_log(sim,_("It was dark I did not solve any slices\n"));
	}*/

}


int light_solve_lam_slice(struct simulation *sim, struct device *dev, struct light *li,long double *sun_E,int z, int x,int l, int w,int flip_material)
{
	int ret=0;
	struct dimensions *dim=&li->dim;
	li->disable_cal_photon_density=FALSE;

	if (flip_material==TRUE)
	{
		//light_flip_y_float_complex(dim,li->r,z,x,l);
		//light_flip_y_float_complex(dim,li->t,z,x,l);
		//light_flip_y_float_complex(dim,li->nbar,z,x,l);

		light_flip_y_float(dim,li->n,z,x,l);
		light_flip_y_float(dim,li->alpha,z,x,l);
		light_calculate_complex_n_zxl(li, z, x, l);
	}


	ret= (*li->fn_solve_lam_slice)(sim,dev,li,sun_E,z,x,l,w);

	if (flip_material==TRUE)
	{

		light_flip_y_float(dim,li->n,z,x,l);
		light_flip_y_float(dim,li->alpha,z,x,l);
		light_calculate_complex_n_zxl(li, z, x, l);

		light_flip_y_float(dim,li->Ep,z,x,l);
		light_flip_y_float(dim,li->Epz,z,x,l);

		light_flip_y_float(dim,li->En,z,x,l);
		light_flip_y_float(dim,li->Enz,z,x,l);
	}

	return ret;
}
