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
#include <memory.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <device_fun.h>


/** @file ray_build_scene.c
	@brief Set up the simulation window for the ray tracer
*/

int ray_src_dump(struct simulation *sim,struct device *dev)
{
	int i;
	struct ray_src *src;
	struct image *my_image=&(dev->my_image);
	printf_log(sim,"x\ty\tz\ttheta_steps\ttheta_start\ttheta_stop\tphi_steps\tphi_start\tphi_stop");
	for (i=0;i<my_image->n_ray_srcs;i++)
	{
		src=my_image->ray_srcs[i];
	
		printf_log(sim,"%Le\t%Le\t%Le\t%d\t%Le\t%Le\t%d\t%Le\t%Le\n",src->x,src->y,src->z,src->theta_steps, src->theta_start,src->theta_stop,src->phi_steps, src->phi_start,	src->phi_stop);
	}
}

int ray_src_add_emitters(struct simulation *sim,struct device *dev, int just_count)
{
	int l;
	int i;
	int emitters=0;
	struct light_src *lightsrc;
	struct ray_src *raysrc;
	struct epi_layer *layer;
	struct image *my_image=&(dev->my_image);
	double xlen=dev->xlen;
	double zlen=dev->zlen;

	double start_z=zlen/2.0;
	double start_x=xlen/2.0;

	for (l=0;l<my_epitaxy->layers;l++)
	{
		layer=&(my_epitaxy->layer[l]);

		if (layer->pl_enabled==TRUE)
		{
			if (just_count==FALSE)
			{
				raysrc=my_image->ray_srcs[emitters];

				raysrc->x=start_x;
				raysrc->y=layer->y_start+layer->width/2.0;
				raysrc->z=start_z;

				raysrc->theta_steps=layer->theta_steps;
				raysrc->ray_theta_start=layer->theta_start;
				raysrc->ray_theta_stop=layer->theta_stop;

				raysrc->phi_steps=layer->phi_steps;
				raysrc->ray_phi_start=layer->phi_start;
				raysrc->ray_phi_stop=layer->phi_stop;

			}
			emitters++;
		}
	}

	for (i=0;i<dev->lights.nlight_sources;i++)
	{
		lightsrc=dev->lights.light_sources[i];
		if (strcmp(lightsrc->illuminate_from,"xyz")==0)
		{
			if (just_count==FALSE)
			{
				raysrc=my_image->ray_srcs[emitters];

				raysrc->x=lightsrc->x0;
				raysrc->y=lightsrc->y0;
				raysrc->z=lightsrc->z0;

				raysrc->theta_steps=lightsrc->theta_steps;
				raysrc->ray_theta_start=lightsrc->theta_start;
				raysrc->ray_theta_stop=lightsrc->theta_stop;

				raysrc->phi_steps=lightsrc->phi_steps;
				raysrc->ray_phi_start=lightsrc->phi_start;
				raysrc->ray_phi_stop=lightsrc->phi_stop;
			}
			emitters++;
		}
	}

	return emitters;

}


