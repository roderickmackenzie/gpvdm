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

void add_triangle(struct device *dev, double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,int obect_uid,int edge)
{
	struct triangle tri;

	triangle_init(&tri);
	vec_set(&(tri.xy0),x0,y0,z0);
	vec_set(&(tri.xy1),x1,y1,z1);
	vec_set(&(tri.xy2),x2,y2,z2);

	tri.object_uid=obect_uid;
	tri.object_type=edge;

	triangles_add_triangle(&(dev->obj[obect_uid].tri), &tri);

	dev->triangles++;
}

struct object *ray_add_object(struct device *dev,struct triangles *tri)
{
	//btm
	int i;
	object_init(&(dev->obj[dev->objects]));
	object_malloc(&(dev->obj[dev->objects]));
	dev->obj[dev->objects].uid=dev->objects;

	for (i=0;i<tri->len;i++)
	{
		add_triangle(dev,
						tri->data[i].xy0.x,tri->data[i].xy0.y,tri->data[i].xy0.z,
						tri->data[i].xy1.x,tri->data[i].xy1.y,tri->data[i].xy1.z,
						tri->data[i].xy2.x,tri->data[i].xy2.y,tri->data[i].xy2.z,
						dev->objects,
						tri->data[i].object_type);
	}


	object_cal_min_max(&(dev->obj[dev->objects]));

	triangles_cal_edges(&(dev->obj[dev->objects].tri));

	dev->objects++;

	return &(dev->obj[dev->objects-1]);
}

struct object *add_box(struct device *dev, double x0,double y0,double z0,double dx,double dy,double dz,int object_type)
{
	object_init(&(dev->obj[dev->objects]));
	object_malloc(&(dev->obj[dev->objects]));
	dev->obj[dev->objects].uid=dev->objects;

	//btm
	add_triangle(dev,
					x0,y0,z0,
					x0+dx,y0,
					z0,x0,y0,z0+dz,
													dev->objects,object_type);
	add_triangle(dev,
					x0+dx	,	y0,z0,
					x0+dx	,	y0,z0+dz,
					x0   	,	y0,z0+dz,
													dev->objects,object_type);

	//top
	add_triangle(dev,
					x0		,y0+dy	,	z0		,
					x0+dx	,y0+dy	,	z0		,
					x0		,y0+dy	,	z0+dz	,
													dev->objects,object_type);
	add_triangle(dev,
					x0+dx	,y0+dy	,z0			,
					x0+dx	,y0+dy	,z0+dz		,
					x0   	,y0+dy	,z0+dz		,
													dev->objects,object_type);

	//left
	add_triangle(dev,
					x0		,y0		,z0			,
					x0		,y0+dy	,z0			,
					x0   	,y0		,z0+dz		,
													dev->objects,object_type);

	add_triangle(dev,
					x0		,y0+dy	,z0			,
					x0		,y0+dy	,z0+dz		,
					x0   	,y0		,z0+dz		,
													dev->objects,object_type);

	//right
	add_triangle(dev,
					x0+dx		,y0		,z0			,
					x0+dx		,y0+dy	,z0			,
					x0+dx   	,y0		,z0+dz		,
													dev->objects,object_type);

	add_triangle(dev,
					x0+dx	,y0+dy	,z0			,
					x0+dx	,y0+dy	,z0+dz		,
					x0+dx  	,y0		,z0+dz		,
													dev->objects,object_type);

	//front
	add_triangle(dev,
					x0			,y0		,z0		,
					x0+dx		,y0		,z0		,
					x0   		,y0+dy	,z0		,
													dev->objects,object_type);

	add_triangle(dev,
					x0			,y0+dy	,z0		,
					x0+dx		,y0+dy	,z0		,
					x0+dx   	,y0		,z0		,
													dev->objects,object_type);

	//back
	add_triangle(dev,
					x0			,y0		,z0+dz		,
					x0+dx		,y0		,z0+dz		,
					x0   		,y0+dy	,z0+dz		,
													dev->objects,object_type);

	add_triangle(dev,
					x0			,y0+dy	,z0+dz		,
					x0+dx		,y0+dy	,z0+dz		,
					x0+dx   	,y0		,z0+dz		,
													dev->objects,object_type);

	object_cal_min_max(&(dev->obj[dev->objects]));

	triangles_cal_edges(&(dev->obj[dev->objects].tri));

	dev->objects++;
	return &(dev->obj[dev->objects-1]);
}

struct object *add_plane(struct device *dev,double x0,double y0,double z0,double dx,double dz,int object_type)
{
	object_init(&(dev->obj[dev->objects]));
	object_malloc(&(dev->obj[dev->objects]));
	dev->obj[dev->objects].uid=dev->objects;

	//btm
	add_triangle(dev,
					x0,y0,z0,
					x0+dx,y0,
					z0,x0,y0,z0+dz,
													dev->objects,object_type);
	add_triangle(dev,
					x0+dx	,	y0,z0,
					x0+dx	,	y0,z0+dz,
					x0   	,	y0,z0+dz,
													dev->objects,object_type);

	object_cal_min_max(&(dev->obj[dev->objects]));

	triangles_cal_edges(&(dev->obj[dev->objects].tri));

	dev->objects++;
	return &(dev->obj[dev->objects-1]);
}

