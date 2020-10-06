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

/** @file ntricks.c
	@brief A collection of helper functions for the newton solver.
*/


#include <stdio.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include "newton_tricks.h"
#include "gui_hooks.h"
#include <plot.h>
#include <cal_path.h>
#include <contacts.h>
#include <dump.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inp.h>
#include <memory.h>
#include <device.h>
#include <device_fun.h>


static int unused __attribute__((unused));

//<stripall>
void newton_state_complex_init(struct newton_state_complex *ns)
{

	ns->phi=NULL;

	ns->x=NULL;
	ns->xp=NULL;

	ns->xt=NULL;
	ns->xpt=NULL;

	dim_init(&(ns->dim));


}

void newton_state_complex_alloc_mesh(struct newton_state_complex *ns,struct dimensions *dim)
{
	ns->dim.xlen=dim->xlen;
	ns->dim.ylen=dim->ylen;
	ns->dim.zlen=dim->zlen;

	dim_alloc(&(ns->dim));

	malloc_zxy_long_double_complex(dim,&(ns->phi));

	malloc_zxy_long_double_complex(dim,&(ns->x));
	malloc_zxy_long_double_complex(dim,&(ns->xp));

}

void newton_state_complex_alloc_traps(struct newton_state_complex *ns,struct dimensions *dim)
{
	//ns->dim.srh_bands=dim->srh_bands;

	//malloc_srh_bands(dim, &(ns->xt));
	//malloc_srh_bands(dim, &(ns->xpt));

}

void newton_state_complex_free(struct newton_state_complex *ns)
{

	free_zxy_long_double_complex(&(ns->dim),&ns->phi);

	free_zxy_long_double_complex(&(ns->dim),&ns->x);
	free_zxy_long_double_complex(&(ns->dim),&ns->xp);


	//free_srh_bands(&(ns->dim), &ns->xt);
	//free_srh_bands(&(ns->dim), &ns->xpt);

	dim_free(&(ns->dim));

}



