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

static int unused __attribute__((unused));

void light_solve_and_update(struct simulation *sim,struct device *dev,struct light *li,long double laser_eff_in)
{

	struct dimensions *dim=&dev->ns.dim;

	li->laser_eff=laser_eff_in;

	if ((li->last_laser_eff!=li->laser_eff)||(li->last_Psun!=li->Psun)||(li->last_wavelength_laser!=li->laser_wavelength)||(li->force_update==TRUE))
	{
		light_solve_optical_problem(sim,dev,li);
		li->last_laser_eff=li->laser_eff;
		li->last_Psun=li->Psun;
		li->last_wavelength_laser=li->laser_wavelength;
	}

	if (li->laser_pos!=-1)
	{
		light_dump_1d(sim,li, li->laser_pos,"");
	}

	light_transfer_gen_rate_to_device(dev,li);

	if (li->flip_field==TRUE)
	{
		flip_zxy_long_double_y(sim, dim,dev->Gn);
		flip_zxy_long_double_y(sim, dim,dev->Gp);
	}

}

void light_solve_optical_problem(struct simulation *sim,struct device *dev,struct light *li)
{
	int l=0;
	long double Psun=0.0;
	struct dim_light *dim=&li->dim;

	Psun=li->Psun*gpow(10.0,-li->ND);

	light_set_sun_power(li,Psun,li->laser_eff);

	light_solve_all(sim,dev,li);

	light_cal_photon_density(sim,li,dev);

	for (l=0;l<dim->llen;l++)
	{
		light_dump_1d(sim,li, l,"");
	}

}

THREAD_FUNCTION thread_light_solve(void * in)
{
	int z;
	int x;
	int nw;
	//printf("yes\n");
	char temp[200];

	struct job *j=(struct job *)in;
	struct worker *my_worker=j->w;
	nw=my_worker->worker_n;
	//printf("nw=%d\n",nw);
	struct simulation *sim=(struct simulation *)j->sim;
	struct light *li=(struct light *)j->data0;
	struct device *cell=(struct device *)j->data1;
	struct dim_light *dim=&li->dim;
	int l=((int*)j->data2)[0];

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{

			
			memset_light_zxyl_long_double_y(dim, li->En,z,x,l,0.0);
			memset_light_zxyl_long_double_y(dim, li->Ep,z,x,l,0.0);
			memset_light_zxyl_long_double_y(dim, li->Enz,z,x,l,0.0);
			memset_light_zxyl_long_double_y(dim, li->Epz,z,x,l,0.0);

			if (li->sun_E[l]!=0.0)
			{
				if ((x==0)&&(z==0))
				{
					if (li->print_wavlengths==TRUE)
					{
						if (get_dump_status(sim,dump_optics)==TRUE)
						{
							sprintf(temp,"%s %Lf nm\n",_("Solve optical slice at"),dim->l[l]*1e9);
							waveprint(sim,temp,dim->l[l]*1e9);
						}
					}
				}
				//printf("here %d\n",li->finished_solveing);
				light_solve_lam_slice(sim,cell,li,z,x,l,nw);

				//printf("here %d\n",li->finished_solveing);
				//getchar();

				if (li->finished_solveing==TRUE)
				{
					break;
				}

			}//else
			//{

			//}
		}
	}

	j->data0=NULL;
	j->data1=NULL;
	server2_job_finished(sim,j);

	return 0;
}

void light_solve_all(struct simulation *sim,struct device *cell,struct light *li)
{
	int l=0;
	struct job j;

	int slices_solved=0;
	struct dim_light *dim=&li->dim;
	li->finished_solveing=FALSE;


	for (l=0;l<dim->llen;l++)
	{
		job_init(sim,&j);
		sprintf(j.name,"light-%d",l);
		j.fun=&thread_light_solve;
		j.sim=(void *)sim;

		j.data0=(void *)li;
		j.data1=(void *)cell;
		malloc_1d((void **)&(j.data2),1,sizeof(int));
		((int *)j.data2)[0]=l;

		server2_add_job(sim,&(sim->server),&j);
		slices_solved++;

	}

	//server2_dump_jobs(sim,&(sim->server));
	//int store=sim->server.worker_max;			//To make this multithreaded you need a fix to the matrix memory in the light solver
	//sim->server.worker_max=3;
	server2_run_until_done(sim,&(sim->server));
	//sim->server.worker_max=store;
	server2_free_finished_jobs(sim,&(sim->server));
	

	if (slices_solved==0)
	{
		printf_log(sim,_("It was dark I did not solve any slices\n"));
	}

}


int light_solve_lam_slice(struct simulation *sim, struct device *dev, struct light *li,int z, int x,int l, int w)
{
li->disable_cal_photon_density=FALSE;
return (*li->fn_solve_lam_slice)(sim,dev,li,z,x,l,w);
}
