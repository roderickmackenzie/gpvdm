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

void free_4d_long_double(int zlen, int xlen, int ylen,int bands, long double *(**** in_var))
{
	long double ****var=*in_var;

	if (var==NULL)
	{
		return;
	}

	int x=0;
	int y=0;
	int z=0;

	for (z = 0; z < zlen; z++)
	{
		for (x = 0; x < xlen; x++)
		{
			for (y = 0; y < ylen; y++)
			{
					free(var[z][x][y]);
			}
			free(var[z][x]);
		}
		free(var[z]);
	}

	free(var);

	*in_var=NULL;
}


void cpy_4d_long_double(int zlen, int xlen, int ylen,int bands, long double *(****dst), long double *(****src))
{
int x=0;
int y=0;
int z=0;
int b=0;

	for (z = 0; z < zlen; z++)
	{
		for (x = 0; x < xlen; x++)
		{
			for (y = 0; y < ylen; y++)
			{
				memcpy((*dst)[z][x][y], (*src)[z][x][y], bands * sizeof(long double));
			}

		}
	}

}

void malloc_4d_long_double(int zlen, int xlen, int ylen,int bands, long double * (****var))
{
	int x=0;
	int y=0;
	int z=0;

	//printf("alloc %d %d %d %d \n",xlen,ylen,zlen,srh_bands);
	if ((zlen>0)&&(xlen>0))
	{
		*var = (long double  ****) malloc(zlen * sizeof(long double  ***));

		for (z = 0; z < zlen; z++)
		{
			(*var)[z] = (long double  ***) malloc(xlen * sizeof(long double **));
			for (x = 0; x < xlen; x++)
			{
				(*var)[z][x] = (long double  **) malloc(ylen * sizeof(long double *));
				for (y = 0; y < ylen; y++)
				{
					if (bands != 0)
					{
						(*var)[z][x][y] = (long double  *) malloc(bands * sizeof(long double ));
						memset((*var)[z][x][y], 0, bands * sizeof(long double ));
					}else
					{
						(*var)[z][x][y] = NULL;
					}
				}

			}
		}
	}

}


