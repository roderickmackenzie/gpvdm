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

