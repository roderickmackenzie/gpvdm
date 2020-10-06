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


/** @file light_memory.c
	@brief Deals with memory allocation for the light model.
*/

#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "config.h"
#include "util.h"
#include "lang.h"
#include "log.h"
#include <light_fun.h>
#include <matrix.h>
#include <memory.h>
#include <device_fun.h>
#include <triangles.h>

	#include <dlfcn.h>

static int unused __attribute__((unused));


void light_malloc(struct simulation *sim,struct light *li)
{
printf_log(sim,"alloc: light_memory\n");
	int l=0;
	int w=0;
	struct dim_light *dim=&(li->dim);
	struct matrix *mx;

	dim_light_malloc(dim);

	//long double zxyl
	malloc_light_zxyl_long_double(dim,&(li->Ep));
	malloc_light_zxyl_long_double(dim,&(li->Epz));
	malloc_light_zxyl_long_double(dim,&(li->En));
	malloc_light_zxyl_long_double(dim,&(li->Enz));
	malloc_light_zxyl_long_double(dim,&(li->n));
	malloc_light_zxyl_long_double(dim,&(li->alpha0));
	malloc_light_zxyl_long_double(dim,&(li->alpha));
	malloc_light_zxyl_long_double(dim,&(li->photons));
	malloc_light_zxyl_long_double(dim,&(li->photons_asb));
	malloc_light_zxyl_long_double(dim,&(li->pointing_vector));
	malloc_light_zxyl_long_double(dim,&(li->E_tot_r));
	malloc_light_zxyl_long_double(dim,&(li->E_tot_i));
	malloc_light_zxyl_long_double(dim,&(li->H));


	//long double zxy
	malloc_light_zxy_long_double(dim,&(li->Gn));
	malloc_light_zxy_long_double(dim,&(li->Gp));
	malloc_light_zxy_long_double(dim,&(li->Htot));
	malloc_light_zxy_long_double(dim,&(li->photons_tot));

	//long double complex
	malloc_light_zxyl_long_double_complex(dim,&(li->t));
	malloc_light_zxyl_long_double_complex(dim,&(li->r));
	malloc_light_zxyl_long_double_complex(dim,&(li->nbar));

	//zxy_p_object
	malloc_light_zxy_p_object(dim, &(li->obj));

	malloc_light_l_long_double(dim,&(li->sun_E));
	malloc_light_l_long_double(dim,&(li->sun));
	malloc_light_l_long_double(dim,&(li->sun_norm));
	malloc_light_l_long_double(dim,&(li->sun_photons));
	malloc_light_l_long_double(dim,&(li->reflect));
	malloc_light_l_long_double(dim,&(li->transmit));
	malloc_light_l_long_double(dim,&(li->extract_eff));
	malloc_light_l_long_double(dim,&(li->filter));

	for (l=0;l<dim->llen;l++)
	{
		li->extract_eff[l]=1.0;
	}

	//sort out the matrix
	

	malloc_1d((void**)&(li->mx),sim->server.worker_max, sizeof(struct matrix));
	malloc_1d((void**)&(li->msm),sim->server.worker_max, sizeof(struct matrix_solver_memory));
	for (w=0;w<sim->server.worker_max;w++)
	{
		matrix_solver_memory_init(&(li->msm[w]));
		mx=&(li->mx[w]);
		matrix_init(mx);
		mx->nz=0.0;
		mx->nz+=dim->ylen;
		mx->nz+=dim->ylen-1;
		mx->nz+=dim->ylen;

		mx->nz+=dim->ylen-1;
		mx->nz+=dim->ylen; //t
		mx->nz+=dim->ylen-1;
		//li->N+=1;
		mx->M=dim->ylen+dim->ylen;
		mx->complex_matrix=TRUE;
		matrix_malloc(sim,mx);	
	}

}

