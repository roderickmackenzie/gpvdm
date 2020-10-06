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
			A=dim->dx[x]*dim->dz[z];
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


