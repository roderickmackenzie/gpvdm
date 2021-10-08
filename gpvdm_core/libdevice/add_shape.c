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

	if (s->enabled==FALSE)
	{
		return;
	}

	for (x=0;x<s->nx;x++)
	{
		for (z=0;z<s->nz;z++)
		{
			triangles_init(&tri);
			triangles_malloc(&tri);

			x_pos=s->x0+(s->dx+s->dx_padding)*(double)x;
			y_pos=s->y0+s->dy_padding;
			//printf("%le %s\n",y_pos,s->name);
			z_pos=s->z0+(s->dz+s->dz_padding)*(double)z;

			//printf("%p\n",&(s->tri));
			//printf(">>>>>\n");
			//triangles_dump(&(s->tri));
			triangles_cpy(&tri,&(s->tri));
			triangles_find_min(&v,&tri);
			triangles_sub_vec(&tri,&v);
			triangles_find_max(&v,&tri);
			triangles_div_vec(&tri,&v);

			vec_set(&v,0.5, 0.5, 0.5);

			triangles_sub_vec(&tri,&v);
			triangles_rotate_x(&tri,(s->rotate_x/360.0)*2.0*M_PI);
			triangles_rotate_y(&tri,(s->rotate_y/360.0)*2.0*M_PI);
			triangles_add_vec(&tri,&v);


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


