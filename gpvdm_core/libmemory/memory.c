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


