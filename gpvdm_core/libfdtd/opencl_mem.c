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
