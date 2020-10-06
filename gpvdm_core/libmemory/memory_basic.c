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

/** @file memory_basic.c
@brief memory functions for 3D arrays
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lang.h>
#include "sim.h"
#include "dump.h"
#include "mesh.h"
#include <math.h>
#include "log.h"
#include <solver_interface.h>
#include "memory.h"



void zx_copy_gdouble(struct dimensions *dim, gdouble **dst, gdouble **src)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			dst[z][x]=src[z][x];
		}
	}

}



void memory_flip_1d_long_double(long double *var,int len)
{
	int x=0;
	int y=0;
	int z=0;
	long double * data=malloc(sizeof(long double)*len);
	for (y=0;y<len;y++)
	{
		data[y]=var[len-1-y];
	}

	for (y=0;y<len;y++)
	{
		var[y]=data[y];
	}

	free(data);

}

void three_d_interpolate_srh(long double ****out, long double ****in, struct dimensions *dim_out, struct dimensions *dim_in,int band)
{
int x=0;
int y=0;
int z=0;

int yi;
int xi;

long double y_out;
long double x_out;

long double y00;
long double y01;
long double yr;
long double y0;

long double y10;
long double y11;
long double y1;

long double x0;
long double x1;
long double xr;

long double c;

	z=0;
	for (x = 0; x < dim_out->xlen; x++)
	{

		x_out=dim_out->xmesh[x];
		xi=hashget(dim_in->xmesh,dim_in->xlen,x_out);

		for (y = 0; y < dim_out->ylen; y++)
		{
			y_out=dim_out->ymesh[y];
			yi=hashget(dim_in->ymesh,dim_in->ylen,y_out);

			y00=dim_in->ymesh[yi];
			y01=dim_in->ymesh[yi+1];
			yr=(y_out-y00)/(y01-y00);
			y0=in[z][xi][yi][band]+yr*(in[z][xi][yi+1][band]-in[z][xi][yi][band]);

			y10=dim_in->ymesh[yi];
			y11=dim_in->ymesh[yi+1];
			yr=(y_out-y10)/(y11-y10);
			y1=in[z][xi+1][yi][band]+yr*(in[z][xi+1][yi+1][band]-in[z][xi+1][yi][band]);

			x0=dim_in->xmesh[xi];
			x1=dim_in->xmesh[xi+1];
			xr=(x_out-x0)/(x1-x0);

			c=y0+xr*(y1-y0);
			out[z][x][y][band]=c;
		}

	}

}

void three_d_interpolate_srh2(long double ****out, long double ****in, struct dimensions *dim_out, struct dimensions *dim_in,int band)
{
int x=0;
int y=0;
int z=0;

int yi;
int xi;

long double y_out;
long double x_out;

long double y00;
long double y01;
long double yr;
long double y0;

long double y10;
long double y11;
long double y1;

long double x0;
long double x1;
long double xr;

long double c;

	z=0;
	for (x = 0; x < dim_out->xlen; x++)
	{

		x_out=dim_out->xmesh[x];
		xi=hashget(dim_in->xmesh,dim_in->xlen,x_out);

		for (y = 0; y < dim_out->ylen; y++)
		{
			y_out=dim_out->ymesh[y];
			yi=hashget(dim_in->ymesh,dim_in->ylen,y_out);

			y00=dim_in->ymesh[yi];
			y01=dim_in->ymesh[yi+1];
			yr=(y_out-y00)/(y01-y00);
			y0=in[z][xi][yi][band]+yr*(in[z][xi][yi+1][band]-in[z][xi][yi][band]);

			y10=dim_in->ymesh[yi];
			y11=dim_in->ymesh[yi+1];
			yr=(y_out-y10)/(y11-y10);
			y1=in[z][xi+1][yi][band]+yr*(in[z][xi+1][yi+1][band]-in[z][xi+1][yi][band]);

			x0=dim_in->xmesh[xi];
			x1=dim_in->xmesh[xi+1];
			xr=(x_out-x0)/(x1-x0);

			c=y0+xr*(y1-y0);
			out[z][x][y][band]=c;
		}

	}

}


void srh_quick_dump(char *file_name, long double ****in, struct dimensions *dim,int band)
{
int x=0;
int y=0;
int z=0;
	FILE *out=fopen(file_name,"w");

	for (z = 0; z < dim->zlen; z++)
	{

		for (x = 0; x < dim->xlen; x++)
		{

			for (y = 0; y < dim->ylen; y++)
			{
				fprintf(out,"%Le %Le %Le\n",dim->xmesh[x],dim->ymesh[y],in[z][x][y][band]);
			}

			fprintf(out,"\n");
		}
	}

fclose(out);
}

/**Do a chop search for a value
@param x index array
@param N length
@param find Value to find
*/
int search(long double *x,int N,long double find)
{
if (N==1) return 0;
int pos=N/2;
int step=N/2;
do
{
	step=step/2 + (step % 2 > 0 ? 1 : 0);

	if (x[pos]>find)
	{
		pos-=step;
	}else
	{
		pos+=step;
	}

	if (pos<=0)
	{
		pos=0;
		break;
	}
	if (pos>=(N-1))
	{
		pos=N-1;
		break;
	}
	if (step==0) break;
	if (x[pos]==find) break;
	if ((x[pos]<=find)&&((x[pos+1]>find))) break;

}while(1);

if (pos==(N-1)) pos=N-2;


return pos;
}
