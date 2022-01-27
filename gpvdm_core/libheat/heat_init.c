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


/** @file init.c
	@brief Initilization for heat structure
*/

#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "gpvdm_const.h"
#include "heat.h"
#include "device.h"
#include "config.h"
#include "util.h"
#include "lang.h"
#include "log.h"
#include <light_fun.h>
#include <matrix.h>
#include <memory.h>
#include <device_fun.h>
#include <mesh.h>

static int unused __attribute__((unused));

void heat_init(struct heat *thermal)
{
	struct dimensions *dim=&(thermal->dim);
	strcpy(thermal->dump_dir,"");

	//zxy
	thermal->Tl=NULL;
	thermal->Te=NULL;
	thermal->Th=NULL;

	thermal->Hl=NULL;
	thermal->H_optical=NULL;
	thermal->H_recombination=NULL;
	thermal->H_joule=NULL;
	thermal->H_parasitic=NULL;

	thermal->He=NULL;
	thermal->Hh=NULL;

	thermal->kl=NULL;
	thermal->ke=NULL;
	thermal->kh=NULL;

	//objects
	thermal->obj=NULL;

	thermal->thermal_l=FALSE;
	thermal->thermal_e=FALSE;
	thermal->thermal_h=FALSE;

	thermal->Ty0=-1.0;
	thermal->Ty1=-1.0;
	thermal->Tx0=-1.0;
	thermal->Tx1=-1.0;
	thermal->Tz0=-1.0;
	thermal->Tz1=-1.0;

	thermal->Ty0_boundry=-1;
	thermal->Ty1_boundry=-1;
	thermal->Tx0_boundry=-1;
	thermal->Tx1_boundry=-1;
	thermal->Tz0_boundry=-1;
	thermal->Tz1_boundry=-1;

	thermal->heatsink_y0=-1;
	thermal->heatsink_y1=-1;
	thermal->heatsink_x0=-1;
	thermal->heatsink_x1=-1;
	thermal->heatsink_z0=-1;
	thermal->heatsink_z1=-1;

	thermal->heatsink_length_y0=-1;
	thermal->heatsink_length_y1=-1;
	thermal->heatsink_length_x0=-1;
	thermal->heatsink_length_x1=-1;
	thermal->heatsink_length_z0=-1;
	thermal->heatsink_length_z1=-1;

	//This is old
	thermal->Tliso=FALSE;
	thermal->Triso=FALSE;

	thermal->thermal_conv=FALSE;

	thermal->min_error=2e-11;

	thermal->joule_heating=-1;
	thermal->parasitic_heating=-1;
	thermal->recombination_heating=-1;
	thermal->optical_heating=-1;

	dim_init(dim);
	mesh_obj_init(&(thermal->mesh_data));
	matrix_init(&(thermal->mx));

	thermal->dump_verbosity=-1;

	thermal->solver_verbosity=-1;


}

