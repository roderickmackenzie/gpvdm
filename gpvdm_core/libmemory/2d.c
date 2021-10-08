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



void malloc_2d(void * (**var), int zlen, int xlen, int item_size)
{
	int z=0;

	*var = (void **) malloc(zlen * sizeof(void *));

	for (z = 0; z < zlen; z++)
	{
		(*var)[z] = (void *) malloc(xlen * item_size);
		memset((*var)[z], 0, xlen * item_size);
	}

}

void free_2d(void *(**in_var), int zlen, int xlen, int item_size)
{
	int z=0;

	void **var=*in_var;

	if (var==NULL)
	{
		return;
	}

	for (z = 0; z < zlen; z++)
	{
		free(var[z]);
	}

	free(var);

	*in_var=NULL;
}

void cpy_2d(void * (**out), void * (**in),int zlen, int xlen, int item_size)
{
	int z=0;

	for (z = 0; z < zlen; z++)
	{
		memcpy((*out)[z], (*in)[z], xlen * item_size);
	}

}

void cpy_2d_alloc(void * (**out), void * (**in),int zlen, int xlen, int item_size)
{
	free_2d((void ***)out, zlen, xlen, item_size);
	if (*in==NULL)
	{
		return;
	}
	malloc_2d((void ***)out, zlen, xlen, item_size);
	cpy_2d((void ***)out, (void ***)in,zlen, xlen, item_size);

}
