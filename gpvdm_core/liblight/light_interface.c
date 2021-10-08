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

/** @file light_interface.c
	@brief Acts as an interface between the model and the light dlls for each optical model.
*/
#include <enabled_libs.h>

	#define _GNU_SOURCE
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
