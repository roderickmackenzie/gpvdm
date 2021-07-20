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
