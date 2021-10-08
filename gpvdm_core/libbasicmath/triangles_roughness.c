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

