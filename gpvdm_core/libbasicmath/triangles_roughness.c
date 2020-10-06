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
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <dat_file.h>
#include <string.h>
#include <util.h>
#include <triangle.h>
#include <triangles.h>


/** @file triangles_roughness.c
	@brief Calculate roughness of triangles 
*/

//Surface average height
double triangle_Ra(struct simulation *sim,struct triangles *obj)
{
	int i;
	//printf("obj->%d",obj->edges_calculated);
	//getchar();
	struct triangle *tri;
	struct vec cog;
	struct vec min;
	//triangles_find_min(&min,obj);
	triangles_unset_flags_for_flat(obj);
	double area=0.0;
	double a=0.0;
	double tot=0.0;
	for (i=0;i<obj->len;i++)
	{
		tri=&(obj->data[i]);

		if (tri->flag==TRUE)
		{
			triangle_cal_cog(tri);
			a=triangle_cal_area(tri);
			area+=a;
			tot+=tri->cog.y*a;
			//printf("%le %le %le\n",cog.y,a,tot/area);
		}
	}

	

	triangles_set_flag(obj,TRUE);
	if (area==0.0)
	{
		return -1.0;
	}

	return tot/area;
}

//Surface RMS roughness
double triangle_Rq(struct simulation *sim,struct triangles *obj)
{
	int i;
	double Ra=0.0;
	//printf("obj->%d",obj->edges_calculated);
	//getchar();
	struct triangle *tri;
	struct vec cog;
	double area=0.0;
	double a=0.0;
	double tot=0.0;
	Ra=triangle_Ra(sim,obj);
	triangles_unset_flags_for_flat(obj);
	for (i=0;i<obj->len;i++)
	{
		tri=&(obj->data[i]);

		if (tri->flag==TRUE)
		{
			a=triangle_cal_area(tri);
			triangle_cal_cog(tri);
			tot+=a*pow((tri->cog.y-Ra),2.0);
			area+=a;
		}
	}

	
	triangles_set_flag(obj,TRUE);

	if (area==0)
	{
		return -1;
	}

	return sqrt(tot/area);
}

