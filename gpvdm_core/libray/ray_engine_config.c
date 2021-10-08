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

void ray_read_config(struct simulation *sim,struct image *my_image,struct world *w,struct json_obj *json_config)
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

	int d;
	struct detector *det;
	for (d=0;d<w->detectors;d++)
	{
		det=&(w->det[d]);
		if (det->viewpoint_enabled==TRUE)
		{
			my_image->viewpoint_dim.xlen=det->viewpoint_nx;
			my_image->viewpoint_dim.zlen=det->viewpoint_nz;
			my_image->viewpoint_dim.ylen=my_image->ray_wavelength_points;
			break;
		}
	}
}


