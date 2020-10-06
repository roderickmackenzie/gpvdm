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

/** @file opencl_mem.c
	@brief Get some GPU memory.
*/

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

void fdtd_opencl_freemem(struct simulation *sim, struct fdtd_data *data)
{
	#ifdef use_open_cl
	printf_log(sim,"Freeingall\n");
	cl_int l_success;
	l_success=clReleaseMemObject(data->ggEx);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggEy);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggEz);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggHx);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggHy);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggHz);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggEx_last);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggEy_last);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggEz_last);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggHx_last);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggHy_last);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggHz_last);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggepsilon_r);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggy);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseMemObject(data->ggC);

	l_success=clReleaseProgram(data->prog);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseKernel(data->cal_E);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseKernel(data->cal_H);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseKernel(data->update_E);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	l_success=clReleaseKernel(data->update_H);
	if( l_success != CL_SUCCESS) printf_log(sim,"Can not free\n");

	clReleaseCommandQueue(data->cq);
	clReleaseContext(data->context);
	#endif
}

void fdtd_opencl_get_mem(struct simulation *sim, struct fdtd_data *data)
{
	#ifdef use_open_cl
	cl_int error;


	data->ggEx=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggEy=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggEz=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggHx=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggHy=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggHz=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggEx_last=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggEy_last=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggEz_last=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggHx_last=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggHy_last=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggHz_last=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggepsilon_r=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->zlen*data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggy=clCreateBuffer(data->context, CL_MEM_READ_WRITE, data->ylen*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}

	data->ggC=clCreateBuffer(data->context, CL_MEM_READ_WRITE, 17*sizeof(float), NULL, &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error Ex '%d'\n",error);
		exit(0);
	}
	#endif

}
