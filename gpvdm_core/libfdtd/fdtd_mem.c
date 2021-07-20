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

#include "vec.h"

void fdtd_memcpy(struct fdtd_data *data, float ***out, float ***in)
{
	int z=0;
	int x=0;

	for (z = 0; z < data->zlen; z++)
	{

		for (x = 0; x < data->xlen; x++)
		{
			memcpy ( out[z][x], in[z][x], sizeof(float)*data->ylen );
		}
	}


}

void fdtd_set_3d_float(struct fdtd_data *in, float ***var, float value)
{
	int x=0;
	int y=0;
	int z=0;

	for (z = 0; z < in->zlen; z++)
	{

		for (x = 0; x < in->xlen; x++)
		{
			for (y = 0; y < in->ylen; y++)
			{
				var[z][x][y]=value;
			}
		}
	}


}

void fdtd_free_3d_float(struct fdtd_data *in, float ***var)
{
	int z=0;
	int x=0;

	for (z = 0; z < in->zlen; z++)
	{

		for (x = 0; x < in->xlen; x++)
		{
			free(var[z][x]);
		}
		free(var[z]);
	}

	free(var);

}

void fdtd_3d_malloc_float(struct fdtd_data *in, float * (***var))
{
	int z=0;
	int x=0;


	*var = (float ***) malloc(in->zlen * sizeof(float **));

	for (z = 0; z < in->zlen; z++)
	{
		(*var)[z] = (float **) malloc(in->xlen * sizeof(float*));
		for (x = 0; x < in->xlen; x++)
		{
			(*var)[z][x] = (float *) malloc(in->ylen * sizeof(float));
			memset((*var)[z][x], 0, in->ylen * sizeof(float));
		}
	}

}

void fdtd_get_mem(struct simulation *sim, struct fdtd_data *data)
{
	fdtd_3d_malloc_float(data, &data->Ex);
	fdtd_3d_malloc_float(data, &data->Ey);
	fdtd_3d_malloc_float(data, &data->Ez);

	fdtd_3d_malloc_float(data, &data->Hx);
	fdtd_3d_malloc_float(data, &data->Hy);
	fdtd_3d_malloc_float(data, &data->Hz);

	fdtd_3d_malloc_float(data, &data->Ex_last);
	fdtd_3d_malloc_float(data, &data->Ey_last);
	fdtd_3d_malloc_float(data, &data->Ez_last);

	fdtd_3d_malloc_float(data, &data->Ex_last_last);
	fdtd_3d_malloc_float(data, &data->Ey_last_last);
	fdtd_3d_malloc_float(data, &data->Ez_last_last);

	fdtd_3d_malloc_float(data, &data->Hx_last);
	fdtd_3d_malloc_float(data, &data->Hy_last);
	fdtd_3d_malloc_float(data, &data->Hz_last);

	fdtd_3d_malloc_float(data, &data->epsilon_r);
	fdtd_3d_malloc_float(data, &data->sigma);


	data->gEx=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gEy=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gEz=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gHx=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gHy=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gHz=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gEx_last=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gEy_last=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gEz_last=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gHx_last=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gHy_last=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gHz_last=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);
	data->gepsilon_r=(float *)malloc(sizeof(float)*data->zlen*data->xlen*data->ylen);

	data->gy=(float *)malloc(sizeof(float)*data->ylen);	//I have no idea what this does

	data->z_mesh=malloc(sizeof(float)*data->zlen);
	data->x_mesh=malloc(sizeof(float)*data->xlen);
	data->y_mesh=malloc(sizeof(float)*data->ylen);

	if (data->use_gpu==TRUE)
	{
		fdtd_opencl_get_mem(sim,data);
	}

	data->layer=(int *)malloc(sizeof(int)*data->ylen);

}

void fdtd_free_all(struct simulation *sim, struct fdtd_data *data)
{

	fdtd_free_3d_float(data, data->Ex);
	fdtd_free_3d_float(data, data->Ey);
	fdtd_free_3d_float(data, data->Ez);

	fdtd_free_3d_float(data, data->Hx);
	fdtd_free_3d_float(data, data->Hy);
	fdtd_free_3d_float(data, data->Hz);

	fdtd_free_3d_float(data, data->Ex_last);
	fdtd_free_3d_float(data, data->Ey_last);
	fdtd_free_3d_float(data, data->Ez_last);

	fdtd_free_3d_float(data, data->Hx_last);
	fdtd_free_3d_float(data, data->Hy_last);
	fdtd_free_3d_float(data, data->Hz_last);

	fdtd_free_3d_float(data, data->epsilon_r);
	fdtd_free_3d_float(data, data->sigma);

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

	free(data->z_mesh);
	free(data->x_mesh);
	free(data->y_mesh);

	if (data->gnuplot!=NULL)
	{
		fprintf(data->gnuplot, "exit\n");
		fflush(data->gnuplot);
		pclose(data->gnuplot);
	}



	if (data->use_gpu==TRUE)
	{
		fdtd_opencl_freemem(sim, data);
	}

	free(data->layer);
}
