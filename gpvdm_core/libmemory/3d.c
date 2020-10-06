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


