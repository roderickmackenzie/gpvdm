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

/** @file fdtd_config.c
	@brief Loads the fdtd config file.
*/

#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json.h>
#include <sim.h>
#include <log.h>
#include <fdtd.h>

#include "vec.h"


void fdtd_load_config(struct simulation *sim, struct fdtd_data *data,struct json_obj *json_obj)
{

	json_get_float(sim,json_obj, &(data->lambda),"lambda");
	json_get_english(sim,json_obj, &(data->use_gpu),"use_gpu");

	json_get_int(sim,json_obj, &(data->max_ittr),"max_ittr");

	json_get_int(sim,json_obj, &(data->zlen),"zlen");

	data->zlen=1.0;
	printf_log(sim,"zlen=%d\n",data->zlen);

	json_get_int(sim,json_obj, &(data->xlen),"xlen");
	printf_log(sim,"xlen=%d\n",data->xlen);

	json_get_int(sim,json_obj, &(data->ylen),"ylen");
	printf_log(sim,"ylen=%d\n",data->ylen);

	//inp_search_float(sim,&inp,&(data->zsize),"#zsize");
	data->zsize=1.0;
	printf_log(sim,"zsize=%e\n",data->zsize);

	json_get_float(sim,json_obj, &(data->xsize),"xsize");
	printf_log(sim,"xsize=%e\n",data->xsize);

	json_get_int(sim,json_obj, &(data->lam_jmax),"lam_jmax");
	printf_log(sim,"lam_jmax=%d\n",data->lam_jmax);
	json_get_int(sim,json_obj, &(data->plot),"plot");

	json_get_float(sim,json_obj, &(data->dt),"dt");
	printf_log(sim,"dt=%e\n",data->dt);

	json_get_float(sim,json_obj, &(data->lambda_start),"fdtd_lambda_start");
	printf_log(sim,"lambda_start=%e\n",data->lambda_start);

	json_get_float(sim,json_obj, &(data->lambda_stop),"fdtd_lambda_stop");
	printf_log(sim,"lambda_stop=%e\n",data->lambda_stop);

	json_get_int(sim,json_obj, &(data->lambda_points),"fdtd_lambda_points");
	printf_log(sim,"lambda_points=%e\n",data->lambda_points);

	data->src_start=10e-9;
	data->src_stop=20e-9;

}
