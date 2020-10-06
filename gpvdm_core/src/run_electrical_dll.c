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

/** @file run_electrical_dll.c
@brief find an electrical dll and run it.
*/


#include <stdio.h>
#include <stdlib.h>
#include <dll_export.h>
#include <log.h>
#include <lang.h>
#include <util.h>

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

	#ifndef disable_dlclose
		dlclose(lib_handle);
	#endif

}
