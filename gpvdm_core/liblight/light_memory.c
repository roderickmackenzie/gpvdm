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


/** @file light_memory.c
	@brief Deals with memory allocation for the light model.
*/

#include <enabled_libs.h>
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

	#define _GNU_SOURCE
	#include <dlfcn.h>

static int unused __attribute__((unused));


void light_malloc(struct simulation *sim,struct light *li)
{
printf_log(sim,"alloc: light_memory\n");

	int l=0;
	int w=0;
	struct dimensions *dim=&(li->dim);
	struct matrix *mx;

	dim_light_malloc(dim);

	//long double zxyl
	malloc_light_zxyl_float(dim,&(li->Ep));
	malloc_light_zxyl_float(dim,&(li->Epz));
	malloc_light_zxyl_float(dim,&(li->En));
	malloc_light_zxyl_float(dim,&(li->Enz));
	malloc_light_zxyl_float(dim,&(li->n));
	malloc_light_zxyl_float(dim,&(li->alpha0));
	malloc_light_zxyl_float(dim,&(li->alpha));
	malloc_light_zxyl_double(dim,&(li->photons));
	malloc_light_zxyl_double(dim,&(li->photons_asb));
	malloc_light_zxyl_float(dim,&(li->H));

	//long double zxy
	//printf("light\n");
	//getchar();
	malloc_light_zxy_long_double(dim,&(li->Gn));
	malloc_light_zxy_long_double(dim,&(li->Gp));
	malloc_light_zxy_long_double(dim,&(li->Htot));
	malloc_light_zxy_long_double(dim,&(li->photons_tot));

	//long double complex
	malloc_light_zxyl_float_complex(dim,&(li->t));
	malloc_light_zxyl_float_complex(dim,&(li->r));
	malloc_light_zxyl_float_complex(dim,&(li->nbar));

	//zxy_p_object
	malloc_light_zxy_p_object(dim, &(li->obj));

	//Input spectra
	malloc_light_l_long_double(dim,&(li->sun_y0));
	malloc_light_l_long_double(dim,&(li->sun_y1));
	malloc_light_l_long_double(dim,&(li->sun_photons_y0));
	malloc_light_l_long_double(dim,&(li->sun_photons_y1));

	//Input spectra no filter
	malloc_light_l_long_double(dim,&(li->sun_y0_no_filter));
	malloc_light_l_long_double(dim,&(li->sun_y1_no_filter));
	malloc_light_l_long_double(dim,&(li->sun_photons_y0_no_filter));
	malloc_light_l_long_double(dim,&(li->sun_photons_y1_no_filter));


	//Input field
	malloc_light_l_long_double(dim,&(li->sun_E_y0));
	malloc_light_l_long_double(dim,&(li->sun_E_y1));

	malloc_light_l_long_double(dim,&(li->reflect));
	malloc_light_l_long_double(dim,&(li->transmit));
	malloc_light_l_long_double(dim,&(li->extract_eff));
	//printf("%p 11\n",li);
	for (l=0;l<dim->llen;l++)
	{
		li->extract_eff[l]=1.0;
	}

	//sort out the matrix

	li->worker_max=sim->server.worker_max;
	if (li->worker_max>0)
	{
		malloc_1d((void**)&(li->mx),li->worker_max, sizeof(struct matrix));
		malloc_1d((void**)&(li->msm),li->worker_max, sizeof(struct matrix_solver_memory));
		for (w=0;w<li->worker_max;w++)
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
	printf_log(sim,"alloc: light_memory end\n");
}

