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
	struct dimensions *dim=&(thermal->dim);
	struct matrix *mx=&(thermal->mx);

	//dim_heat_malloc(dim);

	//long double zxy
	malloc_zxy_long_double(dim,&(thermal->Tl));
	malloc_zxy_long_double(dim,&(thermal->Te));
	malloc_zxy_long_double(dim,&(thermal->Th));

	malloc_zxy_long_double(dim,&(thermal->Hl));

	malloc_zxy_long_double(dim,&(thermal->H_optical));
	malloc_zxy_long_double(dim,&(thermal->H_recombination));
	malloc_zxy_long_double(dim,&(thermal->H_joule));
	malloc_zxy_long_double(dim,&(thermal->H_parasitic));

	malloc_zxy_long_double(dim,&(thermal->He));
	malloc_zxy_long_double(dim,&(thermal->Hh));

	malloc_zxy_long_double(dim,&(thermal->kl));
	malloc_zxy_long_double(dim,&(thermal->ke));
	malloc_zxy_long_double(dim,&(thermal->kh));

	//zxy_p_object
	malloc_zxy_p_object(dim, &(thermal->obj));

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

