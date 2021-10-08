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
