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


/** @file heat_cpy.c
	@brief Malloc for heat structure
*/

#include <stdlib.h>
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


static int unused __attribute__((unused));



void heat_cpy(struct simulation *sim,struct heat *out,struct heat *in)
{
	//printf_log(sim,"cpy: heat_memory\n");
	struct dimensions *dim=&(in->dim);

	strcpy(out->dump_dir,in->dump_dir);
	out->thermal_model_type=in->thermal_model_type;

	//zxy
	cpy_zxy_long_double(dim,&(out->Tl),&(in->Tl));
	cpy_zxy_long_double(dim,&(out->Te),&(in->Te));
	cpy_zxy_long_double(dim,&(out->Th),&(in->Th));

	cpy_zxy_long_double(dim,&(out->Hl),&(in->Hl));

	cpy_zxy_long_double(dim,&(out->H_optical),&(in->H_optical));
	cpy_zxy_long_double(dim,&(out->H_recombination),&(in->H_recombination));
	cpy_zxy_long_double(dim,&(out->H_joule),&(in->H_joule));
	cpy_zxy_long_double(dim,&(out->H_parasitic),&(in->H_parasitic));

	cpy_zxy_long_double(dim,&(out->He),&(in->He));
	cpy_zxy_long_double(dim,&(out->Hh),&(in->Hh));

	cpy_zxy_long_double(dim,&(out->kl),&(in->kl));
	cpy_zxy_long_double(dim,&(out->ke),&(in->ke));
	cpy_zxy_long_double(dim,&(out->kh),&(in->kh));

	//objects
	cpy_zxy_p_object(dim, &(out->obj),&(in->obj));


	//boundry temperatures
	out->Ty0=in->Ty0;
	out->Ty1=in->Ty1;
	out->Tx0=in->Tx0;
	out->Tx1=in->Tx1;
	out->Tz0=in->Tz0;
	out->Tz1=in->Tz1;

	//Boundry type
	out->Ty0_boundry=in->Ty0_boundry;
	out->Ty1_boundry=in->Ty1_boundry;
	out->Tx0_boundry=in->Tx0_boundry;
	out->Tx1_boundry=in->Tx1_boundry;
	out->Tz0_boundry=in->Tz0_boundry;
	out->Tz1_boundry=in->Tz1_boundry;

	//heat sink
	out->heatsink_y0=in->heatsink_y0;
	out->heatsink_y1=in->heatsink_y1;
	out->heatsink_x0=in->heatsink_x0;
	out->heatsink_x1=in->heatsink_x1;
	out->heatsink_z0=in->heatsink_z0;
	out->heatsink_z1=in->heatsink_z1;

	out->heatsink_length_y0=in->heatsink_length_y0;
	out->heatsink_length_y1=in->heatsink_length_y1;
	out->heatsink_length_x0=in->heatsink_length_x0;
	out->heatsink_length_x1=in->heatsink_length_x1;
	out->heatsink_length_z0=in->heatsink_length_z0;
	out->heatsink_length_z1=in->heatsink_length_z1;

	out->Tliso=in->Tliso;
	out->Triso=in->Triso;
	out->nofluxl=in->nofluxl;

	//convergance
	out->thermal_conv=in->thermal_conv;
	out->min_error=in->min_error;
	out->newton_enable_external_thermal=in->newton_enable_external_thermal;
	out->thermal_couple_to_electrical_solver=in->thermal_couple_to_electrical_solver;
	out->thermal_l=in->thermal_l;
	out->thermal_e=in->thermal_e;
	out->thermal_h=in->thermal_h;
	out->thermal_max_ittr=in->thermal_max_ittr;
	out->min_error=in->min_error;

	//Options
	out->joule_heating=in->joule_heating;
	out->parasitic_heating=in->parasitic_heating;
	out->recombination_heating=in->recombination_heating;
	out->optical_heating=in->optical_heating;

	matrix_cpy(sim,&(out->mx),&(in->mx));

	out->dump_verbosity=in->dump_verbosity;
	out->solver_verbosity=in->solver_verbosity;
}

