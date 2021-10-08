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

/** @file world_cpy.c
	@brief Copy the world
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <cal_path.h>
#include <world.h>
#include <world_struct.h>
#include <triangles.h>
#include <triangle.h>
#include <detector.h>

void world_cpy(struct simulation *sim,struct world *out,struct world *in)
{
	int i;

	out->items=in->items;
	if (in->items>0)
	{
		out->shapes=(struct shape *)malloc(sizeof(struct shape)*in->items);

		for (i=0;i<in->items;i++)
		{
			shape_init(sim,&(out->shapes[i]));
			shape_cpy(sim,&(out->shapes[i]),&(in->shapes[i]));
		}
	}

	out->world_automatic_size=in->world_automatic_size;

	out->x0=in->x0;
	out->x1=in->x1;
	out->y0=in->y0;
	out->y1=in->y1;
	out->z0=in->z0;
	out->z1=in->z1;

	out->obj=NULL;			//We are not going to copy this
	out->objects=0;
	out->triangles=0;

	detectors_cpy(sim,out,in);
}


