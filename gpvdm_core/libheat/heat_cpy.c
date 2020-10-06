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
	struct dim_heat *dim=&(in->dim);

	strcpy(out->dump_dir,in->dump_dir);
	out->thermal_model_type=in->thermal_model_type;

	//zxy
	cpy_heat_zxy_long_double(dim,&(out->Tl),&(in->Tl));
	cpy_heat_zxy_long_double(dim,&(out->Te),&(in->Te));
	cpy_heat_zxy_long_double(dim,&(out->Th),&(in->Th));

	cpy_heat_zxy_long_double(dim,&(out->Hl),&(in->Hl));

	cpy_heat_zxy_long_double(dim,&(out->H_optical),&(in->H_optical));
	cpy_heat_zxy_long_double(dim,&(out->H_recombination),&(in->H_recombination));
	cpy_heat_zxy_long_double(dim,&(out->H_joule),&(in->H_joule));


	cpy_heat_zxy_long_double(dim,&(out->He),&(in->He));
	cpy_heat_zxy_long_double(dim,&(out->Hh),&(in->Hh));

	cpy_heat_zxy_long_double(dim,&(out->kl),&(in->kl));
	cpy_heat_zxy_long_double(dim,&(out->ke),&(in->ke));
	cpy_heat_zxy_long_double(dim,&(out->kh),&(in->kh));

	//objects
	cpy_heat_zxy_p_object(dim, &(out->obj),&(in->obj));

	out->thermal_kl=in->thermal_kl;
	out->thermal_tau_e=in->thermal_tau_e;
	out->thermal_tau_h=in->thermal_tau_h;


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
	out->thermal_l=in->thermal_l;
	out->thermal_e=in->thermal_e;
	out->thermal_h=in->thermal_h;
	out->thermal_max_ittr=in->thermal_max_ittr;

	//Options
	out->joule_heating=in->joule_heating;
	out->recombination_heating=in->recombination_heating;
	out->optical_heating=in->optical_heating;

	matrix_cpy(sim,&(out->mx),&(in->mx));

}

