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
	if (w->world_automatic_size==FALSE)
	{
		min->x=w->x0;
		min->y=w->y0;
		min->z=w->z0;

		max->x=w->x1;
		max->y=w->y1;
		max->z=w->z1;
		return;
	}

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


