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

/** @file fdtd_mem.c
	@brief Memory allocation for FDTD.
*/

#include <enabled_libs.h>
#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <inp.h>
#include <sim.h>
#include <log.h>
#include <fdtd.h>
#include <memory.h>
#include "vec.h"

void fdtd_get_mem(struct simulation *sim, struct fdtd_data *data)
{
	struct dimensions *dim=&(data->dim);
	dim_light_malloc(dim);
	malloc_light_zxy_float(dim, &data->Ex);
	malloc_light_zxy_float(dim, &data->Ey);
	malloc_light_zxy_float(dim, &data->Ez);

	malloc_light_zxy_float(dim, &data->Hx);
	malloc_light_zxy_float(dim, &data->Hy);
	malloc_light_zxy_float(dim, &data->Hz);

	malloc_light_zxy_float(dim, &data->Ex_last);
	malloc_light_zxy_float(dim, &data->Ey_last);
	malloc_light_zxy_float(dim, &data->Ez_last);

	malloc_light_zxy_float(dim, &data->Ex_last_last);
	malloc_light_zxy_float(dim, &data->Ey_last_last);
	malloc_light_zxy_float(dim, &data->Ez_last_last);

	malloc_light_zxy_float(dim, &data->Hx_last);
	malloc_light_zxy_float(dim, &data->Hy_last);
	malloc_light_zxy_float(dim, &data->Hz_last);

	malloc_light_zxy_float(dim, &data->epsilon_r);
	malloc_light_zxy_float(dim, &data->sigma);

	malloc_light_zxy_p_object(dim, &(data->obj));

	data->gEx=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gEy=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gEz=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gHx=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gHy=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gHz=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gEx_last=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gEy_last=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gEz_last=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gHx_last=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gHy_last=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gHz_last=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);
	data->gepsilon_r=(float *)malloc(sizeof(float)*dim->zlen*dim->xlen*dim->ylen);

	data->gy=(float *)malloc(sizeof(float)*dim->ylen);	//I have no idea what this does


	if (data->use_gpu==TRUE)
	{
		fdtd_opencl_get_mem(sim,data);
	}


}

void fdtd_free_all(struct simulation *sim, struct fdtd_data *data)
{
	struct dimensions *dim=&(data->dim);

	free_light_zxy_float(dim, &data->Ex);
	free_light_zxy_float(dim, &data->Ey);
	free_light_zxy_float(dim, &data->Ez);

	free_light_zxy_float(dim, &data->Hx);
	free_light_zxy_float(dim, &data->Hy);
	free_light_zxy_float(dim, &data->Hz);

	free_light_zxy_float(dim, &data->Ex_last);
	free_light_zxy_float(dim, &data->Ey_last);
	free_light_zxy_float(dim, &data->Ez_last);

	free_light_zxy_float(dim, &data->Hx_last);
	free_light_zxy_float(dim, &data->Hy_last);
	free_light_zxy_float(dim, &data->Hz_last);

	free_light_zxy_float(dim, &data->epsilon_r);
	free_light_zxy_float(dim, &data->sigma);

	free(data->gEx);
	free(data->gEy);
	free(data->gEz);
	free(data->gHx);
	free(data->gHy);
	free(data->gHz);
	free(data->gEx_last);
	free(data->gEy_last);
	free(data->gEz_last);
	free(data->gHx_last);
	free(data->gHy_last);
	free(data->gHz_last);
	free(data->gepsilon_r);
	free(data->gy);


	if (data->use_gpu==TRUE)
	{
		fdtd_opencl_freemem(sim, data);
	}

	free_light_zxy_p_object(dim, &(data->obj));
	dim_light_free(dim);
}
