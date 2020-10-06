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
#include <dat_file.h>
#include <triangles.h>

/** @file device_world_stats.c
	@brief Dump stats about each object in the world
*/

void device_objects_free(struct simulation *sim,struct device *dev)
{
	int i;
	//world made from triangles
	for (i=0;i<dev->objects;i++)
	{
		object_free(&(dev->obj[i]));
	}

	free(dev->obj);

	dev->obj=NULL;
	dev->objects=0;
}

void device_world_stats(struct simulation *sim,struct device *dev)
{

	FILE *out;
	int o=0;
	double Rq=0.0;
	double Ra=0.0;
	double Rp=0.0;
	struct object *obj;
	struct vec min;
	struct vec max;

	out=fopena(get_output_path(sim),"object_stats.dat","w");
	for (o=0;o<dev->objects;o++)
	{
		obj=&(dev->obj[o]);
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


//getchar();
}

