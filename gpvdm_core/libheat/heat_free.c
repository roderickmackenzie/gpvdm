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


/** @file free.c
	@brief Free for heat structure
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
#include <mesh.h>

static int unused __attribute__((unused));

void heat_free_memory(struct simulation *sim,struct heat *thermal)
{
	struct dimensions *dim=&(thermal->dim);
	struct matrix *mx=&(thermal->mx);

	if (thermal->newton_enable_external_thermal==TRUE)
	{
		//long double zxy
		free_zxy_long_double(dim,&(thermal->Tl));
		free_zxy_long_double(dim,&(thermal->Te));
		free_zxy_long_double(dim,&(thermal->Th));

		free_zxy_long_double(dim,&(thermal->Hl));
		free_zxy_long_double(dim,&(thermal->H_optical));
		free_zxy_long_double(dim,&(thermal->H_recombination));
		free_zxy_long_double(dim,&(thermal->H_joule));
		free_zxy_long_double(dim,&(thermal->H_parasitic));

		free_zxy_long_double(dim,&(thermal->He));
		free_zxy_long_double(dim,&(thermal->Hh));

		free_zxy_long_double(dim,&(thermal->kl));
		free_zxy_long_double(dim,&(thermal->ke));
		free_zxy_long_double(dim,&(thermal->kh));

		//zxy_p_object
		free_zxy_p_object(dim, &(thermal->obj));

		//long double

		matrix_free(sim,mx);
		mesh_obj_free(sim,&(thermal->mesh_data));
		dim_free(dim);

		printf_log(sim,_("Freeing memory from the heat model\n"));
	}
}
