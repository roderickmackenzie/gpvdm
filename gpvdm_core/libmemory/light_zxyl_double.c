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


void malloc_light_zxyl_double(struct dimensions *dim, double * (****var))
{
	malloc_4d( (void *****)var, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
}



void free_light_zxyl_double(struct dimensions *dim, double * (****in_var))
{
	free_4d( (void *****)in_var, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
}

void cpy_light_zxyl_double(struct dimensions *dim, double * (****out), double * (****in))
{
	free_4d( (void *****)out, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
	malloc_4d( (void *****)out, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
	cpy_4d( (void *****)out, (void *****)in,dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
}

void cpy_light_zxyl_wavelength_double(struct dimensions *dim, double ****out, double ****in,int l)
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
				out[z][x][y][l]=in[z][x][y][l];
			}
		}
	}
}

void flip_light_zxyl_double_y(struct simulation *sim, struct dimensions *dim,double **** data)
{
	int x=0;
	int y=0;
	int z=0;
	int l=0;


	double ****temp=NULL;

	malloc_light_zxyl_double(dim, &temp);

	for (l=0;l<dim->llen;l++)
	{
		for (z=0;z<dim->zlen;z++)
		{

			for (x=0;x<dim->xlen;x++)
			{

				for (y=0;y<dim->ylen;y++)
				{
					temp[z][x][y][l]=data[z][x][y][l];
				}

			}
		}
	}

	for (l=0;l<dim->llen;l++)
	{
		for (z=0;z<dim->zlen;z++)
		{

			for (x=0;x<dim->xlen;x++)
			{

				for (y=0;y<dim->ylen;y++)
				{
					data[z][x][dim->ylen-y-1][l]=temp[z][x][y][l];
				}
			}
		}
	}


	free_light_zxyl_double(dim, &temp);
}

void div_light_zxyl_double(struct dimensions *dim, double ****data,double val)
{
	int x=0;
	int y=0;
	int z=0;
	int l=0;

	for (l = 0; l < dim->llen; l++)
	{
		for (z = 0; z < dim->zlen; z++)
		{
			for (x = 0; x < dim->xlen; x++)
			{
				for (y = 0; y < dim->ylen; y++)
				{
					data[z][x][y][l]/=val;
				}
			}
		}
	}

}

void memset_light_zxyl_double(struct dimensions *dim, double ****data,int val)
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
				memset(data[z][x][y], val, dim->llen * sizeof(double ));
			}
		}
	}

}

void memset_light_zxyl_double_y(struct dimensions *dim, double ****data,int z, int x, int l,double val)
{
	int y=0;
	for (y = 0; y < dim->ylen; y++)
	{
		data[z][x][y][l]=val;
	}

}

