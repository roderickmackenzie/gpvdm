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

/** @file world_init.c
	@brief Initalize the world
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <cal_path.h>
#include <world.h>
#include <world_struct.h>
#include <detector.h>


void world_free(struct simulation *sim,struct world *w)
{
	int l;

	if (w->items>0)
	{
		for (l=0;l<w->items;l++)
		{
			shape_free(sim,&(w->shapes[l]));
		}
		free(w->shapes);
	}

	world_objects_free(sim,w);

	detectors_free(sim,w);

	world_init(sim,w);
}

void world_objects_free(struct simulation *sim,struct world *w)
{
	int i;
	//world made from triangles
	if (w->objects>0)
	{
		for (i=0;i<w->objects;i++)
		{
			object_free(&(w->obj[i]));
		}

		free(w->obj);
	}

	w->obj=NULL;
	w->objects=0;
}
