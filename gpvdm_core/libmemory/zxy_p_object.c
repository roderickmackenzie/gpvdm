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

/** @file heat_zxy_p_object.c
@brief Functions to handle zxy arrays which are pointers to objects
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

void malloc_zxy_p_object(struct dimensions *dim, struct object * (****var))
{
	malloc_3d((void****)var,dim->zlen,dim->xlen,dim->ylen,sizeof(struct object *));
}



void free_zxy_p_object(struct dimensions *dim, struct object * (****var))
{
	free_3d((void****)var,dim->zlen,dim->xlen,dim->ylen,sizeof(struct object *));
}

void cpy_zxy_p_object(struct dimensions *dim, struct object * (****out), struct object * (****in))
{
	free_3d((void****)out,dim->zlen,dim->xlen,dim->ylen,sizeof(struct object *));
	malloc_3d((void****)out,dim->zlen,dim->xlen,dim->ylen,sizeof(struct object *));
	cpy_3d((void****)out,(void****)in,dim->zlen,dim->xlen,dim->ylen,sizeof(struct object *));

}

