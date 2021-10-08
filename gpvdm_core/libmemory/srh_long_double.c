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

void free_srh_bands(struct dimensions *dim, gdouble *(**** in_var))
{
	free_4d_long_double(dim->zlen, dim->xlen, dim->ylen,dim->srh_bands, in_var);
}


void cpy_srh_long_double(struct dimensions *dim, long double *(****dst), long double *(****src))
{
	free_4d_long_double(dim->zlen, dim->xlen, dim->ylen,dim->srh_bands, dst);
	if (*src==NULL)
	{
		return;
	}
	malloc_4d_long_double(dim->zlen, dim->xlen, dim->ylen,dim->srh_bands, dst);
	cpy_4d_long_double(dim->zlen, dim->xlen, dim->ylen,dim->srh_bands, dst, src);
}

void malloc_srh_bands(struct dimensions *dim, gdouble * (****var))
{
	malloc_4d_long_double(dim->zlen, dim->xlen, dim->ylen,dim->srh_bands, var);

}


