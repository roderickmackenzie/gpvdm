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
#include <inp.h>
#include <util.h>
#include <triangles.h>

/** @file ray_config.c
	@brief Read the config file for the ray tracer
*/

void ray_read_config(struct simulation *sim,struct image *my_image)
{
	struct inp_file inp;
	char temp[200];

	inp_init(sim,&inp);
	inp_load_from_path(sim,&inp,get_input_path(sim),"ray.inp");

	inp_check(sim,&inp,1.0);


	inp_search_int(sim,&inp,&(my_image->ray_wavelength_points),"#ray_wavelength_points");

	inp_search_int(sim,&inp,&(my_image->escape_bins),"#ray_escape_bins");

	inp_search_double(sim,&inp,&(my_image->ray_xsrc),"#ray_xsrc");
	inp_search_double(sim,&inp,&(my_image->ray_ysrc),"#ray_ysrc");
	//inp_search_double(sim,&inp,&(my_image->ray_zsrc),"#ray_zsrc");

	inp_search_int(sim,&inp,&(my_image->theta_steps),"#ray_theta_steps");
	inp_search_double(sim,&inp,&(my_image->ray_theta_start),"#ray_theta_start");
	inp_search_double(sim,&inp,&(my_image->ray_theta_stop),"#ray_theta_stop");

	inp_search_int(sim,&inp,&(my_image->phi_steps),"#ray_phi_steps");
	inp_search_double(sim,&inp,&(my_image->ray_phi_start),"#ray_phi_start");
	inp_search_double(sim,&inp,&(my_image->ray_phi_stop),"#ray_phi_stop");

	inp_search_double(sim,&inp,&(my_image->ray_lambda_start),"#ray_lambda_start");
	inp_search_double(sim,&inp,&(my_image->ray_lambda_stop),"#ray_lambda_stop");

	inp_search_string(sim,&inp,temp,"#ray_auto_wavelength_range");
	my_image->ray_auto_wavelength_range=english_to_bin(sim,temp);

	inp_search_string(sim,&inp,temp,"#ray_auto_run");
	my_image->ray_auto_run=english_to_bin(sim,temp);

	inp_search_string(sim,&inp,temp,"#ray_emission_source");
	my_image->ray_emission_source=english_to_bin(sim,temp);

	inp_free(sim,&inp);

	ray_read_viewpoint(sim,my_image);
}


