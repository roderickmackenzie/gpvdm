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

/** @file memory.c
@brief get/free memory
*/


#include <enabled_libs.h>

	#define _GNU_SOURCE
	#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lang.h>
#include "sim.h"
#include "dump.h"
#include "mesh.h"
#include <math.h>
#include "log.h"
#include <solver_interface.h>
#include <circuit.h>
#include "memory.h"
#include "ray_fun.h"
#include "newton_tricks.h"
#include "shape.h"

void matrix_solver_memory_check_memory(struct simulation *sim,struct matrix_solver_memory *msm,int col,int nz)
{
double *dtemp;
int *itemp;
	if (msm->last_col!=col)
	{
		//printf("realloc\n");
		dtemp = realloc(msm->x,col*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->x=dtemp;
		}


		dtemp = realloc(msm->b,col*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->b=dtemp;
		}

		itemp = realloc(msm->Ap,(col+1)*sizeof(int));
		if (itemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->Ap=itemp;
		}
	}

	if (msm->last_nz!=nz)
	{
		//printf("realloc\n");

		itemp = realloc(msm->Ai,(nz)*sizeof(int));
		if (itemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->Ai=itemp;
		}

		dtemp  = realloc(msm->Ax,(nz)*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->Ax=dtemp;
		}

		dtemp  = realloc(msm->Tx,(nz)*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->Tx=dtemp;
		}

	}

	msm->last_col=col;
	msm->last_nz=nz;
}


void matrix_solver_memory_init(struct matrix_solver_memory *msm)
{
		//Matrix solver -	external dll
	msm->last_col=0;
	msm->last_nz=0;
	msm->x=NULL;
	msm->Ap=NULL;
	msm->Ai=NULL;
	msm->Ax=NULL;
	msm->b=NULL;
	msm->Tx=NULL;

	//Complex matrix solver -	external dll
	msm->c_last_col=0;
	msm->c_last_nz=0;
	msm->c_x=NULL;
	msm->c_xz=NULL;
	msm->c_Ap=NULL;
	msm->c_Ai=NULL;
	msm->c_Ax=NULL;
	msm->c_Az=NULL;
	msm->c_b=NULL;
	msm->c_bz=NULL;
	msm->c_Tx=NULL;
	msm->c_Txz=NULL;

	msm->fd_ext_solver=-1;
	msm->ext_solver_buf_size=-1;
	msm->ext_solver_buf=NULL;
	strcpy(msm->ext_solver_pipe_name,"");
	strcpy(msm->fname_from_solver,"");
	strcpy(msm->fname_to_solver,"");
	msm->fd_from_solver=-1;

	msm->x_matrix_offset=0;

	/*msm->dll_matrix_handle=NULL;
	msm->dll_matrix_solve=NULL;
	msm->dll_matrix_solver_free=NULL;
	msm->dll_matrix_init=NULL;*/
}

void matrix_solver_memory_load_dll(struct simulation *sim,struct matrix_solver_memory *msm)
{
	/*char lib_path[PATH_MAX];
	if (msm->dll_matrix_handle==NULL)
	{
		find_dll(sim, lib_path,sim->solver_name);
		#ifndef windows
			char *error;

			msm->dll_matrix_handle = dlopen(lib_path, RTLD_LAZY |RTLD_GLOBAL);
			//printf("here1\n");
			//msm->dll_matrix_handle = dlmopen(LM_ID_NEWLM,lib_path, RTLD_LAZY);
			//printf("here2\n");
			//printf("1%p\n",dlmopen(LM_ID_NEWLM,lib_path, RTLD_LAZY ));
			//printf("2%p\n",dlmopen(LM_ID_NEWLM,lib_path, RTLD_LAZY ));
			//getchar();
			if (!msm->dll_matrix_handle)
			{
				ewe(sim, "oh: %s\n", dlerror());
			}

			msm->dll_matrix_solve = dlsym(msm->dll_matrix_handle, "dll_matrix_solve");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

			msm->dll_matrix_solver_free = dlsym(msm->dll_matrix_handle, "dll_matrix_solver_free");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

			msm->dll_matrix_init = dlsym(msm->dll_matrix_handle, "dll_matrix_init");
			if ((error = dlerror()) != NULL)
			{
				ewe(sim, "%s\n", error);
			}

		#else

			msm->dll_matrix_handle = LoadLibrary(lib_path);
			if (msm->dll_matrix_handle==NULL)
			{
				ewe(sim,"%s %s\n",_("dll not loaded"),lib_path);
			}

			msm->dll_matrix_solve = (void*)GetProcAddress(msm->dll_matrix_handle, "dll_matrix_solve");
			if (msm->dll_matrix_solve==NULL)
			{
				ewe(sim,_("dll function dll_matrix_solve not found\n"));
			}


			msm->dll_matrix_solver_free = (void*)GetProcAddress(msm->dll_matrix_handle, "dll_matrix_solver_free");
			if (msm->dll_matrix_solver_free==NULL)
			{
				ewe(sim,_("dll function dll_matrix_solver_free not found\n"));
			}

			msm->dll_matrix_init = (void*)GetProcAddress(msm->dll_matrix_handle, "dll_matrix_init");
			if (msm->dll_matrix_init==NULL)
			{
				ewe(sim,_("dll function dll_matrix_init not found\n"));
			}
		#endif
	}*/
}

