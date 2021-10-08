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

/** @file 1d_long_double.c
@brief 1d long double routines
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


void malloc_1d(void * (*var),int zlen, int item_size)
{

	if (*var!=NULL)
	{
		printf("Warning allocating onto non NULL pointer\n");
		getchar();
	}

	if (zlen>0)
	{
		*var = (void *) malloc(zlen * item_size);
		memset(*var, 0, zlen * item_size);
	}else
	{
		*var = NULL;
	}
}



void free_1d( void * (*in_var), int item_size)
{

	void *var=*in_var;
	if (var==NULL)
	{
		return;
	}

	free(var);

	*in_var=NULL;

}

void cpy_1d(void * (*out), void * (*in), int zlen, int item_size)
{

	if ((zlen>0)&&(*in!=NULL))
	{
		memcpy((*out), (*in), zlen * item_size);
	}
}

void cpy_1d_alloc(void * (*out), void * (*in), int zlen, int item_size)
{
	free_1d(out, item_size);
	malloc_1d(out, zlen, item_size);
	cpy_1d(out, in, zlen, item_size);

}

