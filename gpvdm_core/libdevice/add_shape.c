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

/** @file add_shape_to_world.c
	@brief Add a shape to the image
*/

void device_add_shape_to_world(struct simulation *sim,struct device *dev,struct shape *s)
{
	int x=0;
	int z=0;

	double x_pos;
	double y_pos;
	double z_pos;

	struct object *obj;
	struct triangles tri;
	struct vec v;

	char name[200];


	for (x=0;x<s->nx;x++)
	{
		for (z=0;z<s->nz;z++)
		{
			triangles_init(&tri);
			triangles_malloc(&tri);

			x_pos=s->x0+(s->dx+s->dx_padding)*(double)x;
			y_pos=s->y0+s->dy_padding;
			//printf("%Le %s\n",s->dy_padding,s->name);
			//getchar();
			z_pos=s->z0+(s->dz+s->dz_padding)*(double)z;

			//printf("%p\n",&(s->tri));
			triangles_cpy(&tri,&(s->tri));

			triangles_find_min(&v,&tri);

			triangles_sub_vec(&tri,&v);

			triangles_find_max(&v,&tri);

			triangles_div_vec(&tri,&v);

			if (s->flip_y==TRUE)
			{
				v.x=1.0;
				v.y=-1.0;
				v.z=1.0;

				triangles_mul_vec(&tri,&v);

				//v.x=0.0;
				//v.y=1.0;
				//v.z=0.0;

				//triangles_add_vec(&tri,&v);
			}

			v.x=s->dx;
			v.y=s->dy;
			v.z=s->dz;

			triangles_mul_vec(&tri,&v);

			v.x=x_pos;
			v.y=y_pos;
			v.z=z_pos;

			triangles_add_vec(&tri,&v);

			triangles_set_object_type(&tri,RAY_OBJECT);

			obj=ray_add_object(dev,&tri);
			sprintf(name,"%s-(%d,%d)",s->name,x,z);
			strcpy(obj->name,name);

			obj->s=s;

			triangles_free(&tri);

		}
	}

}


