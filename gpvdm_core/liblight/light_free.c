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



void light_free_memory(struct simulation *sim,struct light *li)
{
	int w;
	struct dim_light *dim=&(li->dim);
	struct matrix *mx;

	//long double zxyl
	free_light_zxyl_float(dim,&(li->Ep));
	free_light_zxyl_float(dim,&(li->Epz));
	free_light_zxyl_float(dim,&(li->En));
	free_light_zxyl_float(dim,&(li->Enz));
	free_light_zxyl_float(dim,&(li->n));
	free_light_zxyl_float(dim,&(li->alpha0));
	free_light_zxyl_float(dim,&(li->alpha));
	free_light_zxyl_double(dim,&(li->photons));
	free_light_zxyl_double(dim,&(li->photons_asb));
	//free_light_zxyl_float(dim,&(li->pointing_vector));
	//free_light_zxyl_float(dim,&(li->E_tot_r));
	//free_light_zxyl_float(dim,&(li->E_tot_i));
	free_light_zxyl_float(dim,&(li->H));

	//long double zxy
	free_light_zxy_long_double(dim,&(li->Gn));
	free_light_zxy_long_double(dim,&(li->Gp));
	free_light_zxy_long_double(dim,&(li->Htot));
	free_light_zxy_long_double(dim,&(li->photons_tot));

	//long double complex
	free_light_zxyl_float_complex(dim,&(li->t));
	free_light_zxyl_float_complex(dim,&(li->r));
	free_light_zxyl_float_complex(dim,&(li->nbar));

	//zxy_p_object
	free_light_zxy_p_object(dim, &(li->obj));


	//1D arrays
	free_light_l_long_double(dim,&(li->transmit));
	free_light_l_long_double(dim,&(li->reflect));

	//Input spectra
	light_src_free(sim, &(li->light_src_y0));
	light_src_free(sim, &(li->light_src_y1));

	free_light_l_long_double(dim,&(li->sun_y0));
	free_light_l_long_double(dim,&(li->sun_y1));
	free_light_l_long_double(dim,&(li->sun_photons_y0));
	free_light_l_long_double(dim,&(li->sun_photons_y1));
	free_light_l_long_double(dim,&(li->sun_E_y0));
	free_light_l_long_double(dim,&(li->sun_E_y1));

	//config
	free_light_l_long_double(dim,&(li->extract_eff));

/////////////

	if (li->mx!=NULL)
	{
		for (w=0;w<li->worker_max;w++)
		{
			mx=&(li->mx[w]);
			matrix_free(sim,mx);
		}

		free_1d((void **)&(li->mx),sizeof(struct matrix));
	}

	if (li->msm!=NULL)
	{
		for (w=0;w<li->worker_max;w++)
		{
			matrix_solver_memory_free(sim,&(li->msm[w]));
		}

		free_1d((void **)&(li->msm),sizeof(struct matrix_solver_memory));
	}

	dim_light_free(dim);

	if (strcmp(li->light_profile,"box")!=0)
	{
		triangles_free((&(li->light_profile_tri)));
	}

	if (li->lib_handle!=NULL)
	{
		dlclose(li->lib_handle);
	}


	light_init(sim,li);
	
	//printf_log(sim,_("Freeing memory from the optical model\n"));
}
