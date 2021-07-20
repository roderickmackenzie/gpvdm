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

void heat_material_init(struct heat_material *mat)
{
	mat->thermal_kl=-1.0;
	mat->thermal_tau_e=-1.0;
	mat->thermal_tau_h=-1.0;
}

void heat_material_cpy(struct heat_material *out,struct heat_material *in)
{
	out->thermal_kl=in->thermal_kl;
	out->thermal_tau_e=in->thermal_tau_e;
	out->thermal_tau_h=in->thermal_tau_h;
}

void heat_material_free(struct heat_material *mat)
{
	mat->thermal_kl=-1.0;
	mat->thermal_tau_e=-1.0;
	mat->thermal_tau_h=-1.0;
}

void heat_material_load_from_json(struct simulation *sim,struct heat_material *mat, struct json_obj *json_heat_material)
{
	json_get_long_double(sim,json_heat_material, &(mat->thermal_kl),"thermal_kl");
	json_get_long_double(sim,json_heat_material, &(mat->thermal_tau_e),"thermal_tau_e");
	json_get_long_double(sim,json_heat_material, &(mat->thermal_tau_h),"thermal_tau_h");
}
