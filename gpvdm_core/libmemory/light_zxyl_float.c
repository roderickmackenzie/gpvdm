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


void malloc_light_zxyl_float(struct dimensions *dim, float * (****var))
{
	malloc_4d( (void *****)var, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(float));
}



void free_light_zxyl_float(struct dimensions *dim, float * (****in_var))
{
	free_4d( (void *****)in_var, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(float));
}

void cpy_light_zxyl_float(struct dimensions *dim, float * (****out), float * (****in))
{
	free_4d( (void *****)out, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(float));
	if (*in==NULL)
	{
		return;
	}
	malloc_4d( (void *****)out, dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(float));
	cpy_4d( (void *****)out, (void *****)in,dim->zlen, dim->xlen, dim->ylen,dim->llen,sizeof(float));
}


void div_light_zxyl_float(struct dimensions *dim, float ****data,float val)
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

void memset_light_zxyl_float(struct dimensions *dim, float ****data,int val)
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
				memset(data[z][x][y], val, dim->llen * sizeof(float ));
			}
		}
	}

}

void memset_light_zxyl_float_y(struct dimensions *dim, float ****data,int z, int x, int l,float val)
{
	int y=0;
	for (y = 0; y < dim->ylen; y++)
	{
		data[z][x][y][l]=val;
	}

}

