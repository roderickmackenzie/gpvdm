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

/** @file fdtd.c
	@brief Main FDTD interface
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
#include <device.h>
#include <fdtd.h>

#include "vec.h"

int dump_number=0;

/*void my_handler(int s)
{
free_all();
           printf_log(sim,"Caught signal %d\n",s);
           exit(1); 

}*/

int do_fdtd(struct simulation *sim,struct device *cell)
{
	printf_log(sim,"**************************\n");
	printf_log(sim,"*       FDTD module      *\n");
	printf_log(sim,"**************************\n");

	FILE * f;
	f=fopen("conv.dat","w");
	fclose(f);

	struct fdtd_data data;

	fdtd_init(&data);

	fdtd_load_config(sim,&data);


	if (data.use_gpu==TRUE)
	{
		opencl_init(sim,&data);
	}

	fdtd_get_mem(sim, &data);

	if (data.use_gpu==TRUE)
	{
		fdtd_opencl_load_code(sim,&data);
		fdtd_opencl_kernel_init(sim, &data);
	}

	fdtd_mesh(sim,&data,cell);

	if (data.plot==1)
	{
		data.gnuplot = popen("gnuplot","w");
		fprintf(data.gnuplot, "set terminal x11 title 'Solarsim' \n");
		fflush(data.gnuplot);
	}


	if (data.use_gpu==TRUE)
	{
		fdtd_opencl_push_to_gpu(sim,&data);
		fdtd_opencl_write_ctrl_data(sim,&data);
	}

	fdtd_solve_all_lambda(sim,cell,&data);

	fdtd_free_all(sim,&data);

	printf("Exit\n");

return 0;

}

