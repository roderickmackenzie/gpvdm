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
#include "object.h"

void malloc_light_zxy_p_object(struct dimensions *dim, struct object * (****var))
{
	/*int x=0;
	int y=0;
	int z=0;

	*var = (struct object ****) malloc(dim->zlen * sizeof(struct object***));

	for (z = 0; z < dim->zlen; z++)
	{
		(*var)[z] = (struct object ***) malloc(dim->xlen * sizeof(struct object**));
		for (x = 0; x < dim->xlen; x++)
		{
			(*var)[z][x] = (struct object**) malloc(dim->ylen * sizeof(struct object*));
			memset((*var)[z][x], 0, dim->ylen * sizeof(struct object*));
		}
	}*/
	malloc_3d( (void ****)var,dim->zlen, dim->xlen, dim->ylen,sizeof(struct object*));
}



void free_light_zxy_p_object(struct dimensions *dim, struct object * (****in_var))
{
	/*int x=0;
	int y=0;
	int z=0;

	struct object ****var=*in_var;
	if (var==NULL)
	{
		return;
	}

	for (z = 0; z < dim->zlen; z++)
	{

		for (x = 0; x < dim->xlen; x++)
		{
			free(var[z][x]);
		}
		free(var[z]);
	}

	free(var);

	*in_var=NULL;*/
	free_3d(  (void ****)in_var,dim->zlen, dim->xlen, dim->ylen,sizeof(struct object*));
}

void cpy_light_zxy_p_object(struct dimensions *dim, struct object * (****out),struct object * (****in))
{
	free_3d(  (void ****)out,dim->zlen, dim->xlen, dim->ylen,sizeof(struct object*));
	malloc_3d(  (void ****)out,dim->zlen, dim->xlen, dim->ylen,sizeof(struct object*));
	cpy_3d(  (void ****)out, (void ****)in,dim->zlen, dim->xlen, dim->ylen,sizeof(struct object*));
}

