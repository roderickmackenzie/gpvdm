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

/** @file light_zxy_float.c
@brief memory functions for light zxy arrays
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


void malloc_light_zxy_float(struct dimensions *dim, float * (***var))
{
	malloc_3d((void ****)var,dim->zlen, dim->xlen, dim->ylen, sizeof(float));

}


void free_light_zxy_float(struct dimensions *dim, float * (***var))
{
	free_3d((void ****)var,dim->zlen, dim->xlen, dim->ylen, sizeof(float));
}

void cpy_light_zxy_float(struct dimensions *dim, float * (***out), float * (***in))
{
	free_3d((void ****)out,dim->zlen, dim->xlen, dim->ylen, sizeof(float));
	malloc_3d((void ****)out,dim->zlen, dim->xlen, dim->ylen, sizeof(float));
	cpy_3d((void ****)out,(void ****)in,dim->zlen, dim->xlen, dim->ylen,sizeof(float));
}

void cpy_light_zxy_float_no_alloc(struct dimensions *dim, float * (***out), float * (***in))
{
	cpy_3d((void ****)out,(void ****)in,dim->zlen, dim->xlen, dim->ylen,sizeof(float));
}

void light_zxy_mul_float(struct simulation *sim, struct dimensions *dim,float *** data,float mul)
{
	int x=0;
	int y=0;
	int z=0;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				data[z][x][y]*=mul;
			}
		}
	}

}

void flip_light_zxy_float_y(struct simulation *sim, struct dimensions *dim,float *** data)
{
	int x=0;
	int y=0;
	int z=0;


	float ***temp=NULL;

	malloc_light_zxy_float(dim, &temp);


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				temp[z][x][y]=data[z][x][y];
			}
		}
	}


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				data[z][x][dim->ylen-y-1]=temp[z][x][y];
			}
		}
	}


	free_light_zxy_float(dim, &temp);
}

void memset_light_zxy_float(struct dimensions *dim, float ***data,int val)
{
	int x=0;
	int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			memset(data[z][x], val, dim->ylen * sizeof(float ));
		}
	}

}

void div_light_zxy_float(struct dimensions *dim, float ***data,float val)
{
	int x=0;
	int y=0;
	int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				data[z][x][y]/=val;
			}
		}
	}

}

