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

	dim_malloc(&(ns->dim));

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



