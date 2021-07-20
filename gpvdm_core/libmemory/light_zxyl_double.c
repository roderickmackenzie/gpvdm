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


void malloc_light_zxyl_double(struct dim_light *dim, double * (****var))
{
	malloc_4d( (void *****)var, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
}



void free_light_zxyl_double(struct dim_light *dim, double * (****in_var))
{
	free_4d( (void *****)in_var, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
}

void cpy_light_zxyl_double(struct dim_light *dim, double * (****out), double * (****in))
{
	free_4d( (void *****)out, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
	malloc_4d( (void *****)out, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
	cpy_4d( (void *****)out, (void *****)in,dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(double));
}

void flip_light_zxyl_double_y(struct simulation *sim, struct dim_light *dim,double **** data)
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

void div_light_zxyl_double(struct dim_light *dim, double ****data,double val)
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

void memset_light_zxyl_double(struct dim_light *dim, double ****data,int val)
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

void memset_light_zxyl_double_y(struct dim_light *dim, double ****data,int z, int x, int l,double val)
{
	int y=0;
	for (y = 0; y < dim->ylen; y++)
	{
		data[z][x][y][l]=val;
	}

}

