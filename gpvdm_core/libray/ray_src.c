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

void ray_check_if_needed(struct simulation *sim,struct device *dev)
{
	int count;
	struct image *my_image=&(dev->my_image);
	my_image->enabled=FALSE;
	count=ray_src_add_emitters(sim,dev, TRUE);
	if (count>0)
	{	
		my_image->enabled=TRUE;
	}
}

void ray_src_dump(struct simulation *sim,struct device *dev)
{
	int i;
	struct ray_src *src;
	struct image *my_image=&(dev->my_image);
	//printf_log(sim,"x\ty\tz\ttheta_steps\ttheta_start\ttheta_stop\tphi_steps\tphi_start\tphi_stop\n");
	printf_log(sim,"%-14s%-14s%-14s","x","y","z");
	printf_log(sim,"%-14s%-14s%-14s","theta_steps","theta_start","theta_stop");
	printf_log(sim,"%-14s%-14s%-14s","phi_steps","phi_start","phi_stop");
	printf_log(sim,"\n");

	for (i=0;i<my_image->n_ray_srcs;i++)
	{
		src=&(my_image->ray_srcs[i]);
	
		printf_log(sim,"%-14le%-14le%-14le",src->x,src->y,src->z);
		printf_log(sim,"%-14d%-14le%-14le",src->theta_steps, src->theta_start,src->theta_stop);
		printf_log(sim,"%-14d%-14le%-14le",src->phi_steps, src->phi_start,src->phi_stop);
		printf_log(sim,"\n");
//le\t%d\t%le\t%le\t%d\t%le\t%le\n",,src->phi_steps, src->phi_start,	src->phi_stop);
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
	struct epitaxy* epitaxy = &(dev->my_epitaxy);
	double xlen=dev->xlen;
	double zlen=dev->zlen;

	double start_z=zlen/2.0;
	double start_x=xlen/2.0;

	for (l=0;l<epitaxy->layers;l++)
	{
		layer=&(epitaxy->layer[l]);

		if (layer->pl_enabled==TRUE)
		{
			if (just_count==FALSE)
			{
				raysrc=&(my_image->ray_srcs[emitters]);

				raysrc->x=start_x;
				raysrc->y=layer->y_start+layer->width/2.0;
				raysrc->z=start_z;

				raysrc->theta_steps=layer->theta_steps;
				raysrc->theta_start=layer->theta_start;
				raysrc->theta_stop=layer->theta_stop;

				raysrc->phi_steps=layer->phi_steps;
				raysrc->phi_start=layer->phi_start;
				raysrc->phi_stop=layer->phi_stop;
				raysrc->epi_layer=l;
				raysrc->emission_source=layer->emission_source;
				raysrc->light=-1;
			}
			emitters++;
		}
	}

	for (i=0;i<dev->lights.nlight_sources;i++)
	{
		lightsrc=&(dev->lights.light_sources[i]);
		if (strcmp(lightsrc->illuminate_from,"xyz")==0)
		{
			if (just_count==FALSE)
			{
				raysrc=&(my_image->ray_srcs[emitters]);

				raysrc->x=lightsrc->x0;
				raysrc->y=lightsrc->y0;
				raysrc->z=lightsrc->z0;

				raysrc->theta_steps=lightsrc->theta_steps;
				raysrc->theta_start=lightsrc->theta_start;
				raysrc->theta_stop=lightsrc->theta_stop;

				raysrc->phi_steps=lightsrc->phi_steps;
				raysrc->phi_start=lightsrc->phi_start;
				raysrc->phi_stop=lightsrc->phi_stop;
				raysrc->epi_layer=-1;
				raysrc->emission_source=-1;
				raysrc->light=i;
			}
			emitters++;
		}
	}

	return emitters;

}


