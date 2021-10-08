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
