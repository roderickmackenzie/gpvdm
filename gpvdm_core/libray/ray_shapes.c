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

/** @file ray_shapes.c
	@brief Basic shapes for ray tracing
*/

void add_triangle(struct world *w, double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,int obect_uid,int edge)
{
	struct triangle tri;

	triangle_init(&tri);
	vec_set(&(tri.xy0),x0,y0,z0);
	vec_set(&(tri.xy1),x1,y1,z1);
	vec_set(&(tri.xy2),x2,y2,z2);

	tri.object_uid=obect_uid;
	tri.object_type=edge;

	triangles_add_triangle(&(w->obj[obect_uid].tri), &tri);

	w->triangles++;
}

struct object *ray_add_object(struct device *dev,struct triangles *tri)
{
	//btm
	int i;
	struct world *w=&(dev->w);
	object_init(&(w->obj[w->objects]));
	object_malloc(&(w->obj[w->objects]));
	w->obj[w->objects].uid=w->objects;

	for (i=0;i<tri->len;i++)
	{
		add_triangle(w,
						tri->data[i].xy0.x,tri->data[i].xy0.y,tri->data[i].xy0.z,
						tri->data[i].xy1.x,tri->data[i].xy1.y,tri->data[i].xy1.z,
						tri->data[i].xy2.x,tri->data[i].xy2.y,tri->data[i].xy2.z,
						w->objects,
						tri->data[i].object_type);
	}


	object_cal_min_max(&(w->obj[w->objects]));

	triangles_cal_edges(&(w->obj[w->objects].tri));

	w->objects++;

	return &(w->obj[w->objects-1]);
}

struct object *add_box(struct device *dev, double x0,double y0,double z0,double dx,double dy,double dz,int object_type)
{
	struct world *w=&(dev->w);
	object_init(&(w->obj[w->objects]));
	object_malloc(&(w->obj[w->objects]));
	w->obj[w->objects].uid=w->objects;

	//btm
	add_triangle(w,
					x0,y0,z0,
					x0+dx,y0,
					z0,x0,y0,z0+dz,
													w->objects,object_type);
	add_triangle(w,
					x0+dx	,	y0,z0,
					x0+dx	,	y0,z0+dz,
					x0   	,	y0,z0+dz,
													w->objects,object_type);

	//top
	add_triangle(w,
					x0		,y0+dy	,	z0		,
					x0+dx	,y0+dy	,	z0		,
					x0		,y0+dy	,	z0+dz	,
													w->objects,object_type);
	add_triangle(w,
					x0+dx	,y0+dy	,z0			,
					x0+dx	,y0+dy	,z0+dz		,
					x0   	,y0+dy	,z0+dz		,
													w->objects,object_type);

	//left
	add_triangle(w,
					x0		,y0		,z0			,
					x0		,y0+dy	,z0			,
					x0   	,y0		,z0+dz		,
													w->objects,object_type);

	add_triangle(w,
					x0		,y0+dy	,z0			,
					x0		,y0+dy	,z0+dz		,
					x0   	,y0		,z0+dz		,
													w->objects,object_type);

	//right
	add_triangle(w,
					x0+dx		,y0		,z0			,
					x0+dx		,y0+dy	,z0			,
					x0+dx   	,y0		,z0+dz		,
													w->objects,object_type);

	add_triangle(w,
					x0+dx	,y0+dy	,z0			,
					x0+dx	,y0+dy	,z0+dz		,
					x0+dx  	,y0		,z0+dz		,
													w->objects,object_type);

	//front
	add_triangle(w,
					x0			,y0		,z0		,
					x0+dx		,y0		,z0		,
					x0   		,y0+dy	,z0		,
													w->objects,object_type);

	add_triangle(w,
					x0			,y0+dy	,z0		,
					x0+dx		,y0+dy	,z0		,
					x0+dx   	,y0		,z0		,
													w->objects,object_type);

	//back
	add_triangle(w,
					x0			,y0		,z0+dz		,
					x0+dx		,y0		,z0+dz		,
					x0   		,y0+dy	,z0+dz		,
													w->objects,object_type);

	add_triangle(w,
					x0			,y0+dy	,z0+dz		,
					x0+dx		,y0+dy	,z0+dz		,
					x0+dx   	,y0		,z0+dz		,
													w->objects,object_type);

	object_cal_min_max(&(w->obj[w->objects]));

	triangles_cal_edges(&(w->obj[w->objects].tri));

	w->objects++;
	return &(w->obj[w->objects-1]);
}

struct object *add_plane(struct world *w,double x0,double y0,double z0,double dx,double dz,int object_type)
{
	object_init(&(w->obj[w->objects]));
	object_malloc(&(w->obj[w->objects]));
	w->obj[w->objects].uid=w->objects;

	//btm
	add_triangle(w,
					x0,y0,z0,
					x0+dx,y0,
					z0,x0,y0,z0+dz,
													w->objects,object_type);
	add_triangle(w,
					x0+dx	,	y0,z0,
					x0+dx	,	y0,z0+dz,
					x0   	,	y0,z0+dz,
													w->objects,object_type);

	object_cal_min_max(&(w->obj[w->objects]));

	triangles_cal_edges(&(w->obj[w->objects].tri));

	w->objects++;
	return &(w->obj[w->objects-1]);
}

