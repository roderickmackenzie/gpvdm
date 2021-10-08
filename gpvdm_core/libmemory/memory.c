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

/** @file memory.c
@brief get/free memory
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
#include <circuit.h>
#include "memory.h"
#include "ray_fun.h"
#include "newton_tricks.h"
#include "shape.h"

void device_alloc_traps(struct device *in)
{
	struct dimensions *dim=&in->ns.dim;

	if (dim->srh_bands==-1)
	{
		return;
	}
	//printf("hello %d %d %d %d \n",dim->xlen,dim->ylen,dim->zlen,dim->srh_bands);
	//getchar();
	malloc_srh_bands(dim, &(in->nt));
	//printf("1\n");
	malloc_srh_bands(dim, &(in->ntlast));

	malloc_srh_bands(dim, &(in->dnt));
	//printf("2\n");
	malloc_srh_bands(dim, &(in->srh_n_r1));
	malloc_srh_bands(dim, &(in->srh_n_r2));
	malloc_srh_bands(dim, &(in->srh_n_r3));
	malloc_srh_bands(dim, &(in->srh_n_r4));
	malloc_srh_bands(dim, &(in->dsrh_n_r1));
	//printf("3\n");
	malloc_srh_bands(dim, &(in->dsrh_n_r2));
	malloc_srh_bands(dim, &(in->dsrh_n_r3));
	malloc_srh_bands(dim, &(in->dsrh_n_r4));
	malloc_srh_bands(dim, &(in->Fnt));
	malloc_srh_bands(dim, &(in->ntb_save));
	//printf("4\n");
	malloc_srh_bands(dim, &(in->nt_r1));
	malloc_srh_bands(dim, &(in->nt_r2));
	malloc_srh_bands(dim, &(in->nt_r3));
	malloc_srh_bands(dim, &(in->nt_r4));

	malloc_srh_bands(dim, &(in->pt));
	malloc_srh_bands(dim, &(in->ptlast));
	//printf("5\n");
	//printf("hello %d %d %d %d \n",dim->xlen,dim->ylen,dim->zlen,dim->srh_bands);

	malloc_srh_bands(dim, &(in->dpt));
	malloc_srh_bands(dim, &(in->srh_p_r1));
	malloc_srh_bands(dim, &(in->srh_p_r2));
	malloc_srh_bands(dim, &(in->srh_p_r3));
	malloc_srh_bands(dim, &(in->srh_p_r4));
	malloc_srh_bands(dim, &(in->dsrh_p_r1));
	malloc_srh_bands(dim, &(in->dsrh_p_r2));
	//printf("6\n");
	malloc_srh_bands(dim, &(in->dsrh_p_r3));
	//printf("7\n");
	malloc_srh_bands(dim, &(in->dsrh_p_r4));
	//printf("8\n");
	malloc_srh_bands(dim, &(in->ptb_save));
	//printf("9\n");
	malloc_srh_bands(dim, &(in->Fpt));
	//printf("10\n");
	malloc_srh_bands(dim, &(in->pt_r1));
	malloc_srh_bands(dim, &(in->pt_r2));
	malloc_srh_bands(dim, &(in->pt_r3));
	malloc_srh_bands(dim, &(in->pt_r4));

	//getchar();

	newton_state_alloc_traps(&(in->ns),dim);

}


