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

/** @file complex_solver_interface.c
	@brief Load the complex sparse matrix solver .so/.dll.  If this is UMFPACK the plugin will call UMFPACK, for other custom solvers the work will be done in the plugin.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>

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

static int unused __attribute__((unused));

void complex_solver_init(struct simulation *sim,char *solver_name)
{
char lib_path[PATH_MAX];

	if (sim->dll_complex_matrix_handle==NULL)
	{
		find_dll(sim, lib_path,solver_name);


			char *error;

			sim->dll_complex_matrix_handle = dlopen(lib_path, RTLD_LAZY |RTLD_GLOBAL);

			if (!sim->dll_complex_matrix_handle)
			{
				ewe(sim, "%s\n", dlerror());
			}

			sim->dll_complex_matrix_solve = dlsym(sim->dll_complex_matrix_handle, "dll_complex_matrix_solve");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

			sim->dll_complex_matrix_solver_free = dlsym(sim->dll_complex_matrix_handle, "dll_complex_matrix_solver_free");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

			sim->dll_complex_matrix_init = dlsym(sim->dll_complex_matrix_handle, "dll_complex_matrix_init");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

	}


}

void complex_solver_get_mem(struct simulation *sim,struct matrix_solver_memory *msm)
{
	(*sim->dll_complex_matrix_init)(msm);
}

void complex_solver_free(struct simulation *sim,struct matrix_solver_memory *msm)
{
	if (sim->dll_complex_matrix_handle!=NULL)
	{
		(*sim->dll_complex_matrix_solver_free)(msm);
		matrix_solver_memory_free(sim,msm);
	}
}

void complex_solver_unload_dll(struct simulation *sim)
{
	if (sim->dll_complex_matrix_handle!=NULL)
	{
		//printf_log(sim,"%s=%p\n",_("Freeing memory"),sim->dll_complex_matrix_handle);

		if (dlclose(sim->dll_complex_matrix_handle)!=0)
		{
			ewe(sim,"%s\n",_("Error closing dll"));
		}

		sim->dll_complex_matrix_handle=NULL;
		sim->dll_complex_matrix_solve=NULL;
		sim->dll_complex_matrix_solver_free=NULL;
		sim->dll_complex_matrix_init=NULL;
	}
}

