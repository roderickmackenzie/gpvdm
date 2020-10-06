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


/** @file free.c
	@brief Free for heat structure
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
#include <mesh.h>

static int unused __attribute__((unused));

void heat_free_memory(struct simulation *sim,struct heat *thermal)
{
	struct dim_heat *dim=&(thermal->dim);
	struct matrix *mx=&(thermal->mx);

	if (thermal->newton_enable_external_thermal==TRUE)
	{
		//long double zxy
		free_heat_zxy_long_double(dim,&(thermal->Tl));
		free_heat_zxy_long_double(dim,&(thermal->Te));
		free_heat_zxy_long_double(dim,&(thermal->Th));

		free_heat_zxy_long_double(dim,&(thermal->Hl));
		free_heat_zxy_long_double(dim,&(thermal->H_optical));
		free_heat_zxy_long_double(dim,&(thermal->H_recombination));
		free_heat_zxy_long_double(dim,&(thermal->H_joule));

		free_heat_zxy_long_double(dim,&(thermal->He));
		free_heat_zxy_long_double(dim,&(thermal->Hh));

		free_heat_zxy_long_double(dim,&(thermal->kl));
		free_heat_zxy_long_double(dim,&(thermal->ke));
		free_heat_zxy_long_double(dim,&(thermal->kh));

		//zxy_p_object
		free_heat_zxy_p_object(dim, &(thermal->obj));

		//long double

		matrix_free(sim,mx);
		mesh_obj_free(sim,&(thermal->mesh_data));
		dim_heat_free(dim);

		printf_log(sim,_("Freeing memory from the heat model\n"));
	}
}
