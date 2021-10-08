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

/** @file run_electrical_dll.c
@brief find an electrical dll and run it.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <dll_export.h>
#include <log.h>
#include <lang.h>
#include <util.h>

	#define _GNU_SOURCE
	#include <dlfcn.h>

#include <cal_path.h>
#include <code_ctrl.h>

void run_electrical_dll(struct simulation *sim,struct device *in,char *dll_name)
{
	void *lib_handle;
	void (*init)();
	void (*dll_sim_run)();


	char lib_path[200];

	printf_log(sim,_("Loading electrical dll\n"));

	find_dll(sim, lib_path, dll_name);


	char *error;

	lib_handle = dlopen(lib_path, RTLD_LAZY);

	if (!lib_handle)
	{
		ewe(sim, "%s\n", dlerror());
	}

	init = dlsym(lib_handle, "set_interface");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}

	dll_sim_run = dlsym(lib_handle, "dll_run_simulation");
	if ((error = dlerror()) != NULL)
	{
		ewe(sim, "%s\n", error);
	}


(*dll_sim_run)(sim,in);

	//#ifndef disable_dlclose
		dlclose(lib_handle);
	//#endif

}
