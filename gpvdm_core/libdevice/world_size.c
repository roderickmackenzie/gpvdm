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

/** @file world_load.c
	@brief Load the world
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <string.h>
#include <util.h>
#include <cal_path.h>
#include <triangles.h>
#include <triangle.h>
#include <world_struct.h>
#include <world.h>

void world_size(struct simulation *sim,struct vec *min,struct vec *max,struct world *w, struct device *dev)
{
	int l=0;
	long double dx=0.0;
	long double dy=0.0;
	long double dz=0.0;

	struct shape *s;
	struct epi_layer *layer;
	struct epitaxy *epi=&(dev->my_epitaxy);
	vec_set(min,1e6,1e6,1e6);
	vec_set(max,-1e6,-1e6,-1e6);

	for (l=0;l<w->items;l++)
	{
		s=&(w->shapes[l]);
		shape_cal_min_max(sim,min,max,s);
	}

	for (l=0;l<epi->layers;l++)
	{
		s=&(w->shapes[l]);
		layer=&(epi->layer[l]);
		s=&(layer->s);
		shape_cal_min_max(sim,min,max,s);
	}

	dx=max->x-min->x;
	dy=max->y-min->y;
	dz=max->z-min->z;

	min->x=min->x-dx*(w->x0-1.0);
	max->x=max->x+dx*(w->x1-1.0);

	min->y=min->y-dy*(w->y0-1.0);
	max->y=max->y+dy*(w->y1-1.0);

	min->z=min->z-dz*(w->z0-1.0);
	max->z=max->z+dz*(w->z1-1.0);
}


