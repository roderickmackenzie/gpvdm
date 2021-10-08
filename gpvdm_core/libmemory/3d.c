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

/** @file 3d_Int.c
@brief memory functions for 3D int arrays
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


void malloc_3d( void * (***var),int zlen, int xlen, int ylen,int item_size)
{
	int x=0;
	int y=0;
	int z=0;

	if (*var!=NULL)
	{
		printf("Warning allocating onto non NULL pointer\n");
		getchar();
	}

	if ((zlen>0)&&(xlen>0)&&(ylen>0))
	{
		*var = (void ***) malloc(zlen * sizeof(void **));

		for (z = 0; z < zlen; z++)
		{
			(*var)[z] = (void **) malloc(xlen * sizeof(void*));
			for (x = 0; x < xlen; x++)
			{
				(*var)[z][x] = (void *) malloc(ylen * item_size);
				memset((*var)[z][x], 0, ylen * item_size);
			}
		}
	}
}

void free_3d(void * (***in_var),int zlen, int xlen, int ylen,int item_size)
{
	int x=0;
	int y=0;
	int z=0;

	void ***var=*in_var;
	if (var==NULL)
	{
		return;
	}

	for (z = 0; z < zlen; z++)
	{

		for (x = 0; x < xlen; x++)
		{
			free(var[z][x]);
		}
		free(var[z]);
	}

	free(var);

	*in_var=NULL;

}

void cpy_3d(void * (***out), void * (***in),int zlen, int xlen, int ylen,int item_size)
{
	int x=0;
	int z=0;

	if (*in==NULL)
	{
		return;
	}

	//printf("%ld %ld %ld\n",zlen,xlen,in);
	for (z = 0; z < zlen; z++)
	{
		for (x = 0; x < xlen; x++)
		{
			memcpy((*out)[z][x], (*in)[z][x], ylen * item_size);
		}
	}

}

void cpy_3d_alloc(void * (***out), void * (***in),int zlen, int xlen, int ylen,int item_size)
{
	free_3d(out,zlen, xlen, ylen,item_size);
	if (*in==NULL)
	{
		return;
	}
	malloc_3d(out, zlen, xlen, ylen,item_size);
	cpy_3d(out, in, zlen, xlen, ylen,item_size);
}
