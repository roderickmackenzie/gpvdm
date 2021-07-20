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


/** @file heat_malloc.c
	@brief Malloc for heat structure
*/

#include <stdlib.h>
#include "util.h"
#include "gpvdm_const.h"
#include "heat.h"
#include "device.h"
#include "config.h"
#include "util.h"
#include "lang.h"
#include "log.h"
#include <light_fun.h>
#include <matrix.h>
#include <memory.h>
#include <device_fun.h>


static int unused __attribute__((unused));



void heat_malloc(struct simulation *sim,struct heat *thermal)
{
	printf_log(sim,"alloc: heat_memory\n");
	struct dim_heat *dim=&(thermal->dim);
	struct matrix *mx=&(thermal->mx);

	//dim_heat_malloc(dim);

	//long double zxy
	malloc_heat_zxy_long_double(dim,&(thermal->Tl));
	malloc_heat_zxy_long_double(dim,&(thermal->Te));
	malloc_heat_zxy_long_double(dim,&(thermal->Th));

	malloc_heat_zxy_long_double(dim,&(thermal->Hl));

	malloc_heat_zxy_long_double(dim,&(thermal->H_optical));
	malloc_heat_zxy_long_double(dim,&(thermal->H_recombination));
	malloc_heat_zxy_long_double(dim,&(thermal->H_joule));
	malloc_heat_zxy_long_double(dim,&(thermal->H_parasitic));

	malloc_heat_zxy_long_double(dim,&(thermal->He));
	malloc_heat_zxy_long_double(dim,&(thermal->Hh));

	malloc_heat_zxy_long_double(dim,&(thermal->kl));
	malloc_heat_zxy_long_double(dim,&(thermal->ke));
	malloc_heat_zxy_long_double(dim,&(thermal->kh));

	//zxy_p_object
	malloc_heat_zxy_p_object(dim, &(thermal->obj));

	mx->nz=0;
	mx->M=0;
	if (thermal->thermal_model_type==THERMAL_HYDRODYNAMIC)
	{
		if (thermal->thermal_l==TRUE)
		{
			mx->nz+=(dim->ylen*3-2)*dim->xlen*dim->zlen;			//dTldTl
			mx->M+=dim->ylen*dim->xlen*dim->zlen;
		}

		if (thermal->thermal_e==TRUE)
		{
			mx->nz+=dim->ylen*3-2;			//dTedTe
			mx->nz+=dim->ylen;				//dTedTl
			mx->nz+=dim->ylen;				//dTldTe
			mx->M+=dim->ylen;
		}

		if (thermal->thermal_h==TRUE)
		{
			mx->nz+=dim->ylen*3-2;			//dThdTh
			mx->nz+=dim->ylen;				//dThdTl
			mx->nz+=dim->ylen;				//dTldTh
			mx->M+=dim->ylen;
		}
	}else
	{
		if (thermal->thermal_l==TRUE)
		{
			mx->nz+=dim->zlen*dim->xlen*(dim->ylen*3-2);//*dim->zlen;			//dTldTl
			if (dim->xlen>1)
			{
				mx->nz+=dim->zlen*(dim->xlen-1)*dim->ylen;		//left diagonal
				mx->nz+=dim->zlen*(dim->xlen-1)*dim->ylen;		//right diagonal
			}

			if (dim->zlen>1)
			{
				mx->nz+=(dim->zlen-1)*dim->xlen*dim->ylen;		//left diagonal
				mx->nz+=(dim->zlen-1)*dim->xlen*dim->ylen;		//right diagonal
			}

			mx->M+=dim->zlen*dim->xlen*dim->ylen;//*dim->zlen;
		}
	}

	mx->complex_matrix=FALSE;
	if (mx->M!=0)
	{
		matrix_malloc(sim,mx);
	}
}

