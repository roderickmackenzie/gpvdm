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

/** @file newton_interface.c
	@brief Load and run the newton solve .dll/.so file.  They are hot swappable hence the interface.
*/


#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

	#define _GNU_SOURCE
	#include <dlfcn.h>

#include "inp.h"
#include "light_interface.h"
#include "gpvdm_const.h"
#include "device.h"
#include "dump_ctrl.h"
#include "config.h"
#include "cal_path.h"
#include "lang.h"
#include "log.h"
#include "newton_interface.h"

static int unused __attribute__((unused));


void newton_init(struct simulation *sim,struct device *dev,char *solver_name)
{
//printf_log(sim,_("Solver initialization\n"));
char lib_path[1000];

find_dll(sim, lib_path,solver_name);


char *error;

	dev->dll_solver_handle = dlopen(lib_path, RTLD_LAZY);

	if (!dev->dll_solver_handle)
	{
		ewe(sim,"%s\n", dlerror());
	}

	dev->dll_solve_cur = dlsym(dev->dll_solver_handle, "dll_solve_cur");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}

	dev->dll_solver_realloc = dlsym(dev->dll_solver_handle, "dll_solver_realloc");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}

	dev->dll_solver_free_memory = dlsym(dev->dll_solver_handle, "dll_solver_free_memory");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}



}


void newton_set_min_ittr(struct device *dev,int ittr)
{
	dev->newton_min_itt=ittr;
}

void solver_realloc(struct simulation *sim,struct device * dev)
{
	if (dev->dll_solver_realloc!=NULL)
	{
		(*dev->dll_solver_realloc)(sim,dev);
	}
}

void solver_free_memory(struct simulation *sim,struct device * dev)
{
	if (dev->dll_solver_free_memory!=NULL)
	{
		(*dev->dll_solver_free_memory)(sim,dev);
	}
}

void newton_interface_free(struct simulation *sim,struct device * dev)
{
	if (dev->dll_solver_handle!=NULL)
	{
		dlclose(dev->dll_solver_handle);
	}
	dev->dll_solver_free_memory=NULL;
	dev->dll_solver_realloc=NULL;
	dev->dll_solve_cur=NULL;
}
