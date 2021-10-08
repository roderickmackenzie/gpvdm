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
#include <dat_file.h>
#include <triangles.h>
#include <device_fun.h>

/** @file device_world_stats.c
	@brief Dump stats about each object in the world
*/


void device_world_stats(struct simulation *sim,struct device *dev)
{
	struct world *w=&(dev->w);

	if (sim->fitting!=FIT_NOT_FITTING)
	{
		return;
	}

	FILE *out;
	int o=0;
	double Rq=0.0;
	double Ra=0.0;
	double Rp=0.0;
	struct object *obj;
	struct vec min;
	struct vec max;

	out=fopena(get_output_path(dev),"object_stats.json","w");
	for (o=0;o<w->objects;o++)
	{
		obj=&(w->obj[o]);
		Rq=triangle_Rq(sim,&(obj->tri));
		if (Rq!=-1.0)
		{
			fprintf(out,"#object_name%d\n%s\n",o,obj->name);
			fprintf(out,"#Rq%d\n%le\n",o,Rq);

			triangles_find_min(&min,&(obj->tri));
			triangles_find_max(&max,&(obj->tri));

			Ra=triangle_Ra(sim,&(obj->tri))-min.y;
			fprintf(out,"#Ra%d\n%le\n",o,Ra);

			Rp=max.y-min.y;
			fprintf(out,"#Rp%d\n%le\n",o,Rp);

		}
	}

	fprintf(out,"#end\n");

	fclose(out);


}

