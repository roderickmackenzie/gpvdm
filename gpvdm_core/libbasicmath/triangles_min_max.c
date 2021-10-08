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
#include <dat_file.h>
#include <string.h>
#include <util.h>


/** @file triangles_min_max.c
	@brief Min max functions for triangle functions
*/


void triangles_find_min(struct vec *out,struct triangles *in)
{
	int i;
	double x=in->data[0].xy0.x;
	double y=in->data[0].xy0.y;
	double z=in->data[0].xy0.z;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		//if (tri->flag==TRUE)
		//{
			if (tri->xy0.x<x)
			{
				x=tri->xy0.x;
			}

			if (tri->xy0.y<y)
			{
				y=tri->xy0.y;
			}

			if (tri->xy0.z<z)
			{
				z=tri->xy0.z;
			}


			if (tri->xy1.x<x)
			{
				x=tri->xy1.x;
			}

			if (tri->xy1.y<y)
			{
				y=tri->xy1.y;
			}

			if (tri->xy1.z<z)
			{
				z=tri->xy1.z;
			}

			if (tri->xy2.x<x)
			{
				x=tri->xy2.x;
			}

			if (tri->xy2.y<y)
			{
				y=tri->xy2.y;
			}

			if (tri->xy2.z<z)
			{
				z=tri->xy2.z;
			}
		//}
	}

	out->x=x;
	out->y=y;
	out->z=z;

}

void triangles_find_max(struct vec *out,struct triangles *in)
{

	int i;

	double x=in->data[0].xy0.x;
	double y=in->data[0].xy0.y;
	double z=in->data[0].xy0.z;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		//if (tri->flag==TRUE)
		//{
			if (tri->xy0.x>x)
			{
				x=tri->xy0.x;
			}

			if (tri->xy0.y>y)
			{
				y=tri->xy0.y;
			}

			if (tri->xy0.z>z)
			{
				z=tri->xy0.z;
			}


			if (tri->xy1.x>x)
			{
				x=tri->xy1.x;
			}

			if (tri->xy1.y>y)
			{
				y=tri->xy1.y;
			}

			if (tri->xy1.z>z)
			{
				z=tri->xy1.z;
			}

			if (tri->xy2.x>x)
			{
				x=tri->xy2.x;
			}

			if (tri->xy2.y>y)
			{
				y=tri->xy2.y;
			}

			if (tri->xy2.z>z)
			{
				z=tri->xy2.z;
			}
		//}
	}

	out->x=x;
	out->y=y;
	out->z=z;

}

