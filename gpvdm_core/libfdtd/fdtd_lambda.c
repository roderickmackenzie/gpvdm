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

/** @file fdtd_lambda.c
	@brief Runs the fdtd code over multiple wavelengths.
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
#include <lang.h>
#include <gui_hooks.h>
#include <dat_file.h>
#include <cal_path.h>

#include "vec.h"

void fdtd_solve_all_lambda(struct simulation *sim,struct device *cell,struct fdtd_data *data)
{
	struct dat_file buf;
	buffer_init(&buf);

	struct math_xy escape;
	inter_init(sim,&escape);

	int i=0;
	float lambda=data->lambda_start;
	int steps=data->lambda_points;
	char send_data[200];

	printf("%d\n",steps);

	float dl=(data->lambda_stop-data->lambda_start)/((float)steps);
	for (i=0;i<steps;i++)
	{
		fdtd_solve_lambda(sim,data,cell,lambda);

		inter_append(&escape,lambda,data->escape*100.0);

		lambda=lambda+dl;
		printf("lambda= %.0f nm\n",lambda*1e9);

		sprintf(send_data,"percent:%f",(float)(i)/(float)(steps));

		gui_send_data(sim,gui_sub,send_data);

	}

	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1.0;
	buf.data_mul=1.0;
	sprintf(buf.title,"%s - %s",_("Photon escape probability"),_("Wavelength"));
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Wavelength"));
	strcpy(buf.data_label,_("Photon escape probability"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"%");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.logscale_z=0;
	buf.x=1;
	buf.y=escape.len;
	buf.z=1;
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,escape.x, escape.data, escape.len);
	buffer_dump_path(sim,get_output_path(sim),"escape.dat",&buf);
	buffer_free(&buf);


	inter_free(&escape);
}

void fdtd_solve_lambda(struct simulation *sim,struct fdtd_data *data,struct device *cell,float lambda)
{
	char send_data[200];

	fdtd_set_lambda(sim,data,cell,lambda);

	sprintf(send_data,"text:%s %.0f nm",_("Running FDTD simulation @"),lambda*1e9);

	gui_send_data(sim,gui_sub,send_data);

	do
	{
		fdtd_solve_step(sim,data);

		if ((data->step%200)==0)
		{
			if (data->use_gpu==TRUE)
			{
				fdtd_opencl_pull_data(sim,data);
			}

			fdtd_dump(sim,data);

			printf_log(sim,"plot! %ld\n",data->step);

			if (data->plot==1)
			{
				fprintf(data->gnuplot, "load 'Ex.plot'\n");
				fflush(data->gnuplot);
			}

		printf_log(sim,"%ld/%ld %e s\n",data->step,data->max_ittr,data->time);
		}
		//exit(0);

		data->step++;

		/*if (fdtd_test_conv(sim,data)!=0)
		{
			printf("break\n");
			break;
		}*/

	}while(data->step<data->max_ittr);

}
