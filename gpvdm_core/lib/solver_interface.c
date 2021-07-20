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

/** @file solver_interface.c
	@brief Load the sparse matrix solver .so/.dll.  If this is UMFPACK the plugin will call UMFPACK, for other custom solvers the work will be done in the plugin.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>

	#define _GNU_SOURCE
	#include <dlfcn.h>

#include "util.h"
#include "inp.h"
#include "gpvdm_const.h"
#include "device.h"
#include "dump_ctrl.h"
#include "config.h"
#include "cal_path.h"
#include <lang.h>
#include <log.h>
#include <code_ctrl.h>

static int unused __attribute__((unused));

void solver_init(struct simulation *sim,char *solver_name)
{
char lib_path[PATH_MAX];




	if (sim->dll_matrix_handle==NULL)
	{
		find_dll(sim, lib_path,solver_name);
			char *error;


			sim->dll_matrix_handle = dlopen(lib_path, RTLD_LAZY |RTLD_GLOBAL);
			if (!sim->dll_matrix_handle)
			{
				ewe(sim, "%s\n", dlerror());
			}

			sim->dll_matrix_solve = dlsym(sim->dll_matrix_handle, "dll_matrix_solve");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

			sim->dll_matrix_solver_free = dlsym(sim->dll_matrix_handle, "dll_matrix_solver_free");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

			sim->dll_matrix_init = dlsym(sim->dll_matrix_handle, "dll_matrix_init");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

	}


}

void solver_get_mem(struct simulation *sim,struct matrix_solver_memory *msm)
{

	(*sim->dll_matrix_init)(msm);
}

void solver_free(struct simulation *sim,struct matrix_solver_memory *msm)
{
	if (sim->dll_matrix_handle!=NULL)
	{
		(*sim->dll_matrix_solver_free)(msm);
		matrix_solver_memory_free(sim,msm);
	}

}

void solver_unload_dll(struct simulation *sim)
{
	printf("unload DLLs\n");
	if (sim->dll_matrix_handle!=NULL)
	{


			//#ifndef disable_dlclose
			//printf(">>>>>>>>>dealloc %p\n",sim->dll_matrix_handle);
			if (dlclose(sim->dll_matrix_handle)!=0)
			{
				ewe(sim,"%s\n",_("Error closing dll"));
			}
			//#endif
			sim->dll_matrix_handle=NULL;
			sim->dll_matrix_solve=NULL;
			sim->dll_matrix_solver_free=NULL;
			sim->dll_matrix_init=NULL;
	}
}
