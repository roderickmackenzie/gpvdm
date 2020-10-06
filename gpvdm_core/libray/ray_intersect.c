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

/** @file ray_intersect.c
	@brief Ray intersect
*/

//bool RayIntersectsTriangle(Vector3D rayOrigin, Vector3D rayVector, Triangle* inTriangle, Vector3D& outIntersectionPoint)
int ray_intersect(struct vec *ret,struct triangle *tri,struct ray *my_ray)
{

    double EPSILON = 1e-18;
	double a,f,u,v,t;

	struct vec h;
	struct vec s;
	struct vec q;

	//struct vec edge1;
	//struct vec edge2;
	//vec_init(&edge1);
	//vec_init(&edge2);
	//vec_init(&h);
	//vec_init(&s);
	//vec_init(&q);



	////edge1 = vertex1 - vertex0;
	//vec_cpy(&edge1,&(tri->xy1));
	//vec_sub(&edge1,&(tri->xy0));

	////edge2 = vertex2 - vertex0;
	//vec_cpy(&edge2,&(tri->xy2));
	//vec_sub(&edge2,&(tri->xy0));

	////h = rayVector.crossProduct(edge2);
	//printf(">>>>>\n");
	//vec_print(&(edge1));
	//vec_print(&(tri->edge1));

	//vec_print(&(edge2));
	//vec_print(&(tri->edge2));

	vec_cross(&h,&(my_ray->dir),&tri->edge2);

	//vec_print(&(h));

    //a = edge1.dotProduct(h);
	a=vec_dot(&tri->edge1,&h);
	//printf("%e\n",a);
    if (a > -EPSILON && a < EPSILON)
	{
		//printf("exit1\n");
		return FALSE;    // This ray is parallel to this triangle.
	}
    f = 1.0/a;

	//s = rayOrigin - vertex0;
	vec_cpy(&s,&(my_ray->xy));
	vec_sub(&s,&(tri->xy0));

    //u = f * s.dotProduct(h);
	u=f*vec_dot(&s,&h);

	if (u < 0.0 || u > 1.0)
	{
		//printf("exit2\n");
		return FALSE;
	}

    //q = s.crossProduct(edge1);
	vec_cross(&q,&(s),&tri->edge1);

	//v = f * rayVector.dotProduct(q);
	v=f*vec_dot(&(my_ray->dir),&q);

    if (v < 0.0 || (u + v) > 1.0)
	{
		//printf("exit3\n");
		return FALSE;
	}

    // At this stage we can compute t to find out where the intersection point is on the line.
    //float t = f * edge2.dotProduct(q);
	t= f * vec_dot(&tri->edge2,&q);
    if (t > EPSILON) // ray intersection
    {
		//outIntersectionPoint = rayOrigin + rayVector * t;
		vec_cpy(ret,&(my_ray->dir));
		vec_mul(ret,t);
		vec_add(ret,&(my_ray->xy));
        return TRUE;
    }else // This means that there is a line intersection but not a ray intersection.
    {
		//printf("exit4\n");
		return FALSE;
	}
}