void matrix_solver_memory_free(struct simulation *sim,struct matrix_solver_memory *msm)
{
	//Real part
	if (msm->x!=NULL)
	{
		free(msm->x);
		msm->x=NULL;	
	}

	if (msm->Ap!=NULL)
	{
		free(msm->Ap);
		msm->Ap=NULL;	
	}

	if (msm->Ai!=NULL)
	{
		free(msm->Ai);
		msm->Ai=NULL;	
	}

	if (msm->Ax!=NULL)
	{
		free(msm->Ax);
		msm->Ax=NULL;	
	}

	if (msm->b!=NULL)
	{
		free(msm->b);
		msm->b=NULL;	
	}

	if (msm->Tx!=NULL)
	{
		free(msm->Tx);
		msm->Tx=NULL;	
	}

	//Complex part
	if (msm->c_x!=NULL)
	{
		free(msm->c_x);
		msm->c_x=NULL;	
	}

	if (msm->c_xz!=NULL)
	{
		free(msm->c_xz);
		msm->c_xz=NULL;	
	}

	if (msm->c_b!=NULL)
	{
		free(msm->c_b);
		msm->c_b=NULL;	
	}

	if (msm->c_bz!=NULL)
	{
		free(msm->c_bz);
		msm->c_bz=NULL;	
	}

	if (msm->c_Ap!=NULL)
	{
		free(msm->c_Ap);
		msm->c_Ap=NULL;
	}

	if (msm->c_Ai!=NULL)
	{
		free(msm->c_Ai);
		msm->c_Ai=NULL;
	}

	if (msm->c_Ax!=NULL)
	{
		free(msm->c_Ax);
		msm->c_Ax=NULL;
	}

	if (msm->c_Az!=NULL)
	{
		free(msm->c_Az);
		msm->c_Az=NULL;
	}

	if (msm->c_Tx!=NULL)
	{
		free(msm->c_Tx);
		msm->c_Tx=NULL;
	}

	if (msm->c_Txz!=NULL)
	{
		free(msm->c_Txz);
		msm->c_Txz=NULL;
	}

	msm->c_last_col=0;
	msm->c_last_nz=0;

	msm->last_col=0;
	msm->last_nz=0;

	/*if (msm->dll_matrix_handle!=NULL)
	{

		#ifdef windows
			FreeLibrary(msm->dll_matrix_handle);
		#else

			//#ifndef disable_dlclose
			//printf(">>>>>>>>>dealloc %p\n",sim->dll_matrix_handle);
			if (dlclose(msm->dll_matrix_handle)!=0)
			{
				ewe(sim,"%s\n",_("Error closing dll"));
			}
			//#endif
			msm->dll_matrix_handle=NULL;
			msm->dll_matrix_solve=NULL;
			msm->dll_matrix_solver_free=NULL;
			msm->dll_matrix_init=NULL;
		#endif
	}*/
}

