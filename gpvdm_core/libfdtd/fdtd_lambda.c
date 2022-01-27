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

/** @file fdtd_lambda.c
	@brief Runs the fdtd code over multiple wavelengths.
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
#include <device.h>
#include <fdtd.h>
#include <lang.h>
#include <gui_hooks.h>
#include <dat_file.h>
#include <cal_path.h>
#include <device_fun.h>
#include "vec.h"

void fdtd_solve_all_lambda(struct simulation *sim,struct device *dev,struct fdtd_data *data)
{
	struct dimensions *dim=&(data->dim);
	struct dat_file buf;
	dat_file_init(&buf);

	struct math_xy escape;
	inter_init(sim,&escape);

	int i=0;
	float lambda=data->lambda_start;
	int steps=dim->llen;
	char send_data[200];

	printf("%d\n",steps);

	float dl=(data->lambda_stop-data->lambda_start)/((float)steps);
	for (i=0;i<steps;i++)
	{
		fdtd_solve_lambda(sim,data,dev,lambda);

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
	dat_file_add_xy_data(sim,&buf,escape.x, escape.data, escape.len);
	buffer_dump_path(sim,get_output_path(dev),"escape.dat",&buf);
	buffer_free(&buf);


	inter_free(&escape);
}

void fdtd_solve_lambda(struct simulation *sim,struct fdtd_data *data,struct device *dev,float lambda)
{
	char send_data[200];

	fdtd_set_lambda(sim,data,dev,lambda);

	sprintf(send_data,"text:%s %.0f nm",_("Running FDTD simulation @"),lambda*1e9);

	gui_send_data(sim,gui_sub,send_data);

	do
	{
		fdtd_solve_step(sim,data,dev);


		if ((data->step%100)==0)
		{
			if (data->use_gpu==TRUE)
			{
				fdtd_opencl_pull_data(sim,data);
			}

			sprintf(send_data,"percent:%Lf",(long double)data->step/data->max_ittr);
			gui_send_data(sim,gui_sub,send_data);

			sprintf(send_data,"step=%d/%d time=%e s\n",data->step,data->max_ittr,data->time);
			gui_send_data(sim,gui_sub,send_data);

			printf_log(sim,send_data);
		}

		fdtd_dump(sim,get_output_path(dev),dev,data);
		poll_gui(sim);

		data->step++;

		/*if (fdtd_test_conv(sim,data)!=0)
		{
			printf("break\n");
			break;
		}*/

		if (data->time>data->max_time)
		{
			break;
		}

	}while(data->step<data->max_ittr);

}
