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

/** @file zx_long_double.c
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
#include "contacts.h"



void malloc_zx_gdouble(struct dimensions *dim, gdouble * (**var))
{
	malloc_2d_long_double(dim->zlen, dim->xlen, var);
}

void mem_set_zx_gdouble_from_zx_gdouble(struct dimensions *dim, gdouble **data_out, gdouble **data_in)
{
	int z=0;
	int x=0;


	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			data_out[z][x]=data_in[z][x];
		}
	}

}

void mem_set_zx_long_double(struct dimensions *dim, long double **data, long double val)
{
	int z=0;
	int x=0;


	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			data[z][x]=val;
		}
	}

}

void mem_zx_invert_long_double(struct dimensions *dim, long double **data)
{
	int z=0;
	int x=0;


	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			data[z][x]=1.0/data[z][x];
		}
	}

}

void mem_mul_zx_area(struct dimensions *dim, long double **data)
{
	int z=0;
	int x=0;
	long double A=0.0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			A=dim->dX[x]*dim->dZ[z];
			data[z][x]*=A;
		}
	}

}

void mem_mul_zx_long_double(struct dimensions *dim, long double **data,long double val)
{
	int z=0;
	int x=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			data[z][x]*=val;
		}
	}

}

void mem_div_zx_long_double(struct dimensions *dim, long double **data,long double val)
{
	int z=0;
	int x=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			data[z][x]/=val;
		}
	}

}

void mem_add_zx_gdouble_from_zx_gdouble(struct dimensions *dim, gdouble **data_out, gdouble **data_in)
{
	int z=0;
	int x=0;


	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			data_out[z][x]+=data_in[z][x];
		}
	}

}

void free_zx_gdouble(struct dimensions *dim, gdouble * (**in_var))
{
	free_2d_long_double(dim->zlen, dim->xlen, in_var);
}

void cpy_zx_long_double(struct dimensions *dim, long double * (**out),long double * (**in))
{
	free_2d_long_double(dim->zlen, dim->xlen, out);
	malloc_2d_long_double(dim->zlen, dim->xlen, out);
	cpy_2d_long_double(dim->zlen, dim->xlen, out, in);
}


