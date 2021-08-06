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

#include <stdio.h>
#include <ray.h>
#include <ray_fun.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <device.h>
#include <json.h>
#include <util.h>
#include <triangles.h>

/** @file ray_config.c
	@brief Read the config file for the ray tracer
*/

void ray_read_config(struct simulation *sim,struct image *my_image,struct json_obj *json_config)
{
	struct json_obj *json_ray;

	if (json_config==NULL)
	{
		ewe(sim,"json_config null\n");
	}

	json_ray=json_obj_find(json_config, "config");
	if (json_ray==NULL)
	{
		ewe(sim,"Object config not found\n");
	}

	json_get_int(sim, json_ray, &(my_image->ray_wavelength_points),"ray_wavelength_points");

	json_get_int(sim, json_ray, &(my_image->escape_bins),"ray_escape_bins");

	json_get_double(sim, json_ray, &(my_image->ray_lambda_start),"ray_lambda_start");
	json_get_double(sim, json_ray, &(my_image->ray_lambda_stop),"ray_lambda_stop");

	json_get_english(sim, json_ray,&(my_image->ray_auto_wavelength_range),"ray_auto_wavelength_range");

	json_get_english(sim, json_ray, &(my_image->ray_auto_run),"ray_auto_run");

	ray_read_viewpoint(sim,my_image, json_config);
}


