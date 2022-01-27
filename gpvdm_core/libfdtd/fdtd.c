// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solardevs.
// The model can simulate OLEDs, Perovskite devs, and OFETs.
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

/** @file fdtd.c
	@brief Main FDTD interface
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
#include <json.h>
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

int do_fdtd(struct simulation *sim,struct device *dev)
{
	struct json_obj *json_config;
	printf_log(sim,"**************************\n");
	printf_log(sim,"*       FDTD module      *\n");
	printf_log(sim,"**************************\n");

	FILE * f;
	f=fopen("conv.dat","w");
	fclose(f);

	struct fdtd_data data;

	fdtd_init(&data);

	json_config=json_find_sim_struct(sim, &(dev->config),dev->simmode);
	fdtd_load_config(sim,&data,json_config);


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

	fdtd_mesh(sim,&data,dev);


	if (data.use_gpu==TRUE)
	{
		fdtd_opencl_push_to_gpu(sim,&data);
		fdtd_opencl_write_ctrl_data(sim,&data);
	}

	fdtd_solve_all_lambda(sim,dev,&data);

	fdtd_free_all(sim,&data);

	printf("Exit\n");

return 0;

}

