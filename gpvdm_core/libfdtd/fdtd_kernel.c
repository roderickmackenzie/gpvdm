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

/** @file fdtd_kernal.c
	@brief Setup the OpenCL kernels.
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


void fdtd_opencl_kernel_init(struct simulation *sim, struct fdtd_data *data)
{
	#ifdef use_open_cl
	cl_int error;
	//cal_E********************************
	data->cal_E=clCreateKernel(data->prog, "cal_E", &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Can not make E kernel %d\n",error);
		exit(0);
	}

	error=clSetKernelArg(data->cal_E, 0, sizeof(cl_mem), &(data->ggEx));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 1, sizeof(cl_mem), &(data->ggEy));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 2, sizeof(cl_mem), &(data->ggEz));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 3, sizeof(cl_mem), &(data->ggHx));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 4, sizeof(cl_mem), &(data->ggHy));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 5, sizeof(cl_mem), &(data->ggHz));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 6, sizeof(cl_mem), &(data->ggEx_last));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 7, sizeof(cl_mem), &(data->ggEy_last));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 8, sizeof(cl_mem), &(data->ggEz_last));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 9, sizeof(cl_mem), &(data->ggHx_last));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 10, sizeof(cl_mem), &(data->ggHy_last));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 11, sizeof(cl_mem), &(data->ggHz_last));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 12, sizeof(cl_mem), &(data->ggepsilon_r));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 13, sizeof(cl_mem), &(data->ggy));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	error=clSetKernelArg(data->cal_E, 14, sizeof(cl_mem), &(data->ggC));
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}
	//cal_E********************************



	data->update_E=clCreateKernel(data->prog, "update_E", &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Can not make E kernel\n");
		exit(0);
	}

	data->cal_H=clCreateKernel(data->prog, "cal_H", &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Can not make H kernel\n");
		exit(0);
	}

	data->update_H=clCreateKernel(data->prog, "update_H", &error);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Can not make E kernel\n");
		exit(0);
	}


	clSetKernelArg(data->update_E, 0, sizeof(cl_mem), &(data->ggEx));
	clSetKernelArg(data->update_E, 1, sizeof(cl_mem), &(data->ggEy));
	clSetKernelArg(data->update_E, 2, sizeof(cl_mem), &(data->ggEz));

	clSetKernelArg(data->update_E, 3, sizeof(cl_mem), &(data->ggHx));
	clSetKernelArg(data->update_E, 4, sizeof(cl_mem), &(data->ggHy));
	clSetKernelArg(data->update_E, 5, sizeof(cl_mem), &(data->ggHz));

	clSetKernelArg(data->update_E, 6, sizeof(cl_mem), &(data->ggEx_last));
	clSetKernelArg(data->update_E, 7, sizeof(cl_mem), &(data->ggEy_last));
	clSetKernelArg(data->update_E, 8, sizeof(cl_mem), &(data->ggEz_last));

	clSetKernelArg(data->update_E, 9, sizeof(cl_mem), &(data->ggHx_last));
	clSetKernelArg(data->update_E, 10, sizeof(cl_mem), &(data->ggHy_last));
	clSetKernelArg(data->update_E, 11, sizeof(cl_mem), &(data->ggHz_last));

	clSetKernelArg(data->update_E, 12, sizeof(cl_mem), &(data->ggepsilon_r));
	clSetKernelArg(data->update_E, 13, sizeof(cl_mem), &(data->ggy));
	error=clSetKernelArg(data->update_E, 14, sizeof(cl_mem), &(data->ggC));

	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	clSetKernelArg(data->cal_H, 0, sizeof(cl_mem), &(data->ggEx));
	clSetKernelArg(data->cal_H, 1, sizeof(cl_mem), &(data->ggEy));
	clSetKernelArg(data->cal_H, 2, sizeof(cl_mem), &(data->ggEz));

	clSetKernelArg(data->cal_H, 3, sizeof(cl_mem), &(data->ggHx));
	clSetKernelArg(data->cal_H, 4, sizeof(cl_mem), &(data->ggHy));
	clSetKernelArg(data->cal_H, 5, sizeof(cl_mem), &(data->ggHz));

	clSetKernelArg(data->cal_H, 6, sizeof(cl_mem), &(data->ggEx_last));
	clSetKernelArg(data->cal_H, 7, sizeof(cl_mem), &(data->ggEy_last));
	clSetKernelArg(data->cal_H, 8, sizeof(cl_mem), &(data->ggEz_last));

	clSetKernelArg(data->cal_H, 9, sizeof(cl_mem), &(data->ggHx_last));
	clSetKernelArg(data->cal_H, 10, sizeof(cl_mem), &(data->ggHy_last));
	clSetKernelArg(data->cal_H, 11, sizeof(cl_mem), &(data->ggHz_last));

	clSetKernelArg(data->cal_H, 12, sizeof(cl_mem), &(data->ggepsilon_r));
	clSetKernelArg(data->cal_H, 13, sizeof(cl_mem), &(data->ggy));
	error=clSetKernelArg(data->cal_H, 14, sizeof(cl_mem), &(data->ggC));

	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	clSetKernelArg(data->update_H, 0, sizeof(cl_mem), &(data->ggEx));
	clSetKernelArg(data->update_H, 1, sizeof(cl_mem), &(data->ggEy));
	clSetKernelArg(data->update_H, 2, sizeof(cl_mem), &(data->ggEz));

	clSetKernelArg(data->update_H, 3, sizeof(cl_mem), &(data->ggHx));
	clSetKernelArg(data->update_H, 4, sizeof(cl_mem), &(data->ggHy));
	clSetKernelArg(data->update_H, 5, sizeof(cl_mem), &(data->ggHz));

	clSetKernelArg(data->update_H, 6, sizeof(cl_mem), &(data->ggEx_last));
	clSetKernelArg(data->update_H, 7, sizeof(cl_mem), &(data->ggEy_last));
	clSetKernelArg(data->update_H, 8, sizeof(cl_mem), &(data->ggEz_last));

	clSetKernelArg(data->update_H, 9, sizeof(cl_mem), &(data->ggHx_last));
	clSetKernelArg(data->update_H, 10, sizeof(cl_mem), &(data->ggHy_last));
	clSetKernelArg(data->update_H, 11, sizeof(cl_mem), &(data->ggHz_last));

	clSetKernelArg(data->update_H, 12, sizeof(cl_mem), &(data->ggepsilon_r));
	clSetKernelArg(data->update_H, 13, sizeof(cl_mem), &(data->ggy));
	error=clSetKernelArg(data->update_H, 14, sizeof(cl_mem), &(data->ggC));

	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}
	#endif

}
