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
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <ray_fun.h>
#include <triangle.h>
#include <triangles.h>

/** @file objects.c
	@brief Basic object manipulation
*/

void object_cal_min_max(struct object *obj)
{
	triangles_find_min(&(obj->min),&(obj->tri));
	triangles_find_max(&(obj->max),&(obj->tri));
}

void object_init(struct object *obj)
{
	//btm
	triangles_init(&(obj->tri));
	obj->epi_layer=-1;
	obj->s=NULL;
	obj->n=NULL;
	obj->alpha=NULL;

}

void object_malloc(struct object *obj)
{
	triangles_malloc(&(obj->tri));
}

void object_nalpha_malloc(struct object *obj,int l_max)
{
	int l=0;
	obj->n=malloc(sizeof(double)*l_max);
	obj->alpha=malloc(sizeof(double)*l_max);

	for (l=0;l<l_max;l++)
	{
		obj->n[l]=1.0;
		obj->alpha[l]=1e-3;
	}
}


void object_nalpha_free(struct object *obj)
{
	free(obj->n);
	free(obj->alpha);
	obj->n=NULL;
	obj->alpha=NULL;
}

void object_free(struct object *obj)
{
	triangles_free(&(obj->tri));
	obj->epi_layer=-1;
	obj->s=NULL;
	free(obj->n);
	free(obj->alpha);
	obj->n=NULL;
	obj->alpha=NULL;
}


double object_get_min_y(struct object *obj)
{
	int i=0;
	double min=1e9;//ray_tri_get_min_y(&(in->tri[0]));
	double min_new=min;
	struct triangle *tri;
	for (i=0;i<obj->tri.len;i++)
	{
		tri=&(obj->tri.data[i]);
		min_new=triangle_get_min_y(tri);
		if (min_new<min)
		{
			min=min_new;
		}
	}

return min;
}

void object_flip_y_axis(struct object *obj)
{
	int i=0;
	double y=object_get_min_y(obj);
	struct triangle *tri;

	for (i=0;i<obj->tri.len;i++)
	{
		tri=&(obj->tri.data[i]);
		ray_tri_flip_y_axis(tri,y);
	}

}

void object_sub_y(struct object *obj,double y)
{
	int i=0;
	struct triangle *tri;
	for (i=0;i<obj->tri.len;i++)
	{
		tri=&(obj->tri.data[i]);
		ray_tri_sub_y(tri,y);
	}

}

void object_add_y(struct object *obj,double y)
{
	int i=0;
	struct triangle *tri;
	for (i=0;i<obj->tri.len;i++)
	{
		tri=&(obj->tri.data[i]);
		ray_tri_add_y(tri,y);
	}

}
