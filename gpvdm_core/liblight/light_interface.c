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

/** @file light_interface.c
	@brief Acts as an interface between the model and the light dlls for each optical model.
*/

	#include <dlfcn.h>

#include "util.h"
#include "inp.h"
#include "light_interface.h"
#include "gpvdm_const.h"
#include "device.h"
#include "dump_ctrl.h"
#include "config.h"
#include "cal_path.h"
#include "lang.h"
#include "log.h"
#include "sim.h"
#include "memory.h"
#include <light_fun.h>
#include <matrix.h>

static int unused __attribute__((unused));

void light_load_dlls(struct simulation *sim,struct light *li)
{
	char lib_path[PATH_MAX];
	char lib_name[100];

	//printf_log(sim,"%s\n",_("Initializing optical model"));

	sprintf(lib_name,"light_%s",li->mode);
	find_dll(sim, lib_path,lib_name);


	char *error;

	li->lib_handle = dlopen(lib_path, RTLD_LAZY);

	if (!li->lib_handle)
	{
		ewe(sim, "%s\n", dlerror());
	}

	li->fn_init = dlsym(li->lib_handle, "light_dll_init");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}

	li->fn_solve_lam_slice = dlsym(li->lib_handle, "light_dll_solve_lam_slice");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}

	li->light_ver = dlsym(li->lib_handle, "light_dll_ver");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}



(*li->light_ver)(sim);
(*li->fn_init)(sim);
}



void light_free(struct simulation *sim,struct light *li)
{
	light_free_memory(sim,li);
}
